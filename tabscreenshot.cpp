#include "tabscreenshot.hpp"
#include "ui_tabscreenshot.h"

tabScreenshot::tabScreenshot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabScreenshot)
{
    ui->setupUi(this);    
    ui->widget_jpeg_settings->hide();
    ui->widget_png_settings->hide();
    ui->frame->hide();
    ui->label_custom_template->hide();
    ui->valuebox_custom_template->hide();

    screenshotSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");

    addScreenshotSetting(ui->valuebox_screenshot_directory, ui->box_screenshot_directory);
    addScreenshotSetting(ui->comboBox_screenshot_format, ui->box_screenshot_format);
    addScreenshotSetting(ui->comboBox_screenshot_tag_colorspace, ui->box_screenshot_tag_colorspace);
    addScreenshotSetting(ui->comboBox_screenshot_high_bit_depth, ui->box_screenshot_high_bit_depth);
    addScreenshotSetting(ui->combobox_screenshot_template, ui->box_screenshot_template);
    addScreenshotSetting(ui->valuebox_screenshot_jpeg_quality, ui->box_screenshot_jpeg_quality);
    addScreenshotSetting(ui->comboBox_jpeg_source_chroma, ui->box_screenshot_jpeg_source_chroma);
    addScreenshotSetting(ui->comboBox_png_compression, ui->box_screenshot_png_compression);
    addScreenshotSetting(ui->comboBox_png_filter, ui->box_screenshot_png_filter);

    connect(ui->comboBox_screenshot_format, SIGNAL(currentTextChanged(QString)), this, SLOT(enablePNGandJPEGWidgets(QString)));
    connect(ui->combobox_screenshot_template, SIGNAL(currentTextChanged(QString)), this, SLOT(enableDisableCustomTemplate(QString)));
    connect(ui->browse_button, SIGNAL(clicked()), this, SLOT(browseForScreenshotFolder()));
}


// Then there's this ugly piece of shit.
// Converts the visible choices into actual usable values.
QString tabScreenshot::convertTemplateToFormat(){
    QString output = ui->valuebox_screenshot_directory->text();
    switch(ui->combobox_screenshot_template->currentIndex()){
        case 0:
            output.append("%f-%n");
            break;
        case 1:
            output.append("%f-%td-%tm-%ty-%n");
            break;
        case 2:
          output.append("%f-%tm-%td-%ty-%n");
          break;
        case 3:
          output.append("%f-%td-%tm-%ty-at-%tH.%tM-%n");
          break;
        case 4:
          output.append("%f-%tm-%td-%ty-at-%tH.%tM-%n");
          break;
        case 5:
          output.append("%f-screenshot-%n");
          break;
        case 6:
          output.append("%f-screenshot-%td-%tm-%ty-%n");
          break;
        case 7:
          output.append("%f-screenshot-%tm-%td-%ty-%n");
          break;
        case 8:
          output.append("%f-screenshot-%td-%tm-%ty-at-%tH.%tM-%n");
          break;
        case 9:
          output.append("%f-screenshot-%tm-%td-%ty-at-%tH.%tM-%n");
          break;
        case 10:
          output.append("mpv-screenshot-%td-%tm-%ty-at-%tH.%tM-%n");
          break;
        case 11:
          output.append("mpv-screenshot-%tm-%td-%ty-at-%tH.%tM-%n");
          break;
        case 12:
          output.append("mpv-screenshot-%td-%tm-%ty-%n");
          break;
        case 13:
          output.append("mpv-screenshot-%tm-%td-%ty-%n");
          break;
        case 14:
          output.append("mpv-screenshot-%n");
          break;
        case 15:
          output.append(ui->valuebox_custom_template->text());
    }
    return output;
}

void tabScreenshot::addScreenshotSetting(QCheckBox* box) {
    screenshotSettings->push_back(new setting(box, manualPages));
}

void tabScreenshot::addScreenshotSetting(QComboBox* comboBox, QCheckBox* box) {
    screenshotSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabScreenshot::addScreenshotSetting(QLineEdit* valueBox, QCheckBox* box) {
    screenshotSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

// Clears all the file specific settings so that JPG won't suddenly have PNG settings or vice versa.
void tabScreenshot::enablePNGandJPEGWidgets(QString changedTo){
    if(changedTo.compare("png") == 0){
        ui->widget_jpeg_settings->setHidden(true);
        ui->widget_png_settings->setHidden(false);
        ui->box_screenshot_jpeg_quality->setChecked(false);
        ui->box_screenshot_jpeg_source_chroma->setChecked(false);
    }
    else if(changedTo.compare("jpg") == 0){
        ui->widget_jpeg_settings->setHidden(false);
        ui->widget_png_settings->setHidden(true);
        ui->box_screenshot_png_compression->setChecked(false);
        ui->box_screenshot_png_filter->setChecked(false);
    }
    else{
        ui->widget_jpeg_settings->setHidden(true);
        ui->widget_png_settings->setHidden(true);
        ui->box_screenshot_png_compression->setChecked(false);
        ui->box_screenshot_png_filter->setChecked(false);
        ui->box_screenshot_jpeg_quality->setChecked(false);
        ui->box_screenshot_jpeg_source_chroma->setChecked(false);
    }
}

// Shows the custom template guide.
void tabScreenshot::enableDisableCustomTemplate(QString changedTo){
    if(changedTo.compare("Custom") == 0){
        ui->frame->show();
        ui->label_custom_template->show();
        ui->valuebox_custom_template->show();
    }
    else{
        ui->frame->hide();
        ui->label_custom_template->hide();
        ui->valuebox_custom_template->hide();
    }
}

void tabScreenshot::parseToConf(QStringList* conf){
    conf->append("\n# Screenshot settings");
    for(auto & it : *screenshotSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else{
                if(it->getCheckBox()->text().compare("screenshot-template") == 0)
                    conf->append(QString(it->getName()).append("=").append(convertTemplateToFormat()));                                // Needed to convert example to actual format.
                else if(it->getCheckBox()->text().compare("screenshot-directory") == 0){
                    // Do nothing. It is automatically merged with screenshot-template.
                }
                else
                    conf->append(QString(it->getName()).append("=").append(it->getValue()));
            }
        }
    }
}

void tabScreenshot::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
        for(auto & it : *screenshotSettings){
            if(readSetting.compare(it->getName()) == 0){
                if(it->getName().compare("screenshot-template") == 0){
                    QStringList tempStringList = conf->value(readSetting).split(QDir::separator());
                    ui->box_screenshot_directory->setChecked(true);
                    ui->valuebox_screenshot_directory->setText(tempStringList.join(QDir::separator()).remove(tempStringList.last()));
                    ui->combobox_screenshot_template->setCurrentIndex(15);
                    ui->valuebox_custom_template->setText(tempStringList.last());
                }
                it->getCheckBox()->setChecked(true);
                if(it->getType().compare("combobox") == 0){
                    comboboxSetting* tempBox = (comboboxSetting*)it;
                    tempBox->getComboBox()->setCurrentText(conf->value(readSetting));
                }
                else if(it->getType().compare("valuebox") == 0){
                    valueboxSetting* tempBox = (valueboxSetting*)it;
                    tempBox->getValueBox()->setText(conf->value(readSetting));
                }
                conf->remove(readSetting);                                      // To find unknown settings added by user
            }
        }
    }
}

void tabScreenshot::browseForScreenshotFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this,
             tr("Open screenshot folder..."), QDir::homePath());
    ui->valuebox_screenshot_directory->setText(folderName.append(QDir::separator()));
}

tabScreenshot::~tabScreenshot()
{
    delete screenshotSettings;
    delete manualPages;
    delete ui;
}
