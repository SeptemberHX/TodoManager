//
// Created by septemberhx on 7/27/18.
//

#include "ItemUtils.h"
#include <QUuid>

const QString todo::ItemUtils::PREFIX_ITEMDETAIL = "todo";
const QString todo::ItemUtils::PREFIX_ITEMGROUP = "group";

QString todo::ItemUtils::generateItemDetailUniqueID() {
    return QString("%1_%2").arg(ItemUtils::PREFIX_ITEMDETAIL).arg(QUuid::createUuid().toString().remove('{').remove('}'));
}

QString todo::ItemUtils::generateItemGroupUniqueID() {
    return QString("%1_%2").arg(ItemUtils::PREFIX_ITEMGROUP).arg(QUuid::createUuid().toString().remove('{').remove('}'));
}

bool todo::ItemUtils::checkIfItemDetail(const QString &itemID) {
    return itemID.startsWith(ItemUtils::PREFIX_ITEMDETAIL);
}

bool todo::ItemUtils::checkIfItemGroup(const QString &itemGroup) {
    return itemGroup.startsWith(ItemUtils::PREFIX_ITEMGROUP);
}

