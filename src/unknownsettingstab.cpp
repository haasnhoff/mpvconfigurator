#include "unknownsettingstab.hpp"
#include "ui_unknownsettingstab.h"

unknownsettingstab::unknownsettingstab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::unknownsettingstab)
{
    ui->setupUi(this);
    connect(ui->button_add_setting, SIGNAL(clicked()), this, SLOT(addSetting()));
    connect(ui->button_remove_setting, SIGNAL(clicked()), this, SLOT(removeSetting()));
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), ui->listWidget, SLOT(edit(QModelIndex)));

}

// adds a setting to the list.
void unknownsettingstab::addSetting(){
    QListWidgetItem* item = new QListWidgetItem("Custom Setting");
    item->setFlags(item->flags() | Qt::ItemIsEditable);                 // Makes it possible to doubleclick to edit.
    ui->listWidget->addItem(item);
}

// Adds an already read setting.
void unknownsettingstab::addSetting(QString withSetting){
    QListWidgetItem* item = new QListWidgetItem(withSetting);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(item);
}

// Removes it.
void unknownsettingstab::removeSetting(){
    delete ui->listWidget->currentItem();
}

void unknownsettingstab::parseToConf(QStringList* conf){
    conf->append("\n# Unknown and custom settings");
    for(int i = 0; i < ui->listWidget->count(); i++){
        conf->append(ui->listWidget->item(i)->text());
    }
}

void unknownsettingstab::applyFromReadConf(QMap<QString, QString>* conf){
    for(auto & readSetting : conf->keys()) {
        addSetting(readSetting.append(conf->value(readSetting)));
        conf->remove(readSetting);
    }
}

unknownsettingstab::~unknownsettingstab()
{
    delete ui;
}
