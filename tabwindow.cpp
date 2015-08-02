#include "tabwindow.hpp"
#include "ui_tabwindow.h"

tabWindow::tabWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabWindow)
{
    ui->setupUi(this);

    windowSettings = new std::vector<setting*>();
    manualPages = new QStringList();
    manualPages->append(":/man/options.rst");

    addWindowSetting(ui->comboBox_screen_default, ui->box_screen_default);
    addWindowSetting(ui->box_fullscreen);
    addWindowSetting(ui->comboBox_keep_open, ui->box_keep_open);
    addWindowSetting(ui->comboBox_force_window, ui->box_force_window);
    addWindowSetting(ui->box_ontop);
    addWindowSetting(ui->box_no_border);
    addWindowSetting(ui->box_on_all_workspaces);
    addWindowSetting(ui->valuebox_autofit, ui->box_autofit);
    addWindowSetting(ui->valuebox_autofit_larger, ui->box_autofit_larger);
    addWindowSetting(ui->valuebox_autofit_smaller, ui->box_autofit_smaller);
    addWindowSetting(ui->comboBox_cursor_autohide, ui->box_cursor_autohide);
    addWindowSetting(ui->box_cursor_autohide_fs_only);
    addWindowSetting(ui->box_no_fixed_vo);
    addWindowSetting(ui->box_no_keepaspect);
    addWindowSetting(ui->box_no_keepaspect_window);
    addWindowSetting(ui->valuebox_monitoraspect, ui->box_monitoraspect);
    addWindowSetting(ui->box_no_stop_screensaver);
    addWindowSetting(ui->box_no_window_dragging);
    addWindowSetting(ui->valuebox_x11_name, ui->box_x11_name);
    addWindowSetting(ui->comboBox_x11_netwm, ui->box_x11_netwm);

}

void tabWindow::addWindowSetting(QCheckBox* box) {
    windowSettings->push_back(new setting(box, manualPages));
}

void tabWindow::addWindowSetting(QComboBox* comboBox, QCheckBox* box) {
    windowSettings->push_back(new comboboxSetting(comboBox, box, manualPages));
}

void tabWindow::addWindowSetting(QLineEdit* valueBox, QCheckBox* box) {
    windowSettings->push_back(new valueboxSetting(valueBox, box, manualPages));
}

void tabWindow::parseToConf(QStringList* conf){
    conf->append("\n# Window settings");
    for(auto & it : *windowSettings) {
        if(it->getSettingStatus()){
            if(it->getType().compare("checkbox") == 0)
                conf->append(it->getName());
            else
                conf->append(QString(it->getName()).append("=").append(it->getValue()));
        }
    }
}

void tabWindow::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
        for(auto & it : *windowSettings){
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

tabWindow::~tabWindow()
{
    delete ui, windowSettings, manualPages;
}
