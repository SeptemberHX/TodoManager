//
// Created by septemberhx on 8/17/18.
//

#include "StickyNote.h"

todo::StickyNote::StickyNote() {
    this->dao.setId(todo::StringUtils::generateStickyNoteUniqueID());
    this->dao.setCreatedTime(QDateTime::currentDateTime());
    this->dao.setLastUpdatedTime(this->dao.getCreatedTime());
    this->dao.setFontColor(Qt::black);
    this->dao.setBackgroundColor(Qt::white);
    this->dao.setShown(false);
    this->dao.setType(StickyNoteType::DAILY);
}

const QString &todo::StickyNote::getId() const {
    return this->dao.getId();
}

void todo::StickyNote::setId(const QString &id) {
    this->dao.setId(id);
}

bool todo::StickyNote::isShown() const {
    return this->dao.isShown();
}

void todo::StickyNote::setShown(bool shown) {
    this->dao.setShown(shown);
}

const QPoint &todo::StickyNote::getPos() const {
    return this->dao.getPos();
}

void todo::StickyNote::setPos(const QPoint &pos) {
    this->dao.setPos(pos);
}

todo::StickyNoteType todo::StickyNote::getType() const {
    return this->dao.getType();
}

void todo::StickyNote::setType(todo::StickyNoteType type) {
    this->dao.setType(type);
}

const QDateTime &todo::StickyNote::getCreatedTime() const {
    return this->dao.getCreatedTime();
}

void todo::StickyNote::setCreatedTime(const QDateTime &createdTime) {
    this->dao.setCreatedTime(createdTime);
}

const QDateTime &todo::StickyNote::getLastUpdatedTime() const {
    return this->dao.getLastUpdatedTime();
}

void todo::StickyNote::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    this->dao.setLastUpdatedTime(lastUpdatedTime);
}

const QColor &todo::StickyNote::getFontColor() const {
    return this->dao.getFontColor();
}

void todo::StickyNote::setFontColor(const QColor &fontColor) {
    this->dao.setFontColor(fontColor);
}

const QColor &todo::StickyNote::getBackgroundColor() const {
    return this->dao.getBackgroundColor();
}

void todo::StickyNote::setBackgroundColor(const QColor &backgroundColor) {
    this->dao.setBackgroundColor(backgroundColor);
}

const QString &todo::StickyNote::getName() const {
    return this->dao.getName();
}

void todo::StickyNote::setName(const QString &name) {
    this->dao.setName(name);
}

todo::StickyNote::StickyNote(const todo::StickyNoteDao &dao) :
    dao(dao)
{

}

todo::StickyNoteDao todo::StickyNote::toDao() const {
    return this->dao;
}
