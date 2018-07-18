//
// Created by septemberhx on 4/24/18.
//

#include <QStringBuilder>
#include <QUuid>
#include "ItemDetail.h"


todo::ItemDetail::ItemDetail() {
    ItemDetail("");
}


todo::ItemDetail::ItemDetail(QString title) {
    this->title = title;
    this->mode = ItemMode::SIMPLE;
    this->done = false;
    this->id = QString("todo_%1_%2").arg(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()))
                                    .arg(QUuid::createUuid().toString().remove('{').remove('}'));
    this->priority = 3;
}

void todo::ItemDetail::addTag(const todo::ItemTag &tag) {
    this->tags.append(tag);
}

void todo::ItemDetail::removeTag(const todo::ItemTag &tag) {
    this->tags.removeOne(tag);
}

const QString &todo::ItemDetail::getTitle() const {
    return title;
}

void todo::ItemDetail::setTitle(const QString &title) {
    ItemDetail::title = title;
}

const QString &todo::ItemDetail::getDescription() const {
    return description;
}

void todo::ItemDetail::setDescription(const QString &description) {
    ItemDetail::description = description;
}

const QTime &todo::ItemDetail::getFromTime() const {
    return fromTime;
}

void todo::ItemDetail::setFromTime(const QTime &fromTime) {
    ItemDetail::fromTime = fromTime;
}

const QTime &todo::ItemDetail::getToTime() const {
    return toTime;
}

void todo::ItemDetail::setToTime(const QTime &toTime) {
    ItemDetail::toTime = toTime;
}

const QDate &todo::ItemDetail::getTargetDate() const {
    return targetDate;
}

void todo::ItemDetail::setTargetDate(const QDate &targetDate) {
    ItemDetail::targetDate = targetDate;
}

todo::ItemMode todo::ItemDetail::getMode() const {
    return mode;
}

void todo::ItemDetail::setMode(todo::ItemMode mode) {
    ItemDetail::mode = mode;
}

const QList<todo::ItemTag> &todo::ItemDetail::getTags() const {
    return tags;
}

void todo::ItemDetail::setTags(const QList<todo::ItemTag> &tags) {
    ItemDetail::tags = tags;
}

int todo::ItemDetail::getPriority() const {
    return priority;
}

void todo::ItemDetail::setPriority(int priority) {
    ItemDetail::priority = priority;
}

const QDateTime &todo::ItemDetail::getCreatedTime() const {
    return createdTime;
}

void todo::ItemDetail::setCreatedTime(const QDateTime &createdTime) {
    ItemDetail::createdTime = createdTime;
}

const QDateTime &todo::ItemDetail::getLastUpdatedTime() const {
    return lastUpdatedTime;
}

void todo::ItemDetail::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    ItemDetail::lastUpdatedTime = lastUpdatedTime;
}

bool todo::ItemDetail::isDone() const {
    return done;
}

void todo::ItemDetail::setDone(bool done) {
    ItemDetail::done = done;
}

const QString &todo::ItemDetail::getId() const {
    return id;
}

void todo::ItemDetail::setId(const QString &id) {
    ItemDetail::id = id;
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
    QString tagStr;
    for (auto &tag : this->getTags()) {
        if (tagStr.size() > 0) {
            tagStr.append('|');
        }
        tagStr += tag.getName();
    }

    QString result("id: %1 title: %2 description: %3 fromTime: %4 toTime: %5 targetDate: %6 tag: %7 priority: %8 mode %9");
    result = result.arg(this->title).arg(this->description).arg(this->fromTime.toString("hh-mm")).arg(this->toTime.toString("hh:mm"));
    result = result.arg(tagStr).arg(this->priority).arg(this->mode);
    return result;
}

QDataStream &todo::operator>>(QDataStream &os, todo::ItemDetail &detail) {
    // TODO: support drag and drop to reorder
    return os;
}
