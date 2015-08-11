#include "tabosc.hpp"
#include "ui_tabosc.h"

tabosc::tabosc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabosc)
{
    ui->setupUi(this);

    // Remove ugly border from the images.
    ui->graphicsview_box_osc->setStyleSheet("background: transparent; border: 0px;");
    ui->graphicsView_bottombar_osc->setStyleSheet("background: transparent; border: 0px;");
    ui->graphicsView_top_bar_osc->setStyleSheet("background: transparent; border: 0px;");
    ui->graphicsView_slim_box_osc->setStyleSheet("background: transparent; border: 0px;");
    ui->graphicsView_seekbar_bar->setStyleSheet("background: transparent; border: 0px;");
    ui->graphicsView_seekbar_diamond->setStyleSheet("background: transparent; border: 0px;");

    box_osc_scene = new QGraphicsScene();
    bottombar_osc_scene = new QGraphicsScene();
    topbar_osc_scene = new QGraphicsScene();
    slimbox_osc_scene = new QGraphicsScene();
    seekbar_osc_bar_scene = new QGraphicsScene();
    seekbar_osc_diamond_scene= new QGraphicsScene();

    box_osc_scene->addPixmap(QPixmap(":/images/oscs/box.png"));
    bottombar_osc_scene->addPixmap(QPixmap(":/images/oscs/bottombar.png"));
    topbar_osc_scene->addPixmap(QPixmap(":/images/oscs/topbar.png"));
    slimbox_osc_scene->addPixmap(QPixmap(":/images/oscs/slimbox.png"));
    seekbar_osc_bar_scene->addPixmap(QPixmap(":/images/oscs/seekbarstylebar.png"));
    seekbar_osc_diamond_scene->addPixmap(QPixmap(":/images/oscs/seekbarstylediamond.png"));

    // Add the OSC radio buttons.
    ui->graphicsview_box_osc->setScene(box_osc_scene);
    ui->graphicsView_bottombar_osc->setScene(bottombar_osc_scene);
    ui->graphicsView_top_bar_osc->setScene(topbar_osc_scene);
    ui->graphicsView_slim_box_osc->setScene(slimbox_osc_scene);
    ui->graphicsView_seekbar_bar->setScene(seekbar_osc_bar_scene);
    ui->graphicsView_seekbar_diamond->setScene(seekbar_osc_diamond_scene);
}

// Ugly parsed, should rewrite.
void tabosc::parseToConf(QStringList* conf){
    conf->append("\n# OSC settings");
    QString toBeAdded;
    if(ui->radio_no_osc->isChecked())
        toBeAdded.append("no-osc");
    else if(ui->radio_preset_2->isChecked())
        toBeAdded.append("script-opts=osc-layout=bottombar,");
    else if(ui->radio_preset_3->isChecked())
        toBeAdded.append("script-opts=osc-layout=topbar,");
    else if(ui->radio_preset_4->isChecked())
        toBeAdded.append("script-opts=osc-layout=slimbox,");
    else
        toBeAdded.append("script-opts=osc-layout=box,");

    if(!ui->radio_no_osc->isChecked() && ui->radio_seekbar_diamond->isChecked())
        toBeAdded.append("osc-seekbarstyle=slider");
    else if(!ui->radio_no_osc->isChecked())
        toBeAdded.append("osc-seekbarstyle=bar");

    conf->append(toBeAdded);
}

void tabosc::readFromConf(QString conf){
    if(conf.contains("no-osc")){
        ui->radio_no_osc->setChecked(true);
        return;
    }
    else if(conf.contains("bottombar"))
        ui->radio_preset_2->setChecked(true);
    else if(conf.contains("topbar"))
        ui->radio_preset_3->setChecked(true);
    else if(conf.contains("slimbox"))
        ui->radio_preset_4->setChecked(true);
    else
        ui->radio_preset_1->setChecked(true);

    if(conf.contains("seekbarstyle=slider"))
        ui->radio_seekbar_diamond->setChecked(true);
    else
        ui->radio_seekbar_bar->setChecked(true);
}

tabosc::~tabosc()
{
    delete box_osc_scene;
    delete bottombar_osc_scene;
    delete topbar_osc_scene;
    delete slimbox_osc_scene;
    delete seekbar_osc_bar_scene;
    delete seekbar_osc_diamond_scene;
    delete ui;

}
