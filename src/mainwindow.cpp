#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Needs to be initialized for parseFullConf.
    tabConfig = new tabconfig();
    tabvideo = new tabVideo();
    tabAudio = new tabaudio();
    tabSubtitle = new tabsubtitle();
    tabOSC = new tabosc();
    tabOSD = new tabosd();
    tabscreenshot = new tabScreenshot();
    tabwindow = new tabWindow();
    tabmisc = new tabMisc();
    tabextensions = new tabExtensions();
    tabunknown = new unknownsettingstab();

    ui->tabWidget->addTab(tabConfig, "Config");
    ui->tabWidget->addTab(tabvideo, "Video");
    ui->tabWidget->addTab(tabAudio, "Audio");
    ui->tabWidget->addTab(tabSubtitle, "Subtitles");
    ui->tabWidget->addTab(tabOSC, "OSC");
    ui->tabWidget->addTab(tabOSD, "OSD");
    ui->tabWidget->addTab(tabscreenshot, "Screenshots");
    ui->tabWidget->addTab(tabwindow, "Window");
    ui->tabWidget->addTab(tabmisc, "Misc");
    ui->tabWidget->addTab(tabextensions, "Extensions");
    ui->tabWidget->addTab(tabunknown, "Unknown Settings");
    ui->tabWidget->setStyleSheet("QTabBar::tab:hover { color: black; background: #e3e3e3 }"
                                "QTabBar::tab:selected { color: white; background: #722b72; }");   // Purple tab color.
    setWindowTitle("mpv Configurator Beta 0.2");
    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(deleteAllSettings(bool)));
    connect(ui->actionOpen_2, SIGNAL(triggered()), this, SLOT(readFullConf()));
    connect(ui->actionSave_2, SIGNAL(triggered()), this, SLOT(parseFullConf()));
    connect(ui->actionSave_as_2, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->action_Bar_Exit, SIGNAL(triggered()), this, SLOT(messageClose()));
    savedConf = new QFile();

    windowSettings = new QSettings();                                                              // Needs to be to new to be called from destructor.
    QByteArray tempArray = windowSettings->value("state/window/geometry").toByteArray();
    if (!tempArray.isEmpty()) {
        this->restoreGeometry(tempArray);
    }
}

void MainWindow::deleteAllSettings(bool withMessage){
    if(!withMessage){                                                                              // Ask before delete
        QMessageBox msgBox;
        msgBox.setText("All settings will be discarded.");
        msgBox.setInformativeText("Are you sure you want to start on a new config?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int answer = msgBox.exec();
        if(answer == QMessageBox::No)
            return;
    }

    ui->tabWidget->clear();

    delete tabAudio; delete tabConfig; delete tabextensions;
    delete tabmisc; delete tabOSC; delete tabOSD; delete tabscreenshot;
    delete tabSubtitle; delete tabunknown; delete tabvideo; delete tabwindow;

    tabConfig = new tabconfig();
    tabvideo = new tabVideo();
    tabAudio = new tabaudio();
    tabSubtitle = new tabsubtitle();
    tabOSC = new tabosc();
    tabOSD = new tabosd();
    tabscreenshot = new tabScreenshot();
    tabwindow = new tabWindow();
    tabmisc = new tabMisc();
    tabextensions = new tabExtensions();
    tabunknown = new unknownsettingstab();

    ui->tabWidget->addTab(tabConfig, "Config");
    ui->tabWidget->addTab(tabvideo, "Video");
    ui->tabWidget->addTab(tabAudio, "Audio");
    ui->tabWidget->addTab(tabSubtitle, "Subtitles");
    ui->tabWidget->addTab(tabOSC, "OSC");
    ui->tabWidget->addTab(tabOSD, "OSD");
    ui->tabWidget->addTab(tabscreenshot, "Screenshots");
    ui->tabWidget->addTab(tabwindow, "Window");
    ui->tabWidget->addTab(tabmisc, "Misc");
    ui->tabWidget->addTab(tabextensions, "Extensions");
    ui->tabWidget->addTab(tabunknown, "Unknown Settings");
}

void MainWindow::saveAs(){
    if(savedConf->exists())
        savedConf->setFileName("");                                 // Clear current path
    parseFullConf();
}

void MainWindow::messageClose(){
    QMessageBox closingMessage;
    closingMessage.setText("Do you want to save before exiting?");
    closingMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    closingMessage.setDefaultButton(QMessageBox::Cancel);
    int answer = closingMessage.exec();
    if(answer == QMessageBox::Yes){
        saveAs();
    }
    else if(answer == QMessageBox::Cancel){
        return;
    }
    this->close();
}

// Saves the config to a file.
void MainWindow::parseFullConf(){
    QStringList conf;
    tabvideo->parseToConf(&conf);
    tabAudio->parseToConf(&conf);
    tabSubtitle->parseToConf(&conf);
    tabOSC->parseToConf(&conf);
    tabOSD->parseToConf(&conf);
    tabscreenshot->parseToConf(&conf);
    tabwindow->parseToConf(&conf);
    tabmisc->parseToConf(&conf);
    tabunknown->parseToConf(&conf);
    tabextensions->parseToConf(&conf);

    if(!savedConf->exists()){
        QFileDialog saveDialog;
        QString filename = saveDialog.getSaveFileName(this,
                    tr("Please choose a save location..."), QDir::homePath() , tr("conf files (*.conf)"));
        if(filename.isNull()){
           return;
        }
        savedConf = new QFile(filename);
    }
    if (!savedConf->open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox msg;
        msg.setText("ERROR: COULD NOT READ FILE. CORRUPTED?");
        savedConf->close();
        delete savedConf;
        return;
    }
    else {
      QTextStream out(savedConf);
      for(auto & it : conf){
          out << it << "\n";
      }
      savedConf->close();
    }

}

// Reads the config from a file.
void MainWindow::readFullConf(){
    QMap<QString, QString> confMap;
    QFileDialog openDialog;
    QString fileName = openDialog.getOpenFileName(this, tr("Please choose a mpv.conf file."), QDir::homePath(), tr("conf files (*.conf)"));
    if(fileName.isNull())
        return;
    savedConf->setFileName(fileName);
    if(!savedConf->open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox msg;
        msg.setText("ERROR: COULD NOT READ FILE. CORRUPTED?");
        savedConf->close();
        delete savedConf;
        return;
    }
    else{
        deleteAllSettings(true);
        QTextStream in(savedConf);
        QString line;
        while (!in.atEnd() && !line.startsWith("[extension")) {
            if(line.startsWith("#") || line.isEmpty() || line.startsWith("\n")){
                // Ignore comments
            }
            else if(line.startsWith("vo=")){                                                // Override for vo= line
                tabvideo->readVOLine(line);
            }
            else if(line.startsWith("ao=")){                                                // Override for ao= line
                tabAudio->readAOLine(line);
            }
            else if(line.startsWith("script-opts") || line.startsWith("no-osc")){           // Override for OSC settings
                tabOSC->readFromConf(line);
            }
            else if(line.startsWith("ass-force-style")){                                    // Override for ass-force-style
                QString style = line.section("", 17);                                       // This is due to style settings can have = in the value.
                confMap.insert("ass-force-style", style);                                   // For example:  Kerning=Yes
            }
            else if(line.contains(QRegularExpression(".*=.*"))){                            // Normal setting with value.
                QStringList splitLine = line.split("=");
                confMap.insert(splitLine.first(), splitLine.last());
            }
            else{                                                                           // Setting without a value.
                confMap.insert(line, "");
            }
            line = in.readLine();                                                           // Read instead of first to break while when extensions arrive.
        }

        // Read every extension the user has added.
        while(!in.atEnd()){
            if(line.startsWith("[extension")){
                QString extensionName = line.remove("[extension").remove("]");
                QStringList extensionSettings;
                line = in.readLine();                                                       // Skip to the next line since current is still [extension].
                while(!in.atEnd() && !line.startsWith("[extension")){                       // Read all the settings until a new extension appears, or the conf is at the end.
                    extensionSettings.append(line);
                    line = in.readLine();
                }
                tabextensions->readExtensions(extensionName, extensionSettings);            // Add those settings to the extensions tab.
            }
            else
                line = in.readLine();
        }
    }
    tabvideo->applyFromReadConf(&confMap);
    tabAudio->applyFromReadConf(&confMap);
    tabSubtitle->applyFromReadConf(&confMap);
    tabOSD->applyFromReadConf(&confMap);
    tabmisc->applyFromReadConf(&confMap);
    tabscreenshot->applyFromReadConf(&confMap);
    tabwindow->applyFromReadConf(&confMap);
    tabunknown->applyFromReadConf(&confMap);
    savedConf->close();
}

MainWindow::~MainWindow()
{
    windowSettings->setValue("state/window/geometry", this->saveGeometry());
    // Cannot be comma'd due to operator precedence.
    delete tabAudio; delete tabConfig; delete tabextensions;
    delete tabmisc; delete tabOSC; delete tabOSD; delete tabscreenshot;
    delete tabSubtitle; delete tabunknown; delete tabvideo; delete tabwindow;
    delete savedConf;
    delete ui;
}
