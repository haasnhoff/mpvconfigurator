#ifndef TABOSD_HPP
#define TABOSD_HPP

#include <QWidget>
#include <setting.hpp>
#include <comboboxsetting.hpp>
#include <valueboxsetting.hpp>
#include <slidersetting.h>
#include <QColorDialog>

namespace Ui {
class tabosd;
}

class tabosd : public QWidget
{
    Q_OBJECT

public:
    explicit tabosd(QWidget *parent = 0);

    std::vector<setting*>* osdSettings;
    QColor fontColor, borderColor, shadowColor, backColor;
    QColorDialog* colorPickerFontColor, *colorPickerBorderColor,
    *colorPickerShadowColor, *colorPickerBackColor;
    QStringList* manualPages;

    void addOSDSetting(QCheckBox* box);
    void addOSDSetting(QComboBox* comboBox, QCheckBox* box);
    void addOSDSetting(QLineEdit* valueBox, QCheckBox* box);
    void addOSDSetting(QSlider* slider, QLabel* valueCounter, int divideBy, QCheckBox* box);
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~tabosd();

public slots:
    void parseToConf(QStringList*);

private slots:
    void setOSDFontColor(QColor);
    void setOSDBorderColor(QColor);
    void setOSDShadowColor(QColor);
    void setOSDBackColor(QColor);

private:
    Ui::tabosd *ui;
};

#endif // TABOSD_HPP
