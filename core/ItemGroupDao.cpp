//
// Created by septemberhx on 7/29/18.
//

#include "ItemGroupDao.h"

const QString &todo::ItemGroupDao::getTitle() const {
    return title;
}

void todo::ItemGroupDao::setTitle(const QString &title) {
    ItemGroupDao::title = title;
}

const QString &todo::ItemGroupDao::getId() const {
    return id;
}

void todo::ItemGroupDao::setId(const QString &id) {
    ItemGroupDao::id = id;
}

const QString &todo::ItemGroupDao::getDescription() const {
    return description;
}

void todo::ItemGroupDao::setDescription(const QString &description) {
    ItemGroupDao::description = description;
}

todo::ItemGroupType todo::ItemGroupDao::getType() const {
    return type;
}

void todo::ItemGroupDao::setType(todo::ItemGroupType type) {
    ItemGroupDao::type = type;
}

bool todo::ItemGroupDao::isMileStone() const {
    return mileStone;
}

void todo::ItemGroupDao::setMileStone(bool mileStone) {
    ItemGroupDao::mileStone = mileStone;
}

const QDateTime &todo::ItemGroupDao::getCreatedTime() const {
    return createdTime;
}

void todo::ItemGroupDao::setCreatedTime(const QDateTime &createdTime) {
    ItemGroupDao::createdTime = createdTime;
}

const QDateTime &todo::ItemGroupDao::getLastUpdatedTime() const {
    return lastUpdatedTime;
}

void todo::ItemGroupDao::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    ItemGroupDao::lastUpdatedTime = lastUpdatedTime;
}

const QDate &todo::ItemGroupDao::getFromDate() const {
    return fromDate;
}

void todo::ItemGroupDao::setFromDate(const QDate &fromDate) {
    ItemGroupDao::fromDate = fromDate;
}

const QDate &todo::ItemGroupDao::getToDate() const {
    return toDate;
}

void todo::ItemGroupDao::setToDate(const QDate &toDate) {
    ItemGroupDao::toDate = toDate;
}
