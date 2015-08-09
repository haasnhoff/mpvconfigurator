#include "tabextensions.hpp"
#include "ui_tabextensions.h"

tabExtensions::tabExtensions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabExtensions)
{
    ui->setupUi(this);

    settingMap = new QMap<QString, QPlainTextEdit*>;

    connect(ui->button_add_extension, SIGNAL(clicked()), this, SLOT(addNewExtensionTab()));
    connect(ui->button_remove_current, SIGNAL(clicked()), this, SLOT(removeExtensionTab()));
}

// Adds a new extension and creates a new tab for it.
void tabExtensions::addNewExtensionTab(){
    if(ui->valuebox_add_new_extension->text().startsWith(".")){
        QWidget* newTab = new QWidget();
        QGridLayout* grid = new QGridLayout();
        newTab->setLayout(grid);
        QLabel* nameLabel = new QLabel("Name:", newTab);
         grid->addWidget(nameLabel, 0, 0);
        QLabel* actualName = new QLabel(ui->valuebox_add_new_extension->text(), newTab);
         grid->addWidget(actualName, 0, 1);
        QPlainTextEdit* textEditor = new QPlainTextEdit(newTab);
        grid->addWidget(textEditor, 1, 0, 1, 0);
        ui->tabWidget->addTab(newTab, ui->valuebox_add_new_extension->text());
        settingMap->insert(actualName->text(), textEditor);
        ui->valuebox_add_new_extension->clear();
    }
    else{
        QMessageBox error;
        error.setText("ERROR: The extension needs to start with a dot.");
        error.exec();
    }
}

// Reads the extensions and inserts tab and text.
void tabExtensions::readExtensions(QString name, QStringList settings){
    QWidget* newTab = new QWidget();
    QPlainTextEdit* textEditor = new QPlainTextEdit(newTab);
    textEditor->setGeometry(80,90,500,260);
    textEditor->appendPlainText(settings.join("\n"));
    QLabel* nameLabel = new QLabel("Name:", newTab);
    nameLabel->setGeometry(80,30,51,16);
    QLabel* actualName = new QLabel(name, newTab);
    actualName->setGeometry(140,30,80,16);
    ui->tabWidget->addTab(newTab, name);
    settingMap->insert(actualName->text(), textEditor);
    ui->valuebox_add_new_extension->clear();

}

// Removes current visible tab from existence.
void tabExtensions::removeExtensionTab(){
    if(ui->tabWidget->currentIndex() > 0){
        QWidget* tabInQuestion = ui->tabWidget->currentWidget();
        settingMap->remove(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
        delete tabInQuestion;
    }
    else{
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    }
}

void tabExtensions::parseToConf(QStringList* conf){
    conf->append("\n# Extensions - Do not add custom settings that are not extension related below this line.");
    if(!settingMap->isEmpty()){
        for(auto & it : settingMap->keys()) {
            conf->append(QString("[extension").append(it).append("]\n").append(settingMap->value(it)->toPlainText()).append("\n"));
        }
    }
}

tabExtensions::~tabExtensions()
{
    delete settingMap;
    delete ui;
}
