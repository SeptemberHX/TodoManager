//
// Created by septemberhx on 8/20/18.
//

#include "TaskArchivingTimeRecorder.h"
#include "../utils/StringUtils.h"
#include <QDebug>

todo::TaskArchivingTimeRecorder *todo::TaskArchivingTimeRecorder::instancePtr = nullptr;

todo::TaskArchivingTimeRecorder *todo::TaskArchivingTimeRecorder::getInstance() {
    if (TaskArchivingTimeRecorder::instancePtr == nullptr) {
        TaskArchivingTimeRecorder::instancePtr = new TaskArchivingTimeRecorder();
    }

    return TaskArchivingTimeRecorder::instancePtr;
}

todo::TaskArchivingTimeRecorder::~TaskArchivingTimeRecorder() {
    delete TaskArchivingTimeRecorder::instancePtr;
}

void todo::TaskArchivingTimeRecorder::start(const QString &itemID) {
    this->startRecord(itemID, QDateTime::currentDateTime());
    this->notifyUser("Attention", QString("%1 started.").arg(this->dataCenter.selectItemDetailByID(itemID).getTitle()));
}

void todo::TaskArchivingTimeRecorder::pause(const QString &itemID) {
    this->endRecord(itemID, QDateTime::currentDateTime());
    this->notifyUser("Attention", QString("%1 paused.").arg(this->dataCenter.selectItemDetailByID(itemID).getTitle()));
    emit itemDetailTimeModified(this->objectName());
}

void todo::TaskArchivingTimeRecorder::resume(const QString &itemID) {
    this->startRecord(itemID, QDateTime::currentDateTime());
    this->notifyUser("Attention", QString("%1 resumed.").arg(this->dataCenter.selectItemDetailByID(itemID).getTitle()));
}

void todo::TaskArchivingTimeRecorder::finish(const QString &itemID) {
    this->pause(itemID);
    this->dataCenter.updateDoneByID(itemID, true);
    this->notifyUser("Attention", QString("%1 finished.").arg(this->dataCenter.selectItemDetailByID(itemID).getTitle()));
    emit itemDetailTimeModified(this->objectName());
}

void todo::TaskArchivingTimeRecorder::saveOneTimePiece(const QString &itemID, const QDateTime &startTime,
                                                   const QDateTime &endTime) {
    ItemDetailTimeDao timeDao;
    timeDao.setItemID(itemID);
    timeDao.setStartTime(startTime);
    timeDao.setEndTime(endTime);
    this->dataCenter.insertItemDetailTime(timeDao);
}

todo::TaskArchivingTimeRecorder::TaskArchivingTimeRecorder(QObject *parent) :
    QObject(parent)
{
    this->setObjectName(StringUtils::generateUniqueID("TaskArchivingTimeRecorder"));
}

todo::TaskArchivingState todo::TaskArchivingTimeRecorder::getTaskArchivingState(const QString &taskID) {
    auto itemDao = this->dataCenter.selectItemDetailByID(taskID);
    auto timePieces = this->dataCenter.selectItemDetailTimeByItemID(taskID);
    if (itemDao.isDone()) return FINISH;
    else if (!this->itemId2StartTimeMap.contains(taskID)) {
        if (timePieces.empty()) return NOT_START;
        else return PAUSE;
    } else return DOING;
}

void todo::TaskArchivingTimeRecorder::operate(const QString &itemID, const todo::TaskArchivingOperation &operation) {
    switch (operation) {
        case TaskArchivingOperation::OPERATION_START:
            this->start(itemID);
            break;
        case TaskArchivingOperation::OPERATION_PAUSE:
            this->pause(itemID);
            break;
        case TaskArchivingOperation::OPERATION_RESUME:
            this->resume(itemID);
            break;
        case TaskArchivingOperation::OPERATION_FINISH:
            this->finish(itemID);
            break;
        default:
            break;
    }
}

void todo::TaskArchivingTimeRecorder::init() {
    ;
}

void todo::TaskArchivingTimeRecorder::notifyUser(const QString &titleStr, const QString &bodyStr) {
    emit showMessage(titleStr, bodyStr);
}

void todo::TaskArchivingTimeRecorder::startRecord(const QString &itemID, const QDateTime &startTime) {
    if (this->itemId2StartTimeMap.contains(itemID)) {
        qDebug() << itemID << "has already started at" << this->itemId2StartTimeMap;
        return;
    }

    this->itemId2StartTimeMap[itemID] = startTime;
}

void todo::TaskArchivingTimeRecorder::endRecord(const QString &itemID, const QDateTime &endTime) {
    if (!this->itemId2StartTimeMap.contains(itemID)) {
        qDebug() << itemID << "has not started yet";
        return;
    }

    this->saveOneTimePiece(itemID, this->itemId2StartTimeMap[itemID], endTime);
    qDebug() << this->itemId2StartTimeMap[itemID];
    this->itemId2StartTimeMap.remove(itemID);
}


