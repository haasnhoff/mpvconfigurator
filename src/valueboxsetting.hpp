#ifndef VALUEBOXSETTING_HPP
#define VALUEBOXSETTING_HPP
#include <QLineEdit>
#include "setting.hpp"

class valueboxSetting : public setting {

    Q_OBJECT

public:
    QLineEdit* valueBox;
    valueboxSetting(QLineEdit* aValueBox, QCheckBox* checkBox, QStringList* manuals);

    void setValueBox(QLineEdit* aValueBox);
    QLineEdit* getValueBox();
    QString getValue();
    ~valueboxSetting();
};

#endif // VALUEBOXSETTING_HPP
