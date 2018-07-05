//
// Created by septemberhx on 7/5/18.
//

#include <QLocale>
#include <QCollator>
#include "StringUtils.h"

bool todo::StringUtils::compareString(const QString &str1, const QString &str2) {
    static QLocale loc(QLocale::Chinese, QLocale::China);
    static QCollator qco(loc);

    ushort uni1 = str1.at(1).unicode();
    ushort uni2 = str2.at(1).unicode();
    bool name1_startWithChinese = false;
    bool name2_startWithChinese = false;
    if (uni1 >= 0x4E00 && uni1 <= 0x9FA5) {
        name1_startWithChinese = true;
    }

    if (uni2 >= 0x4E00 && uni2 <= 0x9FA5) {
        name2_startWithChinese = true;
    }

    if (name1_startWithChinese == name2_startWithChinese) {
        return qco.compare(str1, str2) < 0;
    } else {
        return !name1_startWithChinese;
    }
}
