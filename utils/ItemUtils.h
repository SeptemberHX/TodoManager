//
// Created by septemberhx on 7/27/18.
//

#ifndef TODOMANAGER_ITEMUTILS_H
#define TODOMANAGER_ITEMUTILS_H

#include <QString>

namespace todo {

class ItemUtils {
public:
    static const QString PREFIX_ITEMDETAIL;
    static const QString PREFIX_ITEMGROUP;

    static QString generateItemDetailUniqueID();
    static QString generateItemGroupUniqueID();

    static bool checkIfItemDetail(const QString &itemID);
    static bool checkIfItemGroup(const QString &itemGroup);
};

}


#endif //TODOMANAGER_ITEMUTILS_H
