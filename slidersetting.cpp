#include "slidersetting.h"

sliderSetting::sliderSetting(QSlider* aSlider, QLabel* valuer, int divideItBy, QCheckBox* checkBox, QStringList* manuals) : slider{aSlider}, shownValue{valuer}, divideBy{divideItBy}, setting(checkBox, manuals) {
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(updateShownValue(int)));
    connect(settingCheckBox, SIGNAL(toggled(bool)), slider, SLOT(setEnabled(bool)));
    type = "slider";
}

void sliderSetting::setSlider(QSlider *box){
    slider = box;
}

QSlider* sliderSetting::getSlider(){
    return slider;
}

int sliderSetting::getDivideBy(){
    return divideBy;
}


// This way in case the slider does decimal increments, which slider->value() does not understand.
// Some of the settings have -0.7 for example, which means a custom slider is necessary, or just divide it to achieve the same thing.
void sliderSetting::updateShownValue(int i){
    if(i != 0){
        float converted = (float(i))/float(divideBy);
        shownValue->setText(QString::number(converted));           // Humans can unfortunately not divide by zero yet.
    }
    else
        shownValue->setText(QString::number(i));
}

// Since a slider can be in decimals, the label next to the slider is a more accurate value than the QSlider object's value.
QString sliderSetting::getValue(){
    return shownValue->text();
}

sliderSetting::~sliderSetting(){}
