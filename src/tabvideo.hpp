#ifndef TABVIDEO_HPP
#define TABVIDEO_HPP

#include <QWidget>
#include <vector>
#include <QFileDialog>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"

namespace Ui {
class tabVideo;
}

class tabVideo : public QWidget
{
    Q_OBJECT

public:
    explicit tabVideo(QWidget *parent = 0);
    std::vector<setting*>* voSettings;
    std::vector<setting*>* videoSettings;
    QStringList* manualPages;
    QString vo = "vo=";
    void addVOSetting(QCheckBox* box);
    void addVOSetting(QComboBox* comboBox, QCheckBox* box);
    void addVOSetting(QLineEdit* valueBox, QCheckBox* box);
    void addVideoSetting(QCheckBox* box);
    void addVideoSetting(QComboBox* comboBox, QCheckBox* box);
    void addVideoSetting(QLineEdit* valueBox, QCheckBox* box);
    void parseToConf(QStringList* conf);
    void applyFromReadConf(QMap<QString, QString>* conf);
    void readVOLine(QString readVO);
    ~tabVideo();

public slots:
    void parseVOSettings();
    void parseVOSettings(QString);
    void changeDriver(bool);
    void browseForShader();
    void browseForICC();

private:
    Ui::tabVideo *ui;
};

#endif // TABVIDEO_HPP
