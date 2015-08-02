#ifndef TABCONFIG_HPP
#define TABCONFIG_HPP

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class tabconfig;
}

class tabconfig : public QWidget
{
    Q_OBJECT

public:
    explicit tabconfig(QWidget *parent = 0);
    QGraphicsScene* logoScene;
    ~tabconfig();

private:
    Ui::tabconfig *ui;
};

#endif // TABCONFIG_HPP
