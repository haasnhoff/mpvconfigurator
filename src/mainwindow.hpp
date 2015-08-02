#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include "tabconfig.hpp"
#include "tabvideo.hpp"
#include "tabaudio.hpp"
#include "tabsubtitle.hpp"
#include "tabosc.hpp"
#include "tabosd.hpp"
#include "tabscreenshot.hpp"
#include "tabwindow.hpp"
#include "tabmisc.hpp"
#include "tabextensions.hpp"
#include "unknownsettingstab.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    // Needs to be initialized with variables to be able to call them later for config parsing.
    tabconfig* tabConfig;
    tabVideo* tabvideo;
    tabaudio* tabAudio;
    tabsubtitle* tabSubtitle;
    tabosc* tabOSC;
    tabosd* tabOSD;
    tabScreenshot* tabscreenshot;
    tabWindow* tabwindow;
    tabMisc* tabmisc;
    tabExtensions* tabextensions;
    unknownsettingstab* tabunknown;
    QFile* savedConf;

    QString parseSetting();
    QString parseComboboxSetting(QString* settingName, QObject* objectToChange);
    ~MainWindow();

public slots:
    void parseFullConf();
    void readFullConf();
    void deleteAllSettings(bool);
    void saveAs();
    void messageClose();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
