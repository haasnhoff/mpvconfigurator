#include "valueboxsetting.hpp"

valueboxSetting::valueboxSetting(QLineEdit* aValueBox, QCheckBox* checkBox, QStringList* manuals) : setting(checkBox, manuals), valueBox{aValueBox} {
  type = "valuebox";
  connect(settingCheckBox, SIGNAL(toggled(bool)), valueBox, SLOT(setEnabled(bool)));
}

void valueboxSetting::setValueBox(QLineEdit* aValueBox){
    valueBox = aValueBox;
}

QLineEdit* valueboxSetting::getValueBox(){
    return valueBox;
}

QString valueboxSetting::getValue(){
    return valueBox->text();
}

valueboxSetting::~valueboxSetting(){}
