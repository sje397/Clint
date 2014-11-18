#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QUdpSocket>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(unsigned short p, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addText(const QString &text);
    void sysTrayActivate(QSystemTrayIcon::ActivationReason reason);
    void addNode(const QHostAddress& nodeAddress);

private slots:
    void clipboardChanged();
    void textActivated();
    void readDatagrams();

    void pollClipboard();

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket;
    unsigned short port;
    QList<QHostAddress> nodes;
    QString currentText;

    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
