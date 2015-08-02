#include "tabosd.hpp"
#include "ui_tabosd.h"


tabosd::tabosd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabosd)
{
    ui->setupUi(this);

    osdSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");

    addOSDSetting(ui->box_no_osd);
    addOSDSetting(ui->valuebox_osd_duration, ui->box_osd_duration);
    osdSettings->push_back(new fontBoxSetting(ui->fontComboBox, ui->box_osd_font, manualPages));
    addOSDSetting(ui->valuebox_osd_msg1, ui->box_osd_msg1);
    addOSDSetting(ui->valuebox_osd_msg2, ui->box_osd_msg2);
    addOSDSetting(ui->valuebox_osd_msg3, ui->box_osd_msg3);
    addOSDSetting(ui->valuebox_osd_status_msg, ui->box_osd_status_msg);
    addOSDSetting(ui->valuebox_osd_playing_msg, ui->box_osd_playing_msg);
    addOSDSetting(ui->box_osd_fractions);
    addOSDSetting(ui->comboBox_osd_level, ui->box_osd_level);
    addOSDSetting(ui->comboBox_osd_scale_by_window, ui->box_osd_scale_by_window);
    addOSDSetting(ui->comboBox_osd_align_x, ui->box_osd_align_x);
    addOSDSetting(ui->comboBox_osd_align_y, ui->box_osd_align_y);
    addOSDSetting(ui->comboBox_osd_bold, ui->box_osd_font_bold);

    addOSDSetting(ui->horizontalSlider_osd_bar_align_x, ui->slider_value_osd_align_x, 10, ui->box_osd_bar_align_x);
    addOSDSetting(ui->horizontalSlider_osd_bar_align_y, ui->slider_value_osd_align_y, 10, ui->box_osd_bar_align_y);
    addOSDSetting(ui->horizontalSlider_osd_bar_w, ui->slider_value_osd_bar_w, 1, ui->box_osd_bar_w);
    addOSDSetting(ui->horizontalSlider_osd_bar_h, ui->slider_value_osd_bar_h, 10, ui->box_osd_bar_h);
    addOSDSetting(ui->horizontalSlider_osd_margin_x, ui->slider_value_osd_margin_x, 1, ui->box_osd_margin_x);
    addOSDSetting(ui->horizontalSlider_osd_margin_y, ui->slider_value_osd_margin_y, 1, ui->box_osd_margin_y);
    addOSDSetting(ui->horizontalSlider_osd_blur, ui->slider_value_osd_blur, 10, ui->box_osd_blur);
    addOSDSetting(ui->horizontalSlider_osd_font_size, ui->slider_value_osd_font_size, 1, ui->box_osd_font_size);
    addOSDSetting(ui->horizontalSlider_osd_border_size, ui->slider_value_osd_border_size, 1, ui->box_osd_border_size);
    addOSDSetting(ui->horizontalSlider_osd_shadow_offset, ui->slider_value_osd_shadow_offset, 1, ui->box_osd_shadow_offset);
    addOSDSetting(ui->horizontalSlider_osd_spacing, ui->slider_value_osd_spacing, 1, ui->box_osd_spacing);

    addOSDSetting(ui->value_osd_back_color, ui->box_osd_back_color);
    addOSDSetting(ui->value_osd_color, ui->box_osd_color);
    addOSDSetting(ui->value_osd_border_color, ui->box_osd_border_color);
    addOSDSetting(ui->value_osd_shadow_color, ui->box_osd_shadow_color);

    colorPickerFontColor= new QColorDialog();
    colorPickerFontColor->setOption(QColorDialog::ShowAlphaChannel);
    colorPickerBorderColor= new QColorDialog();
    colorPickerBorderColor->setOption(QColorDialog::ShowAlphaChannel);
    colorPickerShadowColor= new QColorDialog();
    colorPickerShadowColor->setOption(QColorDialog::ShowAlphaChannel);
    colorPickerBackColor= new QColorDialog();
    colorPickerBackColor->setOption(QColorDialog::ShowAlphaChannel);

    connect(ui->button_pick_osd_color, SIGNAL(clicked()), colorPickerFontColor, SLOT(open()));
    connect(colorPickerFontColor, SIGNAL(colorSelected(QColor)), this, SLOT(setOSDFontColor(QColor)));

    connect(ui->button_pick_osd_border_color, SIGNAL(clicked()), colorPickerBorderColor, SLOT(open()));
    connect(colorPickerBorderColor, SIGNAL(colorSelected(QColor)), this, SLOT(setOSDBorderColor(QColor)));

    connect(ui->button_pick_osd_shadow_color, SIGNAL(clicked()), colorPickerShadowColor, SLOT(open()));
    connect(colorPickerShadowColor, SIGNAL(colorSelected(QColor)), this, SLOT(setOSDShadowColor(QColor)));

    connect(ui->button_pick_osd_back_color, SIGNAL(clicked()), colorPickerBackColor, SLOT(open()));
    connect(colorPickerBackColor, SIGNAL(colorSelected(QColor)), this, SLOT(setOSDBackColor(QColor)));
}

void tabosd::setOSDFontColor(QColor i){
    fontColor = i;
    ui->value_osd_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
}

void tabosd::setOSDBorderColor(QColor i){
    borderColor = i;
    ui->value_osd_border_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
}

void tabosd::setOSDShadowColor(QColor i){
    shadowColor = i;
    ui->value_osd_shadow_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
}

void tabosd::setOSDBackColor(QColor i){
    fontColor = i;
    ui->value_osd_back_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
}

void tabosd::addOSDSetting(QCheckBox* box) {
    osdSettings->push_back(new setting(box, manualPages));
}

void tabosd::addOSDSetting(QComboBox* comboBox, QCheckBox* box) {
    osdSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabosd::addOSDSetting(QLineEdit* valueBox, QCheckBox* box) {
    osdSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

void tabosd::addOSDSetting(QSlider* slider, QLabel* valueCounter, int divideBy, QCheckBox* box) {
    osdSettings->push_back(new sliderSetting(slider, valueCounter, divideBy, box, manualPages));
}

void tabosd::parseToConf(QStringList* conf){
    conf->append("\n# OSD settings");
    for(auto & it : *osdSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else{
                if(it->getName().contains(QRegularExpression(".*msg.*")) && !it->getValue().startsWith('"')){
                    conf->append(QString(it->getName()).append("=").append(QString('"').append(it->getValue()).append('"')));
                }
                else
                    conf->append(QString(it->getName()).append("=").append(it->getValue()));
            }
        }
    };
}

void tabosd::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
       for(auto & it : *osdSettings){
           if(readSetting.compare(it->getName()) == 0){
               it->getCheckBox()->setChecked(true);
               if(it->getType().compare("combobox") == 0){
                   comboboxSetting* tempBox = (comboboxSetting*)it;
                   tempBox->getComboBox()->setCurrentText(conf->value(readSetting));
               }
               else if(it->getType().compare("valuebox") == 0){
                   valueboxSetting* tempBox = (valueboxSetting*)it;
                   tempBox->getValueBox()->setText(conf->value(readSetting));
               }
               else if(it->getType().compare("slider") == 0){
                   sliderSetting* tempBox = (sliderSetting*)it;
                   tempBox->getSlider()->setValue((conf->value(readSetting).toFloat()*tempBox->getDivideBy()));       // Sets slider's position properly.
                   tempBox->updateShownValue(tempBox->getSlider()->value());                                          // Needed to set the label's proper value.
               }
               conf->remove(readSetting);                                                                              // To find unknown settings added by user
           }
       }
   }
}

tabosd::~tabosd()
{
    delete ui, manualPages, osdSettings,
        colorPickerFontColor, colorPickerBorderColor,
        colorPickerShadowColor, colorPickerBackColor;
}
