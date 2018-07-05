//
// Created by septemberhx on 7/5/18.
//

#include <QLocale>
#include <QCollator>
#include "StringUtils.h"

bool todo::StringUtils::compareString(const QString &str1, const QString &str2) {
    static QLocale loc(QLocale::Chinese, QLocale::China);
    static QCollator qco(loc);

    bool name1_startWithChinese = StringUtils::ifStringStartWithChinese(str1);
    bool name2_startWithChinese = StringUtils::ifStringStartWithChinese(str2);
    if (name1_startWithChinese == name2_startWithChinese) {
        return qco.compare(str1, str2) < 0;
    } else {
        return !name1_startWithChinese;
    }
}

bool todo::StringUtils::ifStringStartWithChinese(const QString &str) {
    if (str.isEmpty()) return false;

    ushort unicode = str.at(1).unicode();
    return (unicode >= 0x4E00 && unicode <= 0x9FA5);
}
