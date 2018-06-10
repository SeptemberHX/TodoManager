//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_DATESORTER_H
#define TODOMANAGER_DATESORTER_H

#include "AbstractSorter.h"

namespace todo {

class DateSorter : public AbstractSorter {
public:
    DateSorter(bool desc = false);
    int compare(const ItemDetail &item1, const ItemDetail &item2) override;
};

}

#endif //TODOMANAGER_DATESORTER_H
