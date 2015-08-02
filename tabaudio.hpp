#ifndef TABAUDIO_HPP
#define TABAUDIO_HPP

#include <QWidget>
#include <vector>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"


namespace Ui {
class tabaudio;
}

class tabaudio : public QWidget
{
    Q_OBJECT

public:
    explicit tabaudio(QWidget *parent = 0);
    std::vector<setting*>* aoSettings;
    std::vector<setting*>* audioSettings;
    QStringList* manualPages;
    QString ao = "ao=";
    void addAOSetting(QCheckBox* box);
    void addAOSetting(QComboBox* comboBox, QCheckBox* box);
    void addAOSetting(QLineEdit* valueBox, QCheckBox* box);
    void addAudioSetting(QCheckBox* box);
    void addAudioSetting(QComboBox* comboBox, QCheckBox* box);
    void addAudioSetting(QLineEdit* valueBox, QCheckBox* box);
    void applyFromReadConf(QMap<QString, QString>* settingMap);
    void readAOLine(QString readAO);
    ~tabaudio();

public slots:
    void parseAOSettings();
    void parseAOSettings(QString);
    void parseToConf(QStringList*);
    void changeDriver(bool);

private:
    Ui::tabaudio *ui;
};

#endif // TABAUDIO_HPP
