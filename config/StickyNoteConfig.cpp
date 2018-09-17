//
// Created by septemberhx on 9/16/18.
//

#include "StickyNoteConfig.h"

QString todo::StickyNoteConfig::DIR("StickyNote");

todo::StickyNoteConfig::StickyNoteConfig() :
    specificDate(false),
    specificTag(false),
    specificProject(false),
    showFinishedTask(true)
{
    this->setDate(QDate::currentDate(), QDate::currentDate());
}

todo::StickyNoteConfig::StickyNoteConfig(const QString &stickyNoteId) :
    StickyNoteConfig()
{
    this->stickyNoteId = stickyNoteId;
}

void todo::StickyNoteConfig::setDate(const QDate &fromDate, const QDate &toDate, bool flag) {
    this->fromDate = fromDate;
    this->toDate = toDate;
    this->specificDate = flag;
}

void todo::StickyNoteConfig::setTagId(const QString &tagId, bool flag) {
    this->tagId = tagId;
    this->specificTag = flag;
}

void todo::StickyNoteConfig::setProjectId(const QString &projectId, bool flag) {
    this->projectId = projectId;
    this->specificProject = flag;
}

void todo::StickyNoteConfig::removeDate() {
    this->specificDate = false;
}

void todo::StickyNoteConfig::removeTagId() {
    this->specificTag = false;
}

void todo::StickyNoteConfig::removeProjectId() {
    this->specificProject = false;
}

bool todo::StickyNoteConfig::isSpecificDate() const {
    return specificDate;
}

bool todo::StickyNoteConfig::isSpecificTag() const {
    return specificTag;
}

bool todo::StickyNoteConfig::isSpecificProject() const {
    return specificProject;
}

const QDate &todo::StickyNoteConfig::getFromDate() const {
    return fromDate;
}

const QDate &todo::StickyNoteConfig::getToDate() const {
    return toDate;
}

const QString &todo::StickyNoteConfig::getTagId() const {
    return tagId;
}

const QString &todo::StickyNoteConfig::getProjectId() const {
    return projectId;
}

bool todo::StickyNoteConfig::isShowFinishedTask() const {
    return showFinishedTask;
}

void todo::StickyNoteConfig::setShowFinishedTask(bool showFinishedTask) {
    StickyNoteConfig::showFinishedTask = showFinishedTask;
}
