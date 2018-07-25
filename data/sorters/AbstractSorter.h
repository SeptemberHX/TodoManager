//
// Created by septemberhx on 5/18/18.
//

#ifndef TODOMANAGER_ABSTRACTSORTER_H
#define TODOMANAGER_ABSTRACTSORTER_H


#include <QList>
#include "../../core/ItemDetail.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace todo {

/**
 * AbstractSorter is the base class of all sorters.
 * Sorters are used when we are trying to sort some ItemDetails due to some rules.
 * Each rule should be implemented as a sorter class derived from AbstractSorter.
 * Complex rules should be made of some simple sorters.
 */
class AbstractSorter {
public:
    AbstractSorter(bool desc = false);
    /**
     * Child classes just need to implement compare function
     * @param item1
     * @param item2
     * @return -1 if item1 < item2; 0 if item1 = item2; 1 if item1 > item2
     */
    virtual int compare(const ItemDetail &item1, const ItemDetail &item2);
    virtual int compare(const ItemAndGroupWrapper &wrapper1, const ItemAndGroupWrapper &wrapper2) = 0;

protected:
    bool desc;
};

}


#endif //TODOMANAGER_ABSTRACTSORTER_H
