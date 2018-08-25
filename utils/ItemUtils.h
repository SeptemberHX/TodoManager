//
// Created by septemberhx on 7/27/18.
//

#ifndef TODOMANAGER_ITEMUTILS_H
#define TODOMANAGER_ITEMUTILS_H

#include <QString>
#include "../core/ItemDetail.h"
#include "../core/ItemAndGroupWrapper.h"
#include "../data/DataCenter.h"

namespace todo {

class ItemUtils {
public:

    static QColor getRootGroupColor(const ItemAndGroupWrapper &wrapper);

    static ItemDetail generateNewItemDetail(const QString &title,
            const QString &rootItemID="", const QString &directItemID="");

    static QString getPlainDescription(const ItemAndGroupWrapper &wrapper);
    static QString generateToolTip(const ItemAndGroupWrapper & wrapper);

private:
    static DataCenter dataCenter;
};

}


#endif //TODOMANAGER_ITEMUTILS_H
