#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QClipboard>
#include <QMimeData>
#include <QCloseEvent>
#include <QDebug>
#include <QSettings>
#include <QTimer>


MainWindow::MainWindow(unsigned short p, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(0),
    port(p)
{
    ui->setupUi(this);

    // busted in Qt5
    //connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipboardChanged()));
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(textActivated()));

    currentText = QApplication::clipboard()->text();

    QTimer *timer = new QTimer(this);
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()), this, SLOT(pollClipboard()));
    timer->start();

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, port);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));

    QSettings s;
    if(s.contains("geom")) {
        QByteArray geom = s.value("geom").toByteArray();
        restoreGeometry(geom);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
}

void MainWindow::sysTrayActivate(QSystemTrayIcon::ActivationReason reason) {
    qDebug() << "Activate reason:" << reason;
    if(reason == QSystemTrayIcon::Context || reason == QSystemTrayIcon::DoubleClick) {
        qApp->quit();
    } else {
        show();
        raise();
        activateWindow();
    }
}

void MainWindow::addNode(const QHostAddress& nodeAddress) {
    nodes.append(nodeAddress);
}

void MainWindow::addText(const QString &text) {
    if(ui->listWidget->count() > 0 && text == ui->listWidget->item(0)->text())
        return;

    QList<QListWidgetItem *> matches = ui->listWidget->findItems(text, Qt::MatchCaseSensitive);
    foreach(QListWidgetItem *i, matches) {
        delete i;
    }

    ui->listWidget->insertItem(0, text);

    foreach(QHostAddress addy, nodes) {
        socket->writeDatagram(text.toUtf8(), addy, port);
    }
}

void MainWindow::clipboardChanged() {
    qDebug() << "Clipboard changed";
    if(qApp->clipboard()->mimeData()->hasText()) {
        addText(qApp->clipboard()->mimeData()->text());
    }
}

void MainWindow::closeEvent(QCloseEvent *e) {
    e->ignore();
    QSettings s;
    s.setValue("geom", this->saveGeometry());
    hide();
}

void MainWindow::textActivated() {
    qApp->clipboard()->setText(ui->listWidget->currentItem()->text());
    QSettings s;
    s.setValue("geom", this->saveGeometry());
    hide();
}

void MainWindow::readDatagrams() {
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qApp->clipboard()->setText(QString::fromUtf8(datagram.data()));
    }
}

void MainWindow::pollClipboard()
{
    if(QApplication::clipboard()->text() != currentText) {
        currentText = QApplication::clipboard()->text();
        clipboardChanged();
    }
}

