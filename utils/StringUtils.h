//
// Created by septemberhx on 7/5/18.
//

#ifndef TODOMANAGER_STRINGUTILS_H
#define TODOMANAGER_STRINGUTILS_H


#include <QString>

namespace todo {

class StringUtils {
public:
    static bool compareString(const QString &str1, const QString &str2);
    static bool ifStringStartWithChinese(const QString &str);
};

}


#endif //TODOMANAGER_STRINGUTILS_H
