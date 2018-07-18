//
// Created by septemberhx on 7/16/18.
//

#include "ItemGroup.h"

const QString &todo::ItemGroup::getTitle() const {
    return title;
}

void todo::ItemGroup::setTitle(const QString &title) {
    ItemGroup::title = title;
}

const QString &todo::ItemGroup::getId() const {
    return id;
}

void todo::ItemGroup::setId(const QString &id) {
    ItemGroup::id = id;
}

const QString &todo::ItemGroup::getDescription() const {
    return description;
}

void todo::ItemGroup::setDescription(const QString &description) {
    ItemGroup::description = description;
}

todo::ItemGroupType todo::ItemGroup::getType() const {
    return type;
}

void todo::ItemGroup::setType(todo::ItemGroupType type) {
    ItemGroup::type = type;
}

bool todo::ItemGroup::isMileStone() const {
    return mileStone;
}

void todo::ItemGroup::setMileStone(bool mileStone) {
    ItemGroup::mileStone = mileStone;
}

const QDateTime &todo::ItemGroup::getCreatedTime() const {
    return createdTime;
}

void todo::ItemGroup::setCreatedTime(const QDateTime &createdTime) {
    ItemGroup::createdTime = createdTime;
}

const QDateTime &todo::ItemGroup::getLastUpdatedTime() const {
    return lastUpdatedTime;
}

void todo::ItemGroup::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    ItemGroup::lastUpdatedTime = lastUpdatedTime;
}

const QList<todo::ItemGroup> &todo::ItemGroup::getSubGroupList() const {
    return subGroupList;
}

void todo::ItemGroup::setSubGroupList(const QList<todo::ItemGroup> &subGroupList) {
    ItemGroup::subGroupList = subGroupList;
}

const QList<todo::ItemDetail> &todo::ItemGroup::getItemDetailList() const {
    return itemDetailList;
}

void todo::ItemGroup::setItemDetailList(const QList<todo::ItemDetail> &itemDetailList) {
    ItemGroup::itemDetailList = itemDetailList;
}

todo::ItemGroup::ItemGroup() {
    this->id = QString("group_") + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    this->createdTime = QDateTime::currentDateTime();
    this->lastUpdatedTime = this->createdTime;
}

void todo::ItemGroup::addItemDetail(const todo::ItemDetail &itemDetail) {
    this->itemDetailList.append(itemDetail);
}

void todo::ItemGroup::addItemGroup(const todo::ItemGroup &itemGroup) {
    this->subGroupList.append(itemGroup);
}

const QDate &todo::ItemGroup::getFromDate() const {
    return fromDate;
}

void todo::ItemGroup::setFromDate(const QDate &fromDate) {
    ItemGroup::fromDate = fromDate;
}

const QDate &todo::ItemGroup::getToDate() const {
    return toDate;
}

void todo::ItemGroup::setToDate(const QDate &toDate) {
    ItemGroup::toDate = toDate;
}
