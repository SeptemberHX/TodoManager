//
// Created by septemberhx on 8/20/18.
//

#ifndef TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H
#define TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H

#include <QString>
#include <QDateTime>
#include "../data/DataCenter.h"

namespace todo {

enum TaskArchivingState {
    NOT_START,
    DOING,
    PAUSE,
    FINISH
};

class TaskArchivingTimeRecorder : public QObject {
    Q_OBJECT
public:
    static TaskArchivingTimeRecorder *getInstance();
    ~TaskArchivingTimeRecorder();

public slots:
    void start(const QString &itemID);
    void finish(const QString &itemID);
    void pause(const QString &itemID);
    void resume(const QString &itemID);

signals:
    void itemDetailTimeModified();

private:
    static TaskArchivingTimeRecorder *instancePtr;

    explicit TaskArchivingTimeRecorder(QObject *parent = nullptr);
    QMap<QString, QDateTime> itemId2StartTimeMap;
    DataCenter dataCenter;

    void saveOneTimePiece(const QString &itemID, const QDateTime &startTime, const QDateTime &endTime);
    TaskArchivingState getTaskArchivingState(const QString &taskID);
};

}


#endif //TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H
