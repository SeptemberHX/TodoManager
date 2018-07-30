//
// Created by septemberhx on 7/16/18.
//

#include "ItemGroup.h"
#include "../utils/ItemUtils.h"

const QString &todo::ItemGroup::getTitle() const {
    return this->itemGroupDao.getTitle();
}

void todo::ItemGroup::setTitle(const QString &title) {
    this->itemGroupDao.setTitle(title);
}

const QString &todo::ItemGroup::getId() const {
    return this->itemGroupDao.getId();
}

void todo::ItemGroup::setId(const QString &id) {
    this->itemGroupDao.setId(id);
}

const QString &todo::ItemGroup::getDescription() const {
    return this->itemGroupDao.getDescription();
}

void todo::ItemGroup::setDescription(const QString &description) {
    this->itemGroupDao.setDescription(description);
}

todo::ItemGroupType todo::ItemGroup::getType() const {
    return this->itemGroupDao.getType();
}

void todo::ItemGroup::setType(todo::ItemGroupType type) {
    this->itemGroupDao.setType(type);
}

bool todo::ItemGroup::isMileStone() const {
    return this->itemGroupDao.isMileStone();
}

void todo::ItemGroup::setMileStone(bool mileStone) {
    this->itemGroupDao.setMileStone(mileStone);
}

const QDateTime &todo::ItemGroup::getCreatedTime() const {
    return this->itemGroupDao.getCreatedTime();
}

void todo::ItemGroup::setCreatedTime(const QDateTime &createdTime) {
    this->itemGroupDao.setCreatedTime(createdTime);
}

const QDateTime &todo::ItemGroup::getLastUpdatedTime() const {
    return this->itemGroupDao.getLastUpdatedTime();
}

void todo::ItemGroup::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    this->itemGroupDao.setLastUpdatedTime(lastUpdatedTime);
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
    this->itemGroupDao.setId(todo::ItemUtils::generateItemGroupUniqueID());
    this->itemGroupDao.setCreatedTime(QDateTime::currentDateTime());
    this->itemGroupDao.setLastUpdatedTime(this->getCreatedTime());
    this->itemGroupDao.setFromDate(QDate::currentDate());
    this->itemGroupDao.setToDate(QDate::currentDate());
    this->itemGroupDao.setMileStone(false);
    this->itemGroupDao.setType(todo::ItemGroupType::SUB_PROJECT);
}

void todo::ItemGroup::addItemDetail(const todo::ItemDetail &itemDetail) {
    this->itemDetailList.append(itemDetail);
}

void todo::ItemGroup::addItemGroup(const todo::ItemGroup &itemGroup) {
    this->subGroupList.append(itemGroup);
}

const QDate &todo::ItemGroup::getFromDate() const {
    return this->itemGroupDao.getFromDate();
}

void todo::ItemGroup::setFromDate(const QDate &fromDate) {
    this->itemGroupDao.setFromDate(fromDate);
}

const QDate &todo::ItemGroup::getToDate() const {
    return this->itemGroupDao.getToDate();
}

void todo::ItemGroup::setToDate(const QDate &toDate) {
    this->itemGroupDao.setToDate(toDate);
}

bool todo::ItemGroup::operator==(const todo::ItemGroup &otherGroup) const {
    return this->getId() == otherGroup.getId();
}

todo::ItemGroup::ItemGroup(const todo::ItemGroupDao &itemGroupDao) {
    this->itemGroupDao = itemGroupDao;
}

todo::ItemGroupDao todo::ItemGroup::toDao() const {
    return this->itemGroupDao;
}

const QColor &todo::ItemGroup::getColor() const {
    return this->itemGroupDao.getColor();
}

void todo::ItemGroup::setColor(const QColor &color) {
    this->itemGroupDao.setColor(color);
}
