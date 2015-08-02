#include "setting.hpp"

setting::setting(QCheckBox* checkbox, QStringList* manuals) :
                        name{checkbox->text()}, manual{manuals}, description{""}, settingStatus{false}, settingCheckBox{checkbox}, type{"checkbox"}, value{""} {
    connect(settingCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleSetting(bool)));
    description = parseSetting();
    settingCheckBox->setToolTip(description);
}

void setting::setName(QString name_) {
    name = name_;
}

void setting::setDescription(QString description_) {
    description = description_;
}

void setting::toggleSetting(bool toggledTo) {
    settingStatus=toggledTo;
}

void setting::setCheckBox(QCheckBox *box){
    settingCheckBox = box;
}

QString setting::getName() {
    return name;
}

QString setting::getDescription() {
    return description;
}

bool setting::getSettingStatus() {
    return settingStatus;
}

QCheckBox* setting::getCheckBox(){
    return settingCheckBox;
}

QString setting::getType(){
    return type;
}

QString setting::getValue(){
    return value;
}

// This method parses tooltips from .rst files located in ./man.
// accessibleName and accessibleDescription is used as a name/description override for hard-to-parse options.
// This is obviously not how accessible stuff should be used, but you know what? Fuck it.
QString setting::parseSetting(){
    QString regularExpress;
    if(!settingCheckBox->accessibleName().isEmpty())
        regularExpress.append("(``|``--)").append(settingCheckBox->accessibleName()).append(".*``\n");  // This is a override.
    else
        regularExpress.append("(``|``--)").append(name).append(".*``\n");
    QStringList listAuto;
    for(auto &i : *manual){
        QFile manualFile(i);
        if(manualFile.open(QFile::ReadOnly  |QFile::Text))
        {
            while(!manualFile.atEnd())
            {
                QString testString = manualFile.readLine();
                if(testString.contains(QRegularExpression(regularExpress))){
                    break;
                }
            }
        }
        QString currentLine = "";
        while(true){
            currentLine = manualFile.readLine();
            if(currentLine.contains(QRegularExpression(".*---.*"))){
                listAuto.removeLast();
                break;
            }
            else if(currentLine.contains("admonition::")){
                int indentedBy{0};
                QChar whitespace = currentLine.at(indentedBy);
                while(whitespace.isSpace()){
                    indentedBy++;
                    whitespace = currentLine.at(indentedBy);
                }
                QString neededIndentationToAdd = QString("^ {").append(QString::number(indentedBy)).append("}.*");
                while(currentLine.startsWith("\n") || currentLine.contains(QRegularExpression(neededIndentationToAdd))){
                    listAuto += currentLine;
                    currentLine = manualFile.readLine();
                }
                break;
            }
            else if(currentLine.contains(QRegularExpression("``.*``\n")))
                break;
            else if(manualFile.atEnd()){
                break;
            }
            listAuto += currentLine;
        }
        QString output;
        for(int i = 0; i < listAuto.length(); i++){
            output.append(listAuto[i]);
        }
        if(!output.isEmpty()){
            output = output.replace(QRegularExpression("  "), "");
            output = output.replace(QRegularExpression("``"), "'");
            output = output.replace(QRegularExpression(".. admonition:: "), "");
            output = output.replace(QRegularExpression(".. note::"), "Note:");
            output = output.replace(QRegularExpression(".. warning::"), "Warning:");
            return output.trimmed();
        }
    }
    if(!settingCheckBox->accessibleDescription().isEmpty()){
        return settingCheckBox->accessibleDescription();
    }
    qDebug() << "Failed to read manual description from - " << name;
    return "No tooltip available for this option. Please look it up in the manual.";
}

setting::~setting() {}

