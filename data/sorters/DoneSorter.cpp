//
// Created by septemberhx on 6/10/18.
//

#include "DoneSorter.h"


int todo::DoneSorter::compare(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
    int compareResult = 0;
    if (!item1.isDone() && item2.isDone()) {
        compareResult = -1;
    } else if (item1.isDone() == item2.isDone()) {
        compareResult = 0;
    } else {
        compareResult = 1;
    }

    if (this->desc) {
        compareResult *= -1;
    }
    return compareResult;
}

todo::DoneSorter::DoneSorter(bool desc) : AbstractSorter(desc) {

}
