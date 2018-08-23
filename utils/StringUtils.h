//
// Created by septemberhx on 7/5/18.
//

#ifndef TODOMANAGER_STRINGUTILS_H
#define TODOMANAGER_STRINGUTILS_H


#include <QString>
#include <QFontMetrics>

namespace todo {

class StringUtils {
public:
    static const QString PREFIX_ITEMDETAIL;
    static const QString PREFIX_ITEMGROUP;
    static const QString PREFIX_STICKYNOTE;

    static QString generateItemDetailUniqueID();
    static QString generateItemGroupUniqueID();
    static QString generateStickyNoteUniqueID();
    static QString generateUniqueID(const QString &prefix);

    static bool checkIfItemDetail(const QString &itemID);
    static bool checkIfItemGroup(const QString &itemGroup);
    static bool compareString(const QString &str1, const QString &str2);
    static bool ifStringStartWithChinese(const QString &str);
    static QString elideText(const QString &str, const QFontMetrics &fontMetrics, int maxWidth);

    static QString getDateTimeFormat();
};

}


#endif //TODOMANAGER_STRINGUTILS_H
