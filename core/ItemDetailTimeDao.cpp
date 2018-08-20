//
// Created by septemberhx on 8/20/18.
//

#include "ItemDetailTimeDao.h"

const QString &todo::ItemDetailTimeDao::getItemID() const {
    return itemID;
}

void todo::ItemDetailTimeDao::setItemID(const QString &itemID) {
    ItemDetailTimeDao::itemID = itemID;
}

const QDateTime &todo::ItemDetailTimeDao::getStartTime() const {
    return startTime;
}

void todo::ItemDetailTimeDao::setStartTime(const QDateTime &startTime) {
    ItemDetailTimeDao::startTime = startTime;
}

const QDateTime &todo::ItemDetailTimeDao::getEndTime() const {
    return endTime;
}

void todo::ItemDetailTimeDao::setEndTime(const QDateTime &endTime) {
    ItemDetailTimeDao::endTime = endTime;
}

bool todo::ItemDetailTimeDao::operator<(const todo::ItemDetailTimeDao &other) const {
    if (this->startTime != other.startTime) {
        return this->startTime < other.startTime;
    } else {
        return this->endTime < other.endTime;
    }
}

bool todo::ItemDetailTimeDao::operator==(const todo::ItemDetailTimeDao &other) const {
    return this->itemID == other.itemID &&
            this->startTime == other.startTime &&
            this->endTime == other.endTime;
}

bool todo::ItemDetailTimeDao::operator!=(const todo::ItemDetailTimeDao &other) const {
    return !this->operator==(other);
}
