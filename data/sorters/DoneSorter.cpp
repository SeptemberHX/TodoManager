//
// Created by septemberhx on 6/10/18.
//

#include "DoneSorter.h"
#include "../../core/ItemAndGroupWrapper.h"


int todo::DoneSorter::compare_itemDetail(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
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

int todo::DoneSorter::compare(const ItemAndGroupWrapper &wrapper1, const ItemAndGroupWrapper &wrapper2) {
    if (wrapper1.isGroup() && wrapper2.isDetail()) {
        return 1;
    } else if (wrapper1.isDetail() && wrapper2.isGroup()) {
        return -1;
    } else if (wrapper1.isGroup() && wrapper2.isGroup()) {
        return 0;
    } else {
        return compare_itemDetail(wrapper1.getItemDetail(), wrapper2.getItemDetail());
    }
}
