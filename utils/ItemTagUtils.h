//
// Created by septemberhx on 4/26/18.
//

#ifndef TODOMANAGER_ITEMTAGUTILS_H
#define TODOMANAGER_ITEMTAGUTILS_H

#include <QList>
#include "../core/ItemTag.h"

namespace todo {

    class ItemTagUtils {
    public:
        static ItemTag convertStr2Tag(const QString &tagStr);
        static QList<ItemTag> getAllTags();
        static void addNewTag(const ItemTag &tag);
        static void removeTag(const ItemTag &tag);
    };

}

#endif //TODOMANAGER_ITEMTAGUTILS_H
