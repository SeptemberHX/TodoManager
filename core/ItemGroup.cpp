//
// Created by septemberhx on 7/16/18.
//

#include "ItemGroup.h"
#include "../utils/StringUtils.h"

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

todo::ItemGroup::ItemGroup() {
    this->itemGroupDao.setId(todo::StringUtils::generateItemGroupUniqueID());
    this->itemGroupDao.setCreatedTime(QDateTime::currentDateTime());
    this->itemGroupDao.setLastUpdatedTime(this->getCreatedTime());
    this->itemGroupDao.setFromDate(QDate::currentDate());
    this->itemGroupDao.setToDate(QDate::currentDate());
    this->itemGroupDao.setMileStone(false);
    this->itemGroupDao.setType(todo::ItemGroupType::SUB_PROJECT);
    this->itemGroupDao.setColor(Qt::white);
}

void todo::ItemGroup::addItemDetail(const QString &itemDetailID) {
    this->itemDetailIDList.append(itemDetailID);
}

void todo::ItemGroup::addItemGroup(const QString &itemGroupID) {
    this->subGroupIDList.append(itemGroupID);
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

bool todo::ItemGroup::hasRootGroup() const {
    return StringUtils::checkIfItemGroup(this->rootGroupID);
}

const QString &todo::ItemGroup::getDirectGroupID() const {
    return directGroupID;
}

void todo::ItemGroup::setDirectGroupID(const QString &directGroupID) {
    ItemGroup::directGroupID = directGroupID;
}

const QString &todo::ItemGroup::getRootGroupID() const {
    return rootGroupID;
}

void todo::ItemGroup::setRootGroupID(const QString &rootGroupID) {
    ItemGroup::rootGroupID = rootGroupID;
}

const QList<QString> &todo::ItemGroup::getSubGroupIDList() const {
    return subGroupIDList;
}

void todo::ItemGroup::setSubGroupIDList(const QList<QString> &subGroupIDList) {
    ItemGroup::subGroupIDList = subGroupIDList;
}

const QList<QString> &todo::ItemGroup::getItemDetailIDList() const {
    return itemDetailIDList;
}

void todo::ItemGroup::setItemDetailIDList(const QList<QString> &itemDetailIDList) {
    ItemGroup::itemDetailIDList = itemDetailIDList;
}
