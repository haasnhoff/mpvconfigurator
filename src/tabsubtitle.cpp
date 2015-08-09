#include "tabsubtitle.hpp"
#include "ui_tabsubtitle.h"

tabsubtitle::tabsubtitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabsubtitle)
{
    ui->setupUi(this);

    subtitleSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");

    addSubtitleSetting(ui->box_no_sub);
    addSubtitleSetting(ui->value_sub_delay, ui->box_sub_delay);
    addSubtitleSetting(ui->comboBox_sub_scale_by_window, ui->box_sub_scale_by_window);
    addSubtitleSetting(ui->comboBox_sub_scale_with_window, ui->box_sub_scale_with_window);
    addSubtitleSetting(ui->comboBox_ass_scale_with_window, ui->box_ass_scale_with_window);
    addSubtitleSetting(ui->box_no_embedded_fonts);
    addSubtitleSetting(ui->comboBox_ass_hinting, ui->box_ass_hinting);
    addSubtitleSetting(ui->comboBox_ass_sharper, ui->box_ass_sharper);
    addSubtitleSetting(ui->value_ass_force_style, ui->box_ass_force_style);
    addSubtitleSetting(ui->comboBox_ass_override, ui->box_ass_style_override);
    addSubtitleSetting(ui->comboBox_sub_auto, ui->box_sub_auto);
    addSubtitleSetting(ui->comboBox_stretch_dvd_subs, ui->box_stretch_dvd_subs);
    addSubtitleSetting(ui->box_no_sub_fix_timing);
    addSubtitleSetting(ui->value_sub_speed, ui->box_sub_speed);
    addSubtitleSetting(ui->box_demuxer_mkv_subtitle_preroll);
    addSubtitleSetting(ui->valuebox_slang, ui->box_slang);

    // Subtitle looks.
    addSubtitleSetting(ui->horizontalSlider_sub_text_font_size, ui->label_sub_text_font_size_counter, 1, ui->box_sub_text_font_size);
    addSubtitleSetting(ui->horizontalSlider_sub_text_font_shadow_offset, ui->label_sub_text_shadow_offset_counter, 2, ui->box_sub_text_font_shadow_offset);
    addSubtitleSetting(ui->horizontalSlider_sub_text_font_border_size, ui->label_sub_text_border_size_counter, 2, ui->box_sub_text_font_border_size);
    addSubtitleSetting(ui->value_sub_text_font_color, ui->box_sub_text_font_color);
    addSubtitleSetting(ui->value_sub_text_border_color, ui->box_sub_text_border_color);
    addSubtitleSetting(ui->value_sub_text_shadow_color, ui->box_sub_text_shadow_color);
    subtitleSettings->push_back(new fontBoxSetting(ui->fontComboBox_sub_text_font, ui->box_sub_text_font, manualPages));
    connect(ui->fontComboBox_sub_text_font, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTestText(int)));

    colorPickerFontColor= new QColorDialog();
    colorPickerFontColor->setOption(QColorDialog::ShowAlphaChannel);
    colorPickerBorderColor= new QColorDialog();
    colorPickerBorderColor->setOption(QColorDialog::ShowAlphaChannel);
    colorPickerShadowColor= new QColorDialog();
    colorPickerShadowColor->setOption(QColorDialog::ShowAlphaChannel);

    // Font color dialog
    connect(ui->button_pick_color_font_color, SIGNAL(clicked()), colorPickerFontColor, SLOT(open()));
    connect(colorPickerFontColor, SIGNAL(colorSelected(QColor)), this, SLOT(setSubFontColor(QColor)));

    // Border color dialog
    connect(ui->button_pick_color_border_color, SIGNAL(clicked()), colorPickerBorderColor, SLOT(open()));
    connect(colorPickerBorderColor, SIGNAL(colorSelected(QColor)), this, SLOT(setSubBorderColor(QColor)));

    // Shadow color dialog.
    connect(ui->button_pick_color_shadow_color, SIGNAL(clicked()), colorPickerShadowColor, SLOT(open()));
    connect(colorPickerShadowColor, SIGNAL(colorSelected(QColor)), this, SLOT(setSubShadowColor(QColor)));

    connect(ui->horizontalSlider_sub_text_font_border_size, SIGNAL(valueChanged(int)), this, SLOT(updateTestText(int)));
    connect(ui->horizontalSlider_sub_text_font_size, SIGNAL(valueChanged(int)), this, SLOT(updateTestText(int)));
    connect(ui->horizontalSlider_sub_text_font_shadow_offset, SIGNAL(valueChanged(int)), this, SLOT(updateTestText(int)));

    subtitleScene = new QGraphicsScene(this);
    ui->subtitle_test_graphics_view->setScene(subtitleScene);
    testSubtitle = new QGraphicsSimpleTextItem();
    testSubtitle->setText("I am a special little subtitle.");
    testShadow = new QGraphicsDropShadowEffect();
    updateTestText(0);                                                  // Updates the shown subtitle in the GraphicsScene.
    subtitleScene->addItem(testSubtitle);
    //subtitleScene->setSceneRect(0,0,700,100);                           // Very specific boundaries. Should maybe fix?
}

void tabsubtitle::parseToConf(QStringList* conf){
    conf->append("\n# Subtitle settings");
    for(auto & it : *subtitleSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else
                conf->append(QString(it->getName()).append("=").append(it->getValue()));
        }
    }
}

void tabsubtitle::addSubtitleSetting(QCheckBox* box) {
    subtitleSettings->push_back(new setting(box, manualPages));
}

void tabsubtitle::addSubtitleSetting(QComboBox* comboBox, QCheckBox* box) {
    subtitleSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabsubtitle::addSubtitleSetting(QLineEdit* valueBox, QCheckBox* box) {
    subtitleSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

void tabsubtitle::addSubtitleSetting(QSlider* slider, QLabel* valueCounter, int divideBy, QCheckBox* box) {
    subtitleSettings->push_back(new sliderSetting(slider, valueCounter, divideBy, box, manualPages));
}

// Subtitle viewer methods

void tabsubtitle::setSubFontColor(QColor i){
    fontColor = i;
    ui->value_sub_text_font_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
    updateTestText(0);
}

void tabsubtitle::setSubBorderColor(QColor i){
    borderColor = i;
    ui->value_sub_text_border_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
    updateTestText(0);
}

void tabsubtitle::setSubShadowColor(QColor i){
    shadowColor = i;
    ui->value_sub_text_shadow_color->setText(QString('"').append(i.name(QColor::HexArgb)).append('"'));
    updateTestText(0);
}

void tabsubtitle::updateTestText(int i){
    QFont newFont;
    newFont.setFamily(ui->fontComboBox_sub_text_font->currentText());
    newFont.setPointSize(ui->horizontalSlider_sub_text_font_size->value());
    if(newFont.pointSize() > 90)
        testSubtitle->setText("I am a special BIG subtitle.");
    else
        testSubtitle->setText("I am a special little subtitle.");
    QBrush borderBrush(fontColor);
    QPen borderPen(borderColor);
    borderPen.setWidth(ui->horizontalSlider_sub_text_font_border_size->value());
    borderPen.setStyle(Qt::SolidLine);
    testSubtitle->setFont(newFont);
    testSubtitle->setPen(borderPen);
    testSubtitle->setBrush(borderBrush);
    if(ui->horizontalSlider_sub_text_font_shadow_offset->value() != 0){
        testShadow->setColor(shadowColor);
        testShadow->setOffset(ui->horizontalSlider_sub_text_font_shadow_offset->value());
        testSubtitle->setGraphicsEffect(testShadow);
    }
    else
        testShadow->setColor(QColor("black"));
}

void tabsubtitle::applyFromReadConf(QMap<QString, QString>* conf){
     for(auto & readSetting : conf->keys()) {
        for(auto & it : *subtitleSettings){
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
                conf->remove(readSetting);                                                                             // To find unknown settings added by user
            }
        }
    }
     fontColor.setNamedColor(ui->value_sub_text_font_color->text().remove('"'));                                       // Removes the quotes required for the config.
     setSubFontColor(fontColor);                                                                                       // Converts the hex into a QColor.
     borderColor.setNamedColor(ui->value_sub_text_border_color->text().remove('"'));
     setSubBorderColor(borderColor);
     shadowColor.setNamedColor(ui->value_sub_text_shadow_color->text().remove('"'));
     setSubShadowColor(shadowColor);
}

tabsubtitle::~tabsubtitle()
{
    delete subtitleSettings;
    delete manualPages;
    delete colorPickerFontColor;
    delete colorPickerBorderColor;
    delete colorPickerShadowColor;
    delete testShadow;
    delete testSubtitle;
    delete subtitleScene;
    delete ui;
}
