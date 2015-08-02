#include "tabvideo.hpp"
#include "ui_tabvideo.h"

tabVideo::tabVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabVideo)
{
    ui->setupUi(this);
    ui->video_settings_direct3d->hide();
    ui->video_settings_opengl->hide();
    voSettings = new std::vector<setting*>();
    videoSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");
    manualPages->append(":/man/vf.rst");
    manualPages->append(":/man/vo.rst");

    connect(ui->radio_preset_opengl, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_openglhq, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_direct3d, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_direct3d_shaders, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->button_browse_source_shader, SIGNAL(clicked()), this, SLOT(browseForShader()));
    connect(ui->button_browse_icc_profile, SIGNAL(clicked()), this, SLOT(browseForICC()));

    // VO settings.
    addVOSetting(ui->scaler_combobox, ui->box_scale);
    addVOSetting(ui->dscale_combobox, ui->box_dscale);
    addVOSetting(ui->cscale_combobox, ui->box_cscale);
    addVOSetting(ui->tscale_combobox, ui->box_tscale);
    addVOSetting(ui->value_scale_radius, ui->box_scale_radius);
    addVOSetting(ui->value_scale_antiring, ui->box_scale_antiring);
    addVOSetting(ui->box_scale_resizes_only);
    addVOSetting(ui->box_fancy_downscaling);
    addVOSetting(ui->box_pbo);
    addVOSetting(ui->value_dither_depth, ui->box_dither_depth);
    addVOSetting(ui->box_temporal_dither);
    addVOSetting(ui->box_interpolation);
    addVOSetting(ui->box_waitvsync);
    addVOSetting(ui->value_gamma, ui->box_gamma);
    addVOSetting(ui->box_gamma_auto);
    addVOSetting(ui->target_prim_combobox, ui->box_target_prim);
    addVOSetting(ui->value_icc_profile, ui->box_icc_profile);
    addVOSetting(ui->box_icc_profile_auto);
    addVOSetting(ui->fbo_format_combobox, ui->box_fbo_format);
    addVOSetting(ui->value_source_shader, ui->box_source_shader);

    // Direct3D VO
    addVOSetting(ui->box_prefer_stretchrect);
    addVOSetting(ui->box_disablestretchrect);
    addVOSetting(ui->box_disable_textures);
    addVOSetting(ui->box_disable_shaders);
    addVOSetting(ui->box_only_8_bit);
    addVOSetting(ui->box_disable_texture_align);
    addVOSetting(ui->box_force_power_of_2);
    addVOSetting(ui->texture_memory_combobox, ui->box_texture_memory);

    // Video settings - Not connected to the vo=opengl:settings:here.
    addVideoSetting(ui->framedrop_combobox, ui->box_framedrop_mode);
    addVideoSetting(ui->deinterlace_combobox, ui->box_deinterlace);
    addVideoSetting(ui->fielddominance_combobox, ui->box_fielddominance);
    addVideoSetting(ui->value_video_aspect, ui->box_video_aspect);
    addVideoSetting(ui->box_no_video_aspect_2);
    addVideoSetting(ui->value_video_stereo_mode, ui->box_video_stereo_mode);
    addVideoSetting(ui->value_display_fps, ui->box_display_fps);
    addVideoSetting(ui->hwdec_combobox, ui->box_hwdec);
    addVideoSetting(ui->value_panscan, ui->box_panscan);
}

// VO Settings. These will connect to parseVOSetting to update the "VO String"
// box at the top of the widget.
void tabVideo::addVOSetting(QCheckBox* box) {
    voSettings->push_back(new setting(box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseVOSettings()));
}

void tabVideo::addVOSetting(QComboBox* comboBox, QCheckBox* box) {
    voSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseVOSettings()));
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(parseVOSettings(QString)));
}

void tabVideo::addVOSetting(QLineEdit* valueBox, QCheckBox* box) {
    voSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseVOSettings()));
    connect(valueBox, SIGNAL(editingFinished()), this, SLOT(parseVOSettings()));
}

// Video settings not in VO.
// These are not connected to parseVOSettings, which means they won't update the VO string.
void tabVideo::addVideoSetting(QCheckBox* box) {
    videoSettings->push_back(new setting(box, manualPages));
}

void tabVideo::addVideoSetting(QComboBox* comboBox, QCheckBox* box) {
    videoSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabVideo::addVideoSetting(QLineEdit* valueBox, QCheckBox* box) {
    videoSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}


// Changed to QStringList with join instead of regex manipulation with replace. Fuck regex with a stick.

void tabVideo::parseVOSettings(){
    QStringList tempVO;
    tempVO.append("");                      // Empty insert to create seperator between driver and first option if any option is chosen.
    for (auto & it : *voSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                    tempVO.append(it->getName());
            else
                tempVO.append(it->getName().append("=").append(it->getValue()));
        }
    }
    if(!tempVO.isEmpty())
        ui->textEdit->setText(QString(vo).append(tempVO.join(":")));
    else
        ui->textEdit->setText(vo);
}


// Only here to satifsy combobox SLOT requirements. Easier than using a signal mapper.
void tabVideo::parseVOSettings(QString becauseFuckSlots){
    parseVOSettings();
}

void tabVideo::parseToConf(QStringList* conf){
    parseVOSettings();
    conf->append("# Video settings");
    conf->append(ui->textEdit->toPlainText());
    for(auto & it : *videoSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else
                conf->append(QString(it->getName()).append("=").append(it->getValue()));
        }
    }
}

// Bool parameter is dummy to satisfy slot requirements.
// This changes the driver to another oen, clearing the settings if opengl<->direct3d, but only replaces if opengl<->opengl-hq
void tabVideo::changeDriver(bool slotnecessity){
    QString newDriver;
    if(ui->radio_preset_openglhq->isChecked())
        newDriver = "opengl-hq";
    else if(ui->radio_preset_direct3d->isChecked())
        newDriver = "direct3d";
    else if(ui->radio_preset_direct3d_shaders->isChecked())
        newDriver = "direct3d_shaders";
    else
        newDriver = "opengl";

    // Clears the string and unchecks all VO boxes if you change from opengl to direct3d or vice versa.
    if((vo.contains(QRegularExpression("vo=opengl.*")) && newDriver.contains(QRegularExpression("direct.*"))) ||
       (vo.contains(QRegularExpression("vo=direct.*")) && newDriver.contains(QRegularExpression("opengl.*")))){
       for(auto & it : *voSettings) {
           it->getCheckBox()->setChecked(false);
           it->toggleSetting(false);
       }
        vo="vo=";
    }

    // Checks if it still is direct3d or opengl, if so replace. If not append.
    if(vo.contains(QRegularExpression(QString("vo=").append("(opengl.*)|(direct3d.*)")))){
        ui->textEdit->setText(ui->textEdit->toPlainText().replace(QRegularExpression(QRegularExpression(QString("vo=").append(
                                    "((opengl-hq)|(opengl)|(direct3d_shaders)|(direct3d))"))), QString("vo=").append(newDriver)));
    }
    else{
        vo.append(newDriver);
        ui->textEdit->setText(vo);
    }
}

// File dialog for GLSL shader.
void tabVideo::browseForShader(){
    QString shader = QFileDialog::getOpenFileName(this,
             tr("Please choose a shader file."), QDir::homePath(), tr("GLSL files (*.glsl)"));
    ui->value_source_shader->setText(QString('"').append(shader).append('"'));
    parseVOSettings();
}

// File dialog for ICC profile.
void tabVideo::browseForICC(){
    QString shader = QFileDialog::getOpenFileName(this,
             tr("Please choose an ICC profile."), QDir::homePath(), tr("ICC files (*.icc)"));
    ui->value_icc_profile->setText(QString('"').append(shader).append('"'));
    parseVOSettings();
}

void tabVideo::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
        for(auto & it : *videoSettings){
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
                conf->remove(readSetting);                                      // To find unknown settings added by user
            }
        }

        // Reading VO settings.
        for(auto & it : *voSettings){
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
                conf->remove(readSetting);                                      // To find unknown settings added by user
            }
        }
    }
}

void tabVideo::readVOLine(QString readVO){
    readVO = readVO.replace(QRegularExpression("vo="), "");                     // Remove vo= because it makes it easier to read.
    if(readVO.startsWith("opengl-hq")){
        readVO = readVO.replace(QRegularExpression("opengl-hq"), "");
        ui->radio_preset_openglhq->setChecked(true);
    }
    else if(readVO.startsWith("direct3d-shaders")){
        readVO = readVO.replace(QRegularExpression("direct3d-shaders"), "");
        ui->radio_preset_direct3d_shaders->setChecked(true);
    }
    else if(readVO.startsWith("direct3d")){
        readVO = readVO.replace(QRegularExpression("direct3d"), "");
        ui->radio_preset_direct3d->setChecked(true);
    }
    else{
        readVO = readVO.replace(QRegularExpression("opengl"), "");
        ui->radio_preset_opengl->setChecked(true);
    }

    QMap<QString, QString> mappedVO;
    QStringList splittedVO = readVO.split(":");
    for(auto & voset : splittedVO){
        if(voset.contains(QRegularExpression("="))){
            QStringList furtherSplit = voset.split("=");
            mappedVO.insert(furtherSplit.first(), furtherSplit.back());
        }
        else
            mappedVO.insert(voset, "");
    }
    applyFromReadConf(&mappedVO);
    if(!mappedVO.isEmpty()){                                            // Add unknown custom settings added by user to VO.
        for(auto & readSetting : mappedVO.keys()) {
            if(mappedVO.value(readSetting).isEmpty())
                vo.append(readSetting);
            else
                vo.append(readSetting).append("=").append(mappedVO.value(readSetting));
        }
    }
    parseVOSettings();
}

tabVideo::~tabVideo() {
    delete voSettings;
    delete videoSettings;
    delete manualPages;
    delete ui;
}
