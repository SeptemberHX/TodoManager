//
// Created by septemberhx on 7/17/18.
//

#include "ItemGroupRelation.h"

const QString &todo::ItemGroupRelation::getDirectGroupID() const {
    return directGroupID;
}

void todo::ItemGroupRelation::setDirectGroupID(const QString &directGroupID) {
    ItemGroupRelation::directGroupID = directGroupID;
}

const QString &todo::ItemGroupRelation::getRootGroupID() const {
    return rootGroupID;
}

void todo::ItemGroupRelation::setRootGroupID(const QString &rootGroupID) {
    ItemGroupRelation::rootGroupID = rootGroupID;
}

const QString &todo::ItemGroupRelation::getItemID() const {
    return itemID;
}

void todo::ItemGroupRelation::setItemID(const QString &itemID) {
    ItemGroupRelation::itemID = itemID;
}
