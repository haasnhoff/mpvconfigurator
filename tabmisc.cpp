#include "tabmisc.hpp"
#include "ui_tabmisc.h"

tabMisc::tabMisc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabMisc)
{
    ui->setupUi(this);
    miscSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");

    addMiscSetting(ui->comboBox_ytdl_format, ui->box_ytdl_format);
    addMiscSetting(ui->box_save_position_on_quit);
    addMiscSetting(ui->comboBox_priority, ui->box_priority);
    addMiscSetting(ui->comboBox_force_seekable, ui->box_force_seekable);
    addMiscSetting(ui->valuebox_cdrom_device, ui->box_cdrom_device);
    addMiscSetting(ui->valuebox_heartbeat_cmd, ui->box_heartbeat_cmd);
    addMiscSetting(ui->valuebox_heartbeat_interval, ui->box_heartbeat_interval);
    addMiscSetting(ui->box_no_cache);
    addMiscSetting(ui->comboBox_cache, ui->box_cache);
    addMiscSetting(ui->comboBox_cache_default, ui->box_cache_default);
    addMiscSetting(ui->valuebox_cache_initial, ui->box_cache_initial);
    addMiscSetting(ui->valuebox_cache_secs, ui->box_cache_secs);
    addMiscSetting(ui->valuebox_cache_seek_min, ui->box_cache_seek_min);
    addMiscSetting(ui->box_no_cache_pause);
    addMiscSetting(ui->valuebox_network_timeout, ui->box_network_timeout);
    addMiscSetting(ui->comboBox_hls_bitrate, ui->box_hls_bitrate);

    connect(ui->button_browse_cdrom, SIGNAL(clicked()), this, SLOT(openCDROMFolder()));

}

void tabMisc::addMiscSetting(QCheckBox* box) {
    miscSettings->push_back(new setting(box, manualPages));
}

void tabMisc::addMiscSetting(QComboBox* comboBox, QCheckBox* box) {
    miscSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabMisc::addMiscSetting(QLineEdit* valueBox, QCheckBox* box) {
    miscSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

void tabMisc::parseToConf(QStringList* conf){
    conf->append("\n# Miscellaneous settings");
    for(auto & it : *miscSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else
                conf->append(QString(it->getName()).append("=").append(it->getValue()));
        }
    };
}

// Opens a file dialog for the user to choose a CD-ROM drive.
void tabMisc::openCDROMFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this,
             tr("Open CD-ROM folder..."), QDir::homePath());
    ui->valuebox_cdrom_device->setText(QString('"').append(folderName).append('"'));
}

void tabMisc::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
        for(auto & it : *miscSettings){
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

tabMisc::~tabMisc()
{
    delete ui, miscSettings, manualPages;
}
