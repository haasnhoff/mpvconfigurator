#include "mainwindow.hpp"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QCoreApplication::setOrganizationName("HaasnSoft");
    QCoreApplication::setOrganizationDomain("mpvconfigurator");
    QCoreApplication::setApplicationName("mpv Configurator");
    w.show();

    return a.exec();
}
