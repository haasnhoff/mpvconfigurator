#ifndef TABWINDOW_HPP
#define TABWINDOW_HPP

#include <QWidget>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"

namespace Ui {
class tabWindow;
}

class tabWindow : public QWidget
{
    Q_OBJECT

public:
    explicit tabWindow(QWidget *parent = 0);
    std::vector<setting*>* windowSettings;
    QStringList* manualPages;

    void addWindowSetting(QCheckBox* box);
    void addWindowSetting(QComboBox* comboBox, QCheckBox* box);
    void addWindowSetting(QLineEdit* valueBox, QCheckBox* box);
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~tabWindow();

public slots:
    void parseToConf(QStringList* conf);

private:
    Ui::tabWindow *ui;
};

#endif // TABWINDOW_HPP
