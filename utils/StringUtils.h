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
    static bool compareString(const QString &str1, const QString &str2);
    static bool ifStringStartWithChinese(const QString &str);
    static QString elideText(const QString &str, const QFontMetrics &fontMetrics, int maxWidth);
};

}


#endif //TODOMANAGER_STRINGUTILS_H
