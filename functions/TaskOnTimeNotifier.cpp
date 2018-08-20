//
// Created by septemberhx on 8/20/18.
//

#include "TaskOnTimeNotifier.h"
#include <QDebug>

todo::TaskOnTimeNotifier *todo::TaskOnTimeNotifier::instancePtr = nullptr;

todo::TaskOnTimeNotifier *todo::TaskOnTimeNotifier::getInstance() {
    if (TaskOnTimeNotifier::instancePtr == nullptr) {
        TaskOnTimeNotifier::instancePtr = new TaskOnTimeNotifier();
    }

    return TaskOnTimeNotifier::instancePtr;
}

todo::TaskOnTimeNotifier::~TaskOnTimeNotifier() {
    delete TaskOnTimeNotifier::instancePtr;
}

void todo::TaskOnTimeNotifier::init() {
    this->timer = new QTimer();
    this->interval = 60000;
    connect(this->timer, &QTimer::timeout, this, &TaskOnTimeNotifier::update_timer);
    this->update_timer();
}

todo::TaskOnTimeNotifier::TaskOnTimeNotifier(QObject *parent) :
    QObject(parent)
{
    ;
}

void todo::TaskOnTimeNotifier::update_timer() {
    QMutexLocker locker(&this->notificationListMutex);
    for (auto const &item : this->targetItemDetails) {
        if (QTime::currentTime().msecsTo(item.getFromTime()) <= 0) {
            emit notifyUser("Task begins", item.getTitle());
        }
    }
    this->targetItemDetails.clear();

    // select most recently items, and set it to targetItemDetails
    this->targetItemDetails = this->dataCenter.selectNextNotifiedItemDetail();
    for (auto const &item : this->targetItemDetails) {
        qDebug() << "Next item to notify: " << item.getTitle();
    }

    // calculate next timeout round
    qint64 nextInterval = this->interval;
    if (!this->targetItemDetails.empty()) {
        QDateTime nextItemTime;
        nextItemTime.setDate(this->targetItemDetails[0].getTargetDate());
        nextItemTime.setTime(this->targetItemDetails[0].getFromTime());
        nextInterval = QDateTime::currentDateTime().msecsTo(nextItemTime);
    }
    this->timer->start(int(nextInterval));
    qDebug() << "Wait for " << int(nextInterval) / 1000 << " secs to notify next item";
}
