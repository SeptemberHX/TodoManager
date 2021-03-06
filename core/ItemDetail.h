//
// Created by septemberhx on 4/24/18.
//

#ifndef TODOMANAGER_ITEMDETAIL_H
#define TODOMANAGER_ITEMDETAIL_H

#include <QMetaType>
#include <ostream>
#include "ItemTag.h"
#include "ItemDetailDao.h"
#include "ItemGroupDao.h"
#include "ItemGroupRelation.h"
#include "ItemDetailTimeDao.h"

namespace todo {
    /**
     * Record details about one item
     */
    class ItemDetail {
    public:
        ItemDetail();
        explicit ItemDetail(QString title);
        explicit ItemDetail(const ItemDetailDao &itemDetailDao);
        ItemDetailDao toDao() const;

        ~ItemDetail() = default;

        void addTag(const ItemTag &tag);
        void removeTag(const ItemTag &tag);

        // ------ Getter and Setter
        const QList<ItemTag> &getTags() const;

        void setTags(const QList<ItemTag> &tags);

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

        ItemMode getMode() const;

        void setMode(ItemMode mode);

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

        bool hasRootGroup() const;

        const QString &getRootGroupID() const;

        void setRootGroupID(const QString &rootGroupID);

        const QString &getDirectGroupID() const;

        void setDirectGroupID(const QString &directGroupID);

        const QList<ItemDetailTimeDao> &getTimeDaos() const;

        void setTimeDaos(const QList<ItemDetailTimeDao> &timeDaos);

        bool isTagDiff(const ItemDetail &other) const;

        bool isRootGroupDiff(const ItemDetail &other) const;

        bool isDirectGroupDiff(const ItemDetail &other) const;

        bool isTimePieceDiff(const ItemDetail &other) const;

        ItemGroupRelation generateRelation() const;

        // ------ End

        QString toString() const;

        friend std::ostream &operator<<(std::ostream &os, const ItemDetail &detail);

        friend QDataStream&operator<<(QDataStream &os, const ItemDetail &detail);

        friend QDataStream&operator>>(QDataStream &os, ItemDetail &detail);


    private:
        QList<ItemTag> tags;
        QList<ItemDetailTimeDao> timeDaos;
        ItemDetailDao itemDetailDao;
        QString rootGroupID;
        QString directGroupID;
    };

}

Q_DECLARE_METATYPE(todo::ItemDetail)


#endif //TODOMANAGER_ITEMDETAIL_H
