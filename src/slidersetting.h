#ifndef SLIDERSETTING_H
#define SLIDERSETTING_H

#include <QSlider>
#include <setting.hpp>
#include <QLabel>

class sliderSetting : public setting{

    Q_OBJECT

public:
    QSlider* slider;
    int divideBy;
    QLabel* shownValue;
    sliderSetting(QSlider* aSlider, QLabel* valuer, int divideItBy, QCheckBox* checkBox, QStringList* manuals);

    void setSlider(QSlider* aSlider);
    int getDivideBy();
    QSlider* getSlider();
    QString getValue();
    ~sliderSetting();

public slots:
    void updateShownValue(int);
};

#endif // SLIDERSETTING_H
