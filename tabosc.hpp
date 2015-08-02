#ifndef TABOSC_HPP
#define TABOSC_HPP

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class tabosc;
}

class tabosc : public QWidget
{
    Q_OBJECT

public:
    explicit tabosc(QWidget *parent = 0);
    QGraphicsScene *box_osc_scene, *bottombar_osc_scene, *topbar_osc_scene, *slimbox_osc_scene,
                    *seekbar_osc_bar_scene, *seekbar_osc_diamond_scene;
    void readFromConf(QString conf);
    ~tabosc();

public slots:
    void parseToConf(QStringList*);

private:
    Ui::tabosc *ui;
};

#endif // TABOSC_HPP
