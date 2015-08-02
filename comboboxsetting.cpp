#include "comboboxsetting.hpp"

comboboxSetting::comboboxSetting(QComboBox* box, QCheckBox* checkBox, QStringList* manuals) : combobox{box}, setting(checkBox, manuals) {
    type = "combobox";
      connect(settingCheckBox, SIGNAL(toggled(bool)), combobox, SLOT(setEnabled(bool)));
}

void comboboxSetting::setComboBox(QComboBox *box){
    combobox = box;
}

QComboBox* comboboxSetting::getComboBox(){
    return combobox;
}

QString comboboxSetting::getValue(){
    return combobox->currentText();
}

comboboxSetting::~comboboxSetting() {}

// ----------------------------------

fontBoxSetting::fontBoxSetting(QFontComboBox* box, QCheckBox* checkBox, QStringList* manuals) : combobox{box}, setting(checkBox, manuals) {
    type = "combobox";
    connect(settingCheckBox, SIGNAL(toggled(bool)), combobox, SLOT(setEnabled(bool)));
}

void fontBoxSetting::setComboBox(QFontComboBox *box){
    combobox = box;
}

QFontComboBox* fontBoxSetting::getComboBox(){
    return combobox;
}

QString fontBoxSetting::getValue(){
    return QString('"').append(combobox->currentText()).append('"');                // For writing config. Needs to be encased since many include whitespace.
}

fontBoxSetting::~fontBoxSetting() {}

