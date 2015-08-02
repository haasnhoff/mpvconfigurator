#include "tabconfig.hpp"
#include "ui_tabconfig.h"

tabconfig::tabconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabconfig)
{
    ui->setupUi(this);

    logoScene = new QGraphicsScene();
    ui->graphicsView->setScene(logoScene);
    ui->graphicsView->setStyleSheet("background: transparent; border: 0px;");
    logoScene->addPixmap(QPixmap(":/images/icon.png"));
}

tabconfig::~tabconfig()
{
    delete ui, logoScene;
}
