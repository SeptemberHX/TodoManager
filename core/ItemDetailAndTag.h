//
// Created by septemberhx on 5/31/18.
//

#ifndef TODOMANAGER_ITEMDETAILANDTAG_H
#define TODOMANAGER_ITEMDETAILANDTAG_H

#include <QString>

namespace todo {
class ItemDetailAndTag {
public:
    ItemDetailAndTag(const QString &itemID, const QString &tagID);

    const QString &getItemID() const;

    const QString &getTagID() const;

private:
    QString itemID;
    QString tagID;
};
}

#endif //TODOMANAGER_ITEMDETAILANDTAG_H
