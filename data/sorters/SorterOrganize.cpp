//
// Created by septemberhx on 6/10/18.
//

#include "SorterOrganize.h"

todo::SorterOrganize::SorterOrganize(const QList<todo::AbstractSorter *> &sorterPtrs) : sorterPtrs(sorterPtrs) {

}

bool todo::SorterOrganize::operator()(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
    return operator()(ItemAndGroupWrapper(item1), ItemAndGroupWrapper(item2));
}

bool
todo::SorterOrganize::operator()(const todo::ItemAndGroupWrapper &wrapper1, const todo::ItemAndGroupWrapper &wrapper2) {
    for (auto sorterPtr : this->sorterPtrs) {
        int compareResult = sorterPtr->compare(wrapper1, wrapper2);
        switch (compareResult) {
            case 0:
                continue;
            case 1:
                return false;
            case -1:
                return true;
            default:
                continue;
        }
    }
    return false;
}
