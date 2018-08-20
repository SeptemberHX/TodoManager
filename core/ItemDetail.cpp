//
// Created by septemberhx on 4/24/18.
//

#include <QStringBuilder>
#include "ItemDetail.h"
#include "../utils/StringUtils.h"


todo::ItemDetail::ItemDetail() : ItemDetail("") {
}


todo::ItemDetail::ItemDetail(QString title) {
    this->itemDetailDao.setTitle(title);
    this->itemDetailDao.setMode(ItemMode::SIMPLE);
    this->itemDetailDao.setDone(false);
    this->itemDetailDao.setId(todo::StringUtils::generateItemDetailUniqueID());
    this->itemDetailDao.setPriority(3);
    this->itemDetailDao.setTargetDate(QDate::currentDate());
    this->itemDetailDao.setCreatedTime(QDateTime::currentDateTime());
    this->itemDetailDao.setLastUpdatedTime(this->getCreatedTime());
}

todo::ItemDetail::ItemDetail(const todo::ItemDetailDao &itemDetailDao) {
    this->itemDetailDao = itemDetailDao;
}

void todo::ItemDetail::addTag(const todo::ItemTag &tag) {
    this->tags.append(tag);
}

void todo::ItemDetail::removeTag(const todo::ItemTag &tag) {
    this->tags.removeOne(tag);
}

const QString &todo::ItemDetail::getTitle() const {
    return this->itemDetailDao.getTitle();
}

void todo::ItemDetail::setTitle(const QString &title) {
    this->itemDetailDao.setTitle(title);
}

const QString &todo::ItemDetail::getDescription() const {
    return this->itemDetailDao.getDescription();
}

void todo::ItemDetail::setDescription(const QString &description) {
    this->itemDetailDao.setDescription(description);
}

const QTime &todo::ItemDetail::getFromTime() const {
    return this->itemDetailDao.getFromTime();
}

void todo::ItemDetail::setFromTime(const QTime &fromTime) {
    this->itemDetailDao.setFromTime(fromTime);
}

const QTime &todo::ItemDetail::getToTime() const {
    return this->itemDetailDao.getToTime();
}

void todo::ItemDetail::setToTime(const QTime &toTime) {
    this->itemDetailDao.setToTime(toTime);
}

const QDate &todo::ItemDetail::getTargetDate() const {
    return this->itemDetailDao.getTargetDate();
}

void todo::ItemDetail::setTargetDate(const QDate &targetDate) {
    this->itemDetailDao.setTargetDate(targetDate);
}

todo::ItemMode todo::ItemDetail::getMode() const {
    return this->itemDetailDao.getMode();
}

void todo::ItemDetail::setMode(todo::ItemMode mode) {
    this->itemDetailDao.setMode(mode);
}

const QList<todo::ItemTag> &todo::ItemDetail::getTags() const {
    return tags;
}

void todo::ItemDetail::setTags(const QList<todo::ItemTag> &tags) {
    ItemDetail::tags = tags;
}

int todo::ItemDetail::getPriority() const {
    return this->itemDetailDao.getPriority();
}

void todo::ItemDetail::setPriority(int priority) {
    this->itemDetailDao.setPriority(priority);
}

const QDateTime &todo::ItemDetail::getCreatedTime() const {
    return this->itemDetailDao.getCreatedTime();
}

void todo::ItemDetail::setCreatedTime(const QDateTime &createdTime) {
    this->itemDetailDao.setCreatedTime(createdTime);
}

const QDateTime &todo::ItemDetail::getLastUpdatedTime() const {
    return this->itemDetailDao.getLastUpdatedTime();
}

void todo::ItemDetail::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    this->itemDetailDao.setLastUpdatedTime(lastUpdatedTime);
}

bool todo::ItemDetail::isDone() const {
    return this->itemDetailDao.isDone();
}

void todo::ItemDetail::setDone(bool done) {
    this->itemDetailDao.setDone(done);
}

const QString &todo::ItemDetail::getId() const {
    return this->itemDetailDao.getId();
}

void todo::ItemDetail::setId(const QString &id) {
    this->itemDetailDao.setId(id);
}

std::ostream & todo::operator<<(std::ostream &os, const todo::ItemDetail &detail) {
//    os << detail.toString();
    return os;
}

QDataStream &todo::operator<<(QDataStream &os, const todo::ItemDetail &detail) {
    // TODO: support drag and drop to reorder
    return os;
}

QString todo::ItemDetail::toString() const {
    return this->getId();
}

QDataStream &todo::operator>>(QDataStream &os, todo::ItemDetail &detail) {
    // TODO: support drag and drop to reorder
    return os;
}

todo::ItemDetailDao todo::ItemDetail::toDao() const {
    return this->itemDetailDao;
}

bool todo::ItemDetail::hasRootGroup() const {
    return StringUtils::checkIfItemGroup(this->rootGroupID);
}

const QString &todo::ItemDetail::getRootGroupID() const {
    return rootGroupID;
}

void todo::ItemDetail::setRootGroupID(const QString &rootGroupID) {
    ItemDetail::rootGroupID = rootGroupID;
}

const QString &todo::ItemDetail::getDirectGroupID() const {
    return directGroupID;
}

void todo::ItemDetail::setDirectGroupID(const QString &directGroupID) {
    ItemDetail::directGroupID = directGroupID;
}

bool todo::ItemDetail::isTagDiff(const todo::ItemDetail &other) const {
    if (this->tags.size() != other.tags.size()) return true;

    for (int i = 0; i < this->tags.size(); ++i) {
        if (this->tags[i] != other.tags[i]) {
            return true;
        }
    }
    return false;
}

bool todo::ItemDetail::isRootGroupDiff(const todo::ItemDetail &other) const {
    return this->getRootGroupID() != other.getRootGroupID();
}

bool todo::ItemDetail::isDirectGroupDiff(const todo::ItemDetail &other) const {
    return this->getDirectGroupID() != other.getDirectGroupID();
}

todo::ItemGroupRelation todo::ItemDetail::generateRelation() const {
    ItemGroupRelation relation;
    relation.setRootGroupID(this->getRootGroupID());
    relation.setDirectGroupID(this->getDirectGroupID());
    relation.setItemID(this->getId());
    return relation;
}

const QList<todo::ItemDetailTimeDao> &todo::ItemDetail::getTimeDaos() const {
    return timeDaos;
}

void todo::ItemDetail::setTimeDaos(const QList<todo::ItemDetailTimeDao> &timeDaos) {
    ItemDetail::timeDaos = timeDaos;
}

bool todo::ItemDetail::isAchievingTimeDiff(const todo::ItemDetail &other) const {
    if (this->timeDaos.size() != other.timeDaos.size()) return true;

    for (int i = 0; i < this->timeDaos.size(); ++i) {
        if (this->timeDaos[i] != other.timeDaos[i]) {
            return true;
        }
    }
    return false;
}
