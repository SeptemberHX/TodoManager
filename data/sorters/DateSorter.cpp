//
// Created by septemberhx on 6/10/18.
//

#include "DateSorter.h"

todo::DateSorter::DateSorter(bool desc) : AbstractSorter(desc) {

}

int todo::DateSorter::compare(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
    int compareResult = 0;
    if (item1.getTargetDate() < item2.getTargetDate()) {
        compareResult = -1;
    } else if (item1.getTargetDate() == item2.getTargetDate()) {
        compareResult = 0;
    } else {
        compareResult = 1;
    }

    if (this->desc) {
        compareResult *= -1;
    }

    return compareResult;
}
