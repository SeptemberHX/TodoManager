//
// Created by septemberhx on 8/20/18.
//

#ifndef TODOMANAGER_ITEMDETAILTIMEDAO_H
#define TODOMANAGER_ITEMDETAILTIMEDAO_H

#include <QDateTime>
#include <QDateTime>
#include <QString>

namespace todo {

class ItemDetailTimeDao {
public:
    const QString &getItemID() const;

    void setItemID(const QString &itemID);

    const QDateTime &getStartTime() const;

    void setStartTime(const QDateTime &startTime);

    const QDateTime &getEndTime() const;

    void setEndTime(const QDateTime &endTime);

    bool operator<(const ItemDetailTimeDao &other) const;

    bool operator==(const ItemDetailTimeDao &other) const;

    bool operator!=(const ItemDetailTimeDao &other) const;

private:
    QString itemID;
    QDateTime startTime;
    QDateTime endTime;
};

}


#endif //TODOMANAGER_ITEMDETAILTIMEDAO_H
