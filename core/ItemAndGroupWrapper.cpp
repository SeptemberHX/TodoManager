//
// Created by septemberhx on 7/17/18.
//

#include "ItemAndGroupWrapper.h"

const todo::ItemDetail &todo::ItemAndGroupWrapper::getItemDetail() const {
    return itemDetail;
}

void todo::ItemAndGroupWrapper::setItemDetail(const todo::ItemDetail &itemDetail) {
    ItemAndGroupWrapper::itemDetail = itemDetail;
    this->group = false;
}

const todo::ItemGroup &todo::ItemAndGroupWrapper::getItemGroup() const {
    return itemGroup;
}

void todo::ItemAndGroupWrapper::setItemGroup(const todo::ItemGroup &itemGroup) {
    ItemAndGroupWrapper::itemGroup = itemGroup;
    this->group = true;
}

bool todo::ItemAndGroupWrapper::isGroup() const {
    return group;
}

std::ostream &todo::operator<<(std::ostream &os, const todo::ItemAndGroupWrapper &itemAndGroupPair) {
    return os;
}

QDataStream &todo::operator<<(QDataStream &os, const todo::ItemAndGroupWrapper &itemAndGroupPair) {
    return os;
}

QDataStream &todo::operator>>(QDataStream &os, todo::ItemAndGroupWrapper &itemAndGroupPair) {
    return os;
}

todo::ItemAndGroupWrapper::ItemAndGroupWrapper(const todo::ItemDetail &itemDetail) {
    this->setItemDetail(itemDetail);
}

todo::ItemAndGroupWrapper::ItemAndGroupWrapper(const todo::ItemGroup &itemGroup) {
    this->setItemGroup(itemGroup);
}

todo::ItemAndGroupWrapper::ItemAndGroupWrapper() {
    this->group = false;
}

const QString &todo::ItemAndGroupWrapper::getID() const {
    if (this->isGroup()) return this->itemGroup.getId();
    else return this->itemDetail.getId();
}

bool todo::ItemAndGroupWrapper::isDetail() const {
    return !this->isGroup();
}

QString todo::ItemAndGroupWrapper::getRootGroupID() const {
    if (this->isGroup()) return this->itemGroup.getRootGroupID();
    else return this->itemDetail.getRootGroupID();
}

bool todo::ItemAndGroupWrapper::hasRootGroup() const {
    if (this->isGroup()) return this->itemGroup.hasRootGroup();
    else return this->itemDetail.hasRootGroup();
}

QString todo::ItemAndGroupWrapper::getDirectGroupID() const {
    if (this->isGroup()) return this->itemGroup.getDirectGroupID();
    else return this->itemDetail.getDirectGroupID();}
