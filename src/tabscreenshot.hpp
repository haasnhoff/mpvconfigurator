#ifndef TABSCREENSHOT_HPP
#define TABSCREENSHOT_HPP

#include <QWidget>
#include <vector>
#include <QFileDialog>
#include "setting.hpp"
#include "comboboxsetting.hpp"
#include "valueboxsetting.hpp"

namespace Ui {
class tabScreenshot;
}

class tabScreenshot : public QWidget
{
    Q_OBJECT

public:
    explicit tabScreenshot(QWidget *parent = 0);

    std::vector<setting*>* screenshotSettings;
    QStringList* manualPages;

    void addScreenshotSetting(QCheckBox* box);
    void addScreenshotSetting(QComboBox* comboBox, QCheckBox* box);
    void addScreenshotSetting(QLineEdit* valueBox, QCheckBox* box);
    QString convertTemplateToFormat();
    void applyFromReadConf(QMap<QString, QString>* conf);
    ~tabScreenshot();

private slots:
    void enablePNGandJPEGWidgets(QString);
    void enableDisableCustomTemplate(QString);
    void browseForScreenshotFolder();

public slots:
    void parseToConf(QStringList*);

private:
    Ui::tabScreenshot *ui;
};

#endif // TABSCREENSHOT_HPP
