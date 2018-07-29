//
// Created by septemberhx on 7/29/18.
//

#ifndef TODOMANAGER_ITEMGROUPDAO_H
#define TODOMANAGER_ITEMGROUPDAO_H

#include <QString>
#include <QDate>
#include <QDateTime>

namespace todo {

enum ItemGroupType {
    PROJECT,  // top project
    SUB_PROJECT  // child project. For sql operation simplify
};


class ItemGroupDao {
public:
    const QString &getTitle() const;

    void setTitle(const QString &title);

    const QString &getId() const;

    void setId(const QString &id);

    const QString &getDescription() const;

    void setDescription(const QString &description);

    ItemGroupType getType() const;

    void setType(ItemGroupType type);

    bool isMileStone() const;

    void setMileStone(bool mileStone);

    const QDateTime &getCreatedTime() const;

    void setCreatedTime(const QDateTime &createdTime);

    const QDateTime &getLastUpdatedTime() const;

    void setLastUpdatedTime(const QDateTime &lastUpdatedTime);

    const QDate &getFromDate() const;

    void setFromDate(const QDate &fromDate);

    const QDate &getToDate() const;

    void setToDate(const QDate &toDate);

private:
    QString title;                  // 1
    QString id;                     // 2
    QString description;            // 3
    ItemGroupType type;             // 4
    bool mileStone;                 // 5
    QDateTime createdTime;          // 6
    QDateTime lastUpdatedTime;      // 7
    QDate fromDate;                 // 8
    QDate toDate;                   // 9
};

}


#endif //TODOMANAGER_ITEMGROUPDAO_H
