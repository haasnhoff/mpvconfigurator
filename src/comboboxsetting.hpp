#ifndef COMBOBOXSETTING_HPP
#define COMBOBOXSETTING_HPP
#include <setting.hpp>
#include <QComboBox>
#include <QFontComboBox>

class comboboxSetting : public setting {

    Q_OBJECT

public:
    QComboBox* combobox;

    comboboxSetting(QComboBox* box, QCheckBox* checkBox, QStringList* manuals);
    void setComboBox(QComboBox* box);
    QComboBox* getComboBox();
    QString getValue();
    ~comboboxSetting();
};

class fontBoxSetting : public setting {

    Q_OBJECT

public:
    QFontComboBox* combobox;

    fontBoxSetting(QFontComboBox* box, QCheckBox* checkBox, QStringList* manuals);
    void setComboBox(QFontComboBox* box);
    QFontComboBox* getComboBox();
    QString getValue();
    ~fontBoxSetting();
};

#endif // COMBOBOXSETTING_HPP
