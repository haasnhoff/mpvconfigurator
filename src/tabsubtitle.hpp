#ifndef TABSUBTITLE_HPP
#define TABSUBTITLE_HPP

#include <QWidget>
#include <QColorDialog>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsDropShadowEffect>
#include <vector>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"
#include "slidersetting.h"

namespace Ui {
class tabsubtitle;
}

class tabsubtitle : public QWidget
{
    Q_OBJECT

public:
    explicit tabsubtitle(QWidget *parent = 0);
    QGraphicsSimpleTextItem* testSubtitle;
    QColor fontColor, borderColor, shadowColor;
    QGraphicsDropShadowEffect* testShadow;
    QColorDialog* colorPickerFontColor, *colorPickerBorderColor, *colorPickerShadowColor;
    QGraphicsScene* subtitleScene;
    std::vector<setting*>* subtitleSettings;
    QStringList* manualPages;

    void addSubtitleSetting(QCheckBox* box);
    void addSubtitleSetting(QComboBox* comboBox, QCheckBox* box);
    void addSubtitleSetting(QLineEdit* valueBox, QCheckBox* box);
    void addSubtitleSetting(QSlider* slider, QLabel* valueCounter, int divideBy, QCheckBox* box);
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~tabsubtitle();

private slots:
    void setSubFontColor(QColor);
    void setSubBorderColor(QColor);
    void setSubShadowColor(QColor);
    void updateTestText(int);

public slots:
    void parseToConf(QStringList*);

private:
    Ui::tabsubtitle *ui;
};

#endif // TABSUBTITLE_HPP
