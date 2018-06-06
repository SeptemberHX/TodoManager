//
// Created by septemberhx on 6/6/18.
//

#include "NofityUtils.h"

void todo::NofityUtils::push(const QString &title, const QString &content) {

#ifdef Q_OS_LINUX
    QString commandStr("kdialog --title '%1' --passivepopup '%2'");
    QString commandStr2("notify-send '%1' '%2'");
    if (!system(commandStr.arg(title).arg(content).toStdString().c_str())) {
        system(commandStr2.arg(title).arg(content).toStdString().c_str());
    }
#endif

}
