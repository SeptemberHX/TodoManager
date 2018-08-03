//
// Created by septemberhx on 7/29/18.
//

#include "ItemDetailDao.h"

const QString &todo::ItemDetailDao::getTitle() const {
    return title;
}

void todo::ItemDetailDao::setTitle(const QString &title) {
    ItemDetailDao::title = title;
}

const QString &todo::ItemDetailDao::getDescription() const {
    return description;
}

void todo::ItemDetailDao::setDescription(const QString &description) {
    ItemDetailDao::description = description;
}

const QTime &todo::ItemDetailDao::getFromTime() const {
    return fromTime;
}

void todo::ItemDetailDao::setFromTime(const QTime &fromTime) {
    ItemDetailDao::fromTime = fromTime;
}

const QTime &todo::ItemDetailDao::getToTime() const {
    return toTime;
}

void todo::ItemDetailDao::setToTime(const QTime &toTime) {
    ItemDetailDao::toTime = toTime;
}

const QDate &todo::ItemDetailDao::getTargetDate() const {
    return targetDate;
}

void todo::ItemDetailDao::setTargetDate(const QDate &targetDate) {
    ItemDetailDao::targetDate = targetDate;
}

const todo::ItemMode &todo::ItemDetailDao::getMode() const {
    return mode;
}

void todo::ItemDetailDao::setMode(const ItemMode &mode) {
    ItemDetailDao::mode = mode;
}

int todo::ItemDetailDao::getPriority() const {
    return priority;
}

void todo::ItemDetailDao::setPriority(int priority) {
    ItemDetailDao::priority = priority;
}

const QDateTime &todo::ItemDetailDao::getCreatedTime() const {
    return createdTime;
}

void todo::ItemDetailDao::setCreatedTime(const QDateTime &createdTime) {
    ItemDetailDao::createdTime = createdTime;
}

const QDateTime &todo::ItemDetailDao::getLastUpdatedTime() const {
    return lastUpdatedTime;
}

void todo::ItemDetailDao::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    ItemDetailDao::lastUpdatedTime = lastUpdatedTime;
}

bool todo::ItemDetailDao::isDone() const {
    return done;
}

void todo::ItemDetailDao::setDone(bool done) {
    ItemDetailDao::done = done;
}

const QString &todo::ItemDetailDao::getId() const {
    return id;
}

void todo::ItemDetailDao::setId(const QString &id) {
    ItemDetailDao::id = id;
}

bool todo::ItemDetailDao::operator==(const todo::ItemDetailDao &other) const {
    return this->title == other.title &&
           this->description == other.description &&
           this->fromTime == other.fromTime &&
           this->toTime == other.toTime &&
           this->targetDate == other.targetDate &&
           this->mode == other.mode &&
           this->priority == other.priority &&
           this->createdTime == other.createdTime &&
           this->lastUpdatedTime == other.lastUpdatedTime &&
           this->done == other.done;
}

bool todo::ItemDetailDao::operator!=(const todo::ItemDetailDao &other) const {
    return operator==(other);
}
