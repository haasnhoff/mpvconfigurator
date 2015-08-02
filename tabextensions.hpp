#ifndef TABEXTENSIONS_HPP
#define TABEXTENSIONS_HPP

#include <QWidget>
#include <QPlainTextEdit>
#include <QMessageBox>

namespace Ui {
class tabExtensions;
}

class tabExtensions : public QWidget
{
    Q_OBJECT

public:
    explicit tabExtensions(QWidget *parent = 0);
    QMap<QString, QPlainTextEdit*>* settingMap;

    void parseToConf(QStringList* conf);
    void readExtensions(QString name, QStringList settings);
    ~tabExtensions();

private slots:
    void addNewExtensionTab();
    void removeExtensionTab();

private:
    Ui::tabExtensions *ui;
};

#endif // TABEXTENSIONS_HPP
