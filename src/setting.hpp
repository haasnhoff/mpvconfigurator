#ifndef SETTING_HPP
#define SETTING_HPP
#include <QObject>
#include <QString>
#include <QCheckBox>
#include <QRegularExpression>
#include <QFile>
#include <QDebug>

class setting : public QObject {

    Q_OBJECT

public:
    QString name, description, type, value;
    bool settingStatus;
    QCheckBox* settingCheckBox;
    QStringList* manual;

    setting(QCheckBox* checkbox, QStringList* manuals);
    void setName(QString name);
    void setDescription(QString descriptor);
    void setCheckBox(QCheckBox* box);
    QString getName();
    QString getDescription();
    bool getSettingStatus();
    QCheckBox* getCheckBox();
    QString parseSetting();
    QString getType();
    virtual QString getValue();
    ~setting();

public slots:
    void toggleSetting(bool);
};

#endif // SETTING_HPP
