//
// Created by septemberhx on 8/17/18.
//

#include "StickyNoteDao.h"

const QString &todo::StickyNoteDao::getId() const {
    return id;
}

void todo::StickyNoteDao::setId(const QString &id) {
    StickyNoteDao::id = id;
}

bool todo::StickyNoteDao::isShown() const {
    return shown;
}

void todo::StickyNoteDao::setShown(bool shown) {
    StickyNoteDao::shown = shown;
}

const QPoint &todo::StickyNoteDao::getPos() const {
    return pos;
}

void todo::StickyNoteDao::setPos(const QPoint &pos) {
    StickyNoteDao::pos = pos;
}

todo::StickyNoteType todo::StickyNoteDao::getType() const {
    return type;
}

void todo::StickyNoteDao::setType(todo::StickyNoteType type) {
    StickyNoteDao::type = type;
}

const QDateTime &todo::StickyNoteDao::getCreatedTime() const {
    return createdTime;
}

void todo::StickyNoteDao::setCreatedTime(const QDateTime &createdTime) {
    StickyNoteDao::createdTime = createdTime;
}

const QDateTime &todo::StickyNoteDao::getLastUpdatedTime() const {
    return lastUpdatedTime;
}

void todo::StickyNoteDao::setLastUpdatedTime(const QDateTime &lastUpdatedTime) {
    StickyNoteDao::lastUpdatedTime = lastUpdatedTime;
}

const QColor &todo::StickyNoteDao::getFontColor() const {
    return fontColor;
}

void todo::StickyNoteDao::setFontColor(const QColor &fontColor) {
    StickyNoteDao::fontColor = fontColor;
}

const QColor &todo::StickyNoteDao::getBackgroundColor() const {
    return backgroundColor;
}

void todo::StickyNoteDao::setBackgroundColor(const QColor &backgroundColor) {
    StickyNoteDao::backgroundColor = backgroundColor;
}
