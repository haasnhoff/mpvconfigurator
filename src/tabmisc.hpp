#ifndef TABMISC_HPP
#define TABMISC_HPP

#include <QWidget>
#include <QFileDialog>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"

namespace Ui {
class tabMisc;
}

class tabMisc : public QWidget
{
    Q_OBJECT

public:
    explicit tabMisc(QWidget *parent = 0);

    std::vector<setting*>* miscSettings;
    QStringList* manualPages;

    void addMiscSetting(QCheckBox* box);
    void addMiscSetting(QComboBox* comboBox, QCheckBox* box);
    void addMiscSetting(QLineEdit* valueBox, QCheckBox* box);
    void parseToConf(QStringList* conf);
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~tabMisc();

private slots:
    void openCDROMFolder();

private:
    Ui::tabMisc *ui;
};

#endif // TABMISC_HPP
