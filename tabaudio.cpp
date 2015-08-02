#include "tabaudio.hpp"
#include "ui_tabaudio.h"

tabaudio::tabaudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabaudio)
{
    ui->setupUi(this);
    ui->alsa_settings->hide();
    ui->coreaudio_settings->hide();
    ui->dsound_settings->hide();
    ui->pulseaudio_settings->hide();
    ui->jack_settings->hide();
    ui->oss_settings->hide();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");
    manualPages->append(":/man/af.rst");
    manualPages->append(":/man/ao.rst");
    audioSettings = new std::vector<setting*>();
    aoSettings = new std::vector<setting*>();

    // Connect all radio buttons to clear a VO setting if the driver is changed.
    connect(ui->radio_preset_alsa, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_auto, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_coreaudio, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_dsound, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_jack, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_oss, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_pulse, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));
    connect(ui->radio_preset_wasapi, SIGNAL(clicked(bool)), this, SLOT(changeDriver(bool)));

    // General settings.
    addAudioSetting(ui->comboBox_audio_pitch, ui->box_audio_pitch_correction);
    addAudioSetting(ui->value_audio_device, ui->box_audio_device);
    addAudioSetting(ui->value_volume, ui->box_volume);
    addAudioSetting(ui->value_audio_delay, ui->box_audio_delay);
    addAudioSetting(ui->comboBox_mute, ui->box_mute);
    addAudioSetting(ui->comboBox_softvol, ui->box_softvol);
    addAudioSetting(ui->box_no_softvol);
    addAudioSetting(ui->value_softvol_max, ui->box_softvol_max);
    addAudioSetting(ui->value_audio_demuxer, ui->box_audio_demuxer);
    addAudioSetting(ui->box_ad_lavc_downmix);
    addAudioSetting(ui->comboBox_lavc_threads, ui->box_ad_lavc_threads);
    addAudioSetting(ui->value_audio_channels, ui->box_audio_channels);
    addAudioSetting(ui->comboBox_audio_display, ui->box_audio_display);
    addAudioSetting(ui->comboBox_gapless_audio, ui->box_gapless_audio);
    addAudioSetting(ui->box_initial_audio_sync);
    addAudioSetting(ui->box_no_initial_audio_sync);
    addAudioSetting(ui->value_audio_buffer, ui->box_audio_buffer);
    addAudioSetting(ui->valuebox_alang, ui->box_alang);

    // alsa settings
    addAOSetting(ui->value_device, ui->box_device);
    addAOSetting(ui->comboBox_resample_alsa, ui->box_resample);
    addAOSetting(ui->value_mixer_device, ui->box_mixer_device);
    addAOSetting(ui->box_non_interleaved);

    // OSS settings
    addAOSetting(ui->value_dsp_device, ui->box_dsp_device);
    addAOSetting(ui->value_mixer_channel, ui->box_mixer_channel);

    // JACK settings
    addAOSetting(ui->value_jack_port, ui->box_jack_port);
    addAOSetting(ui->value_name_client, ui->box_name_client);
    addAOSetting(ui->box_autostart);
    addAOSetting(ui->box_connect);
    addAOSetting(ui->comboBox_std_channel_layout, ui->box_std_channel_layout);

    // CoreAudio settings
    addAOSetting(ui->comboBox_change_physical_format, ui->box_change_physical_format);
    addAOSetting(ui->box_exclusive);

    // PulseAudio settings
    addAOSetting(ui->value_pulse_buffer, ui->box_pulse_buffer);
    addAOSetting(ui->comboBox_latency_hacks, ui->box_latency_hacks);

    // DirectSound setings
    addAOSetting(ui->value_dsound_buffer, ui->box_dsound_buffer);
}

// Audio settings not in AO.
// These are not connected to parseAOSettings, which means they won't update the VO string.
void tabaudio::addAudioSetting(QCheckBox* box) {
    audioSettings->push_back(new setting(box, manualPages));
}

void tabaudio::addAudioSetting(QComboBox* comboBox, QCheckBox* box) {
    audioSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabaudio::addAudioSetting(QLineEdit* valueBox, QCheckBox* box) {
    audioSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

// AO Settings. These will connect to parseAOSetting to update the "AO String"
// box at the top of the widget.
void tabaudio::addAOSetting(QCheckBox* box) {
    aoSettings->push_back(new setting(box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseAOSettings()));
}

void tabaudio::addAOSetting(QComboBox* comboBox, QCheckBox* box) {
    aoSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseAOSettings()));
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(parseAOSettings(QString)));
}

void tabaudio::addAOSetting(QLineEdit* valueBox, QCheckBox* box) {
    aoSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
    connect(box, SIGNAL(clicked()), this, SLOT(parseAOSettings()));
    connect(valueBox, SIGNAL(editingFinished()), this, SLOT(parseAOSettings()));
}


// Parses all settings to a QStringList. Called from MainWindow.
void tabaudio::parseToConf(QStringList* conf){
    conf->append("\n# Audio settings");
    if(ao.compare("ao=") != 0)
        conf->append(ui->textEdit->toPlainText());
    for(auto & it : *audioSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else
                conf->append(QString(it->getName()).append("=").append(it->getValue()));
        }
    }
}

void tabaudio::parseAOSettings(){
    QStringList aoToBe;
    aoToBe.append("");                          // Empty insert to create a seperator between driver and first option if any option is chosen.
    for (auto & it : *aoSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                    aoToBe.append(it->getName());
            else
                aoToBe.append(it->getName().append("=").append(it->getValue()));
        }
    }
    if(!aoToBe.isEmpty())
        ui->textEdit->setText(QString(ao).append(aoToBe.join(":")));
    else
        ui->textEdit->setText(ao);
}

void tabaudio::parseAOSettings(QString neededToSolveSignalSlot){
    parseAOSettings();
}

// Could unfortunatley not use switch case here unless a vector or array of
// each radio button is created which would be more code than this.

void tabaudio::changeDriver(bool slotnecessity){
    QString newDriver;
    if(ui->radio_preset_alsa->isChecked())
        newDriver = "alsa";
    else if(ui->radio_preset_coreaudio->isChecked())
        newDriver = "coreaudio";
    else if(ui->radio_preset_dsound->isChecked())
        newDriver = "dsound";
    else if(ui->radio_preset_jack->isChecked())
        newDriver = "jack";
    else if(ui->radio_preset_oss->isChecked())
        newDriver = "oss";
    else if(ui->radio_preset_pulse->isChecked())
        newDriver = "pulse";
    else if(ui->radio_preset_wasapi->isChecked())
        newDriver = "wasapi";
    else
        newDriver = "";             // This is the auto driver. Which is literally nothing.

    // Clears the string and unchecks all AO boxes if you change any driver as they are not compatible.
    for(auto & it : *aoSettings) {
       it->getCheckBox()->setChecked(false);
       it->toggleSetting(false);
    }
    ao="ao=";
    ao.append(newDriver);                                                       // No point in replacing the ao= like done in vo because the setting are never compatible.

    ui->textEdit->setText(ao);
}

void tabaudio::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {                                    // For each setting in the config
        for(auto & it : *audioSettings){                                        // Search each setting available here
            if(readSetting.compare(it->getName()) == 0){                        // If checkbox's text and the setting name is identical.
                it->getCheckBox()->setChecked(true);                            // Check the checkbox.
                if(it->getType().compare("combobox") == 0){                     // If it is a combobox, set the text accordingly.
                    comboboxSetting* tempBox = (comboboxSetting*)it;
                    tempBox->getComboBox()->setCurrentText(conf->value(readSetting));
                }
                else if(it->getType().compare("valuebox") == 0){                // Or if QEditLine, set the text accordingly.
                    valueboxSetting* tempBox = (valueboxSetting*)it;
                    tempBox->getValueBox()->setText(conf->value(readSetting));
                }
                conf->remove(readSetting);                                      // To find unknown settings added by user
            }
        }

        // Reading AO settings.
        for(auto & it : *aoSettings){
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
                conf->remove(readSetting);
            }
        }
    }
}

void tabaudio::readAOLine(QString readAO){
    readAO = readAO.replace(QRegularExpression("ao="), "");

    // Why the fuck doesn't C++ have switch case with string? What is this, 1991? Holy shit this is ugly. Should rewrite.
    if(readAO.startsWith("alsa")){
        readAO = readAO.replace(QRegularExpression("alsa"), "");
        ui->radio_preset_alsa->setChecked(true);
    }
    else if(readAO.startsWith("coreaudio")){
        readAO = readAO.replace(QRegularExpression("coreaudio"), "");
        ui->radio_preset_coreaudio->setChecked(true);
    }
    else if(readAO.startsWith("dsound")){
        readAO = readAO.replace(QRegularExpression("dsound"), "");
        ui->radio_preset_dsound->setChecked(true);
    }
    else if(readAO.startsWith("jack")){
        readAO = readAO.replace(QRegularExpression("jack"), "");
        ui->radio_preset_jack->setChecked(true);
    }
    else if(readAO.startsWith("oss")){
        readAO = readAO.replace(QRegularExpression("oss"), "");
        ui->radio_preset_oss->setChecked(true);
    }
    else if(readAO.startsWith("pulse")){
        readAO = readAO.replace(QRegularExpression("pulse"), "");
        ui->radio_preset_pulse->setChecked(true);
    }
    else if(readAO.startsWith("wasapi")){
        readAO = readAO.replace(QRegularExpression("wasapi"), "");
        ui->radio_preset_wasapi->setChecked(true);
    }
    else
        ui->radio_preset_auto->setChecked(true);
    changeDriver(true);
    QMap<QString, QString> mappedAO;
    QStringList splittedAO = readAO.split(":");
    for(auto & aoset : splittedAO){
        if(aoset.contains(QRegularExpression("="))){
            QStringList furtherSplit = aoset.split("=");
            mappedAO.insert(furtherSplit.first(), furtherSplit.back());
        }
        else{
            mappedAO.insert(aoset, "");
        }
    }
    applyFromReadConf(&mappedAO);
    if(!mappedAO.isEmpty()){                                            // If there are unknown settings, add them last.
        for(auto & readSetting : mappedAO.keys()) {
            if(mappedAO.value(readSetting).isEmpty()){
                ao.append(readSetting);
            }
            else{
                ao.append(readSetting).append("=").append(mappedAO.value(readSetting));
            }
        }
    }
    parseAOSettings();
}

tabaudio::~tabaudio()
{
    delete audioSettings;
    delete aoSettings;
    delete manualPages;
    delete ui;
}
