//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_DONESORTER_H
#define TODOMANAGER_DONESORTER_H

#include "AbstractSorter.h"

namespace todo {

class DoneSorter : public AbstractSorter {
public:
    DoneSorter(bool desc = false);
    int compare(const ItemDetail &item1, const ItemDetail &item2) override;
};

}

#endif //TODOMANAGER_DONESORTER_H
