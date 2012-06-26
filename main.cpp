#include <QApplication>
#include <QSystemTrayIcon>
#include <QSettings>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Clint");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("sje");
    a.setOrganizationDomain("scottellis.com.au");

    // select port
    unsigned short port = 34123;
    for(int i = 1; i < argc - 1; i+=2) {
        if(a.arguments().at(i) == "-p" || a.arguments().at(i) == "--port") {
            port = a.arguments().at(i + 1).toShort();
        }
    }

    MainWindow w(port);

    // add nodes to send to
    for(int i = 1; i < argc - 1; i+=2) {
        if(a.arguments().at(i) == "-n" || a.arguments().at(i) == "--node") {
            w.addNode(QHostAddress(a.arguments().at(i + 1)));
        }
    }

    QSystemTrayIcon sysTray(QIcon(":/images/clipboard"), &a);
    w.connect(&sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), &w, SLOT(sysTrayActivate(QSystemTrayIcon::ActivationReason)));

    sysTray.show();

    return a.exec();
}
