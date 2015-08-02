#ifndef UNKNOWNSETTINGSTAB_HPP
#define UNKNOWNSETTINGSTAB_HPP

#include <QWidget>

namespace Ui {
class unknownsettingstab;
}

class unknownsettingstab : public QWidget
{
    Q_OBJECT

public:
    explicit unknownsettingstab(QWidget *parent = 0);
    void addSetting(QString withSetting);
    void parseToConf(QStringList* conf);
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~unknownsettingstab();

private slots:
    void addSetting();
    void removeSetting();

private:
    Ui::unknownsettingstab *ui;
};

#endif // UNKNOWNSETTINGSTAB_HPP
