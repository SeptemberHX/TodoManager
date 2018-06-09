//
// Created by septemberhx on 5/18/18.
//

#ifndef TODOMANAGER_ABSTRACTFILTER_H
#define TODOMANAGER_ABSTRACTFILTER_H

#include <QList>
#include "../../core/ItemDetail.h"

namespace todo {


/**
 * AbstractFilter is the base class of all filters.
 * Filters are used when we are trying to pick some ItemDetails out due to some rules.
 * Each rule should be implemented as a filter class derived from AbstractFilter.
 * Complex rules should be made of some simple filters.
 */
class AbstractFilter {
public:
    QList<ItemDetail> filter(const QList<ItemDetail> &itemDetails);
    virtual bool check(const ItemDetail &itemDetails) = 0;
};

}


#endif //TODOMANAGER_ABSTRACTFILTER_H
