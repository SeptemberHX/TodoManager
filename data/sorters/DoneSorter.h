//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_DONESORTER_H
#define TODOMANAGER_DONESORTER_H

#include "AbstractSorter.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace todo {

class DoneSorter : public AbstractSorter {
public:
    DoneSorter(bool desc = false);

    int compare(const ItemAndGroupWrapper &wrapper1, const ItemAndGroupWrapper &wrapper2) override;

private:
    int compare_itemDetail(const ItemDetail &item1, const ItemDetail &item2);
};

}

#endif //TODOMANAGER_DONESORTER_H
