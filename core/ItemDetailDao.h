//
// Created by septemberhx on 7/29/18.
//

#ifndef TODOMANAGER_ITEMDETAILDAO_H
#define TODOMANAGER_ITEMDETAILDAO_H

#include <QString>
#include <QTime>

namespace todo {

enum ItemMode {
    SIMPLE = 1,
    SCHEDULE = 2,
    RECURSION = 3,
    PLAIN = 4
};

class ItemDetailDao {
public:
    const QString &getTitle() const;

    void setTitle(const QString &title);

    const QString &getDescription() const;

    void setDescription(const QString &description);

    const QTime &getFromTime() const;

    void setFromTime(const QTime &fromTime);

    const QTime &getToTime() const;

    void setToTime(const QTime &toTime);

    const QDate &getTargetDate() const;

    void setTargetDate(const QDate &targetDate);

    const ItemMode &getMode() const;

    void setMode(const ItemMode &mode);

    int getPriority() const;

    void setPriority(int priority);

    const QDateTime &getCreatedTime() const;

    void setCreatedTime(const QDateTime &createdTime);

    const QDateTime &getLastUpdatedTime() const;

    void setLastUpdatedTime(const QDateTime &lastUpdatedTime);

    bool isDone() const;

    void setDone(bool done);

    const QString &getId() const;

    void setId(const QString &id);

private:
    QString title;
    QString description;

    QTime fromTime;
    QTime toTime;

    QDate targetDate;
    ItemMode mode;

    int priority;
    QDateTime createdTime;
    QDateTime lastUpdatedTime;

    bool done;
    QString id;
};

}

#endif //TODOMANAGER_ITEMDETAILDAO_H
