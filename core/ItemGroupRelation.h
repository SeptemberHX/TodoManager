//
// Created by septemberhx on 7/17/18.
//

#ifndef TODOMANAGER_ITEMGROUPRELATION_H
#define TODOMANAGER_ITEMGROUPRELATION_H

#include <QString>

namespace todo {

class ItemGroupRelation {
public:
    const QString &getDirectGroupID() const;

    void setDirectGroupID(const QString &directGroupID);

    const QString &getRootGroupID() const;

    void setRootGroupID(const QString &rootGroupID);

    const QString &getItemID() const;

    void setItemID(const QString &itemID);

private:
    QString directGroupID;
    QString rootGroupID;
    QString itemID;
};

}


#endif //TODOMANAGER_ITEMGROUPRELATION_H
