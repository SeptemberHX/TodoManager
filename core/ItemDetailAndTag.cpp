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

const int &todo::ItemDetailAndTag::getOrder() const {
    return this->order;
}

todo::ItemDetailAndTag::ItemDetailAndTag(const QString &itemID, const QString &tagID, const int &orderNum)
        : itemID(itemID), tagID(tagID), order(orderNum) {}
