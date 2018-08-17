//
// Created by septemberhx on 7/5/18.
//

#include <QLocale>
#include <QCollator>
#include <QUuid>
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

QString todo::StringUtils::elideText(const QString &str, const QFontMetrics &fontMetrics, int maxWidth) {
    QString newStr(str);
    newStr.replace('\n', ' ');

    if (fontMetrics.width(newStr) > maxWidth) {
        return fontMetrics.elidedText(newStr, Qt::ElideRight, maxWidth);
    }
    return newStr;
}

const QString todo::StringUtils::PREFIX_ITEMDETAIL = "todo";
const QString todo::StringUtils::PREFIX_ITEMGROUP = "group";
const QString todo::StringUtils::PREFIX_STICKYNOTE = "stickynote";

QString todo::StringUtils::generateItemDetailUniqueID() {
    return StringUtils::generateUniqueID(StringUtils::PREFIX_ITEMDETAIL);
}

QString todo::StringUtils::generateItemGroupUniqueID() {
    return StringUtils::generateUniqueID(StringUtils::PREFIX_ITEMGROUP);
}

bool todo::StringUtils::checkIfItemDetail(const QString &itemID) {
    return itemID.startsWith(StringUtils::PREFIX_ITEMDETAIL);
}

bool todo::StringUtils::checkIfItemGroup(const QString &itemGroup) {
    return itemGroup.startsWith(StringUtils::PREFIX_ITEMGROUP);
}

QString todo::StringUtils::generateUniqueID(const QString &prefix) {
    return QString("%1_%2").arg(prefix).arg(QUuid::createUuid().toString().remove('{').remove('}'));
}

QString todo::StringUtils::generateStickyNoteUniqueID() {
    return StringUtils::generateUniqueID(StringUtils::PREFIX_STICKYNOTE);
}
