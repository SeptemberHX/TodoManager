//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_DATESORTER_H
#define TODOMANAGER_DATESORTER_H

#include "AbstractSorter.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace todo {

class DateSorter : public AbstractSorter {
public:
    DateSorter(bool desc = false);
    int compare(const ItemAndGroupWrapper &wrapper1, const ItemAndGroupWrapper &wrapper2) override;
};

}

#endif //TODOMANAGER_DATESORTER_H
