//
// Created by septemberhx on 8/20/18.
//

#ifndef TODOMANAGER_TASKONTIMENOTIFIER_H
#define TODOMANAGER_TASKONTIMENOTIFIER_H

#include "../data/DataCenter.h"
#include <QTimer>
#include <QMutex>

namespace todo {

class TaskOnTimeNotifier : public QObject {
    Q_OBJECT
public:
    static TaskOnTimeNotifier *getInstance();
    ~TaskOnTimeNotifier();
    void init();

public slots:
    void update_timer();

signals:
    void notifyUser(const QString &titleStr, const QString &bodyStr);

private:
    static TaskOnTimeNotifier *instancePtr;

    DataCenter dataCenter;
    explicit TaskOnTimeNotifier(QObject *parent = 0);

    QTimer *timer;
    qint64 interval;
    QList<todo::ItemDetail> targetItemDetails;
    QMutex notificationListMutex;
};

}

#endif //TODOMANAGER_TASKONTIMENOTIFIER_H
