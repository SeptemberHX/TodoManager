//
// Created by septemberhx on 7/17/18.
//

#include "ItemAndGroupPair.h"

const todo::ItemDetail &todo::ItemAndGroupPair::getItemDetail() const {
    return itemDetail;
}

void todo::ItemAndGroupPair::setItemDetail(const todo::ItemDetail &itemDetail) {
    ItemAndGroupPair::itemDetail = itemDetail;
    this->group = false;
}

const todo::ItemGroup &todo::ItemAndGroupPair::getItemGroup() const {
    return itemGroup;
}

void todo::ItemAndGroupPair::setItemGroup(const todo::ItemGroup &itemGroup) {
    ItemAndGroupPair::itemGroup = itemGroup;
    this->group = true;
}

bool todo::ItemAndGroupPair::isGroup() const {
    return group;
}

void todo::ItemAndGroupPair::setGroup(bool group) {
    ItemAndGroupPair::group = group;
}

std::ostream &todo::operator<<(std::ostream &os, const todo::ItemAndGroupPair &itemAndGroupPair) {
    return os;
}

QDataStream &todo::operator<<(QDataStream &os, const todo::ItemAndGroupPair &itemAndGroupPair) {
    return os;
}

QDataStream &todo::operator>>(QDataStream &os, todo::ItemAndGroupPair &itemAndGroupPair) {
    return os;
}

todo::ItemAndGroupPair::ItemAndGroupPair(const todo::ItemDetail &itemDetail) {
    this->setItemDetail(itemDetail);
}

todo::ItemAndGroupPair::ItemAndGroupPair(const todo::ItemGroup &itemGroup) {
    this->setItemGroup(itemGroup);
}

todo::ItemAndGroupPair::ItemAndGroupPair() {
    this->group = false;
}

const QString &todo::ItemAndGroupPair::getID() const {
    if (this->isGroup()) return this->itemGroup.getId();
    else return this->itemDetail.getId();
}
