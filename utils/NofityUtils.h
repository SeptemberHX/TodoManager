//
// Created by septemberhx on 6/6/18.
//

#ifndef TODOMANAGER_NOFITYUTILS_H
#define TODOMANAGER_NOFITYUTILS_H


#include <QString>

namespace todo {

class NofityUtils {
public:
    static void push(const QString &title, const QString &content);
};

}

#endif //TODOMANAGER_NOFITYUTILS_H
