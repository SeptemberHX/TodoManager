//
// Created by septemberhx on 6/10/18.
//

#include "DateSorter.h"
#include "../../core/ItemAndGroupWrapper.h"

todo::DateSorter::DateSorter(bool desc) : AbstractSorter(desc) {

}

int todo::DateSorter::compare(const ItemAndGroupWrapper &wrapper1, const ItemAndGroupWrapper &wrapper2) {
    if (wrapper1.isGroup() && wrapper2.isDetail()) {
        return 1;
    } else if (wrapper1.isDetail() && wrapper2.isGroup()) {
        return -1;
    } else if (wrapper1.isGroup() && wrapper2.isGroup()) {
        ItemDetail item1 = wrapper1.getItemDetail();
        ItemDetail item2 = wrapper2.getItemDetail();

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
    } else {
        ItemGroup group1 = wrapper1.getItemGroup();
        ItemGroup group2 = wrapper2.getItemGroup();

        int compareResult = 0;
        if (group1.getFromDate() < group2.getFromDate()) {
            compareResult = -1;
        } else if (group1.getFromDate() == group2.getFromDate()) {
            if (group1.getToDate() < group2.getToDate()) {
                compareResult = -1;
            } else if (group1.getToDate() == group2.getToDate()) {
                compareResult = 0;
            } else {
                compareResult = 1;
            }
        } else {
            compareResult = 1;
        }

        return compareResult;
    }
}
