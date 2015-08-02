#include "mainwindow.hpp"
#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int movetocenterX = (a.desktop()->screenGeometry().width()-w.width()) / 2;          // Center the application
    int movetocenterY = (a.desktop()->screenGeometry().height()-w.height()) / 2;
    w.move(movetocenterX, movetocenterY);
    QCoreApplication::setApplicationName("mpv Configurator");
    w.show();

    return a.exec();
}
