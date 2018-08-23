//
// Created by septemberhx on 8/20/18.
//

#ifndef TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H
#define TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H

#include <QString>
#include <QDateTime>
#include <QMap>
#include "../data/DataCenter.h"

namespace todo {

enum TaskArchivingState {
    NOT_START,
    DOING,
    PAUSE,
    FINISH
};

enum TaskArchivingOperation {
    OPERATION_START,
    OPERATION_PAUSE,
    OPERATION_RESUME,
    OPERATION_FINISH
};

class TaskArchivingTimeRecorder : public QObject {
    Q_OBJECT
public:
    static TaskArchivingTimeRecorder *getInstance();
    void init();
    ~TaskArchivingTimeRecorder();
    TaskArchivingState getTaskArchivingState(const QString &taskID);

public slots:
    void start(const QString &itemID);
    void finish(const QString &itemID);
    void pause(const QString &itemID);
    void resume(const QString &itemID);
    void operate(const QString &itemID, const TaskArchivingOperation &operation);

signals:
    void itemDetailTimeModified(const QString &senderObjectName);
    void showMessage(const QString &titleStr, const QString &bodyStr);

private:
    static TaskArchivingTimeRecorder *instancePtr;

    explicit TaskArchivingTimeRecorder(QObject *parent = nullptr);
    QMap<QString, QDateTime> itemId2StartTimeMap;
    DataCenter dataCenter;

    void saveOneTimePiece(const QString &itemID, const QDateTime &startTime, const QDateTime &endTime);
    void notifyUser(const QString &titleStr, const QString &bodyStr);

    void startRecord(const QString &itemID, const QDateTime &startTime);
    void endRecord(const QString &itemID, const QDateTime &endTime);
};

}


#endif //TODOMANAGER_TASKARCHIEVINGTIMERECORDER_H
