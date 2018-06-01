//
// Created by septemberhx on 5/31/18.
//

#include "ItemDetailAndTag.h"

const QString &todo::ItemDetailAndTag::getItemID() const {
    return itemID;
}

const QString &todo::ItemDetailAndTag::getTagID() const {
    return tagID;
}

todo::ItemDetailAndTag::ItemDetailAndTag(const QString &itemID, const QString &tagID) : itemID(itemID), tagID(tagID) {}
