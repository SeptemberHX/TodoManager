//
// Created by septemberhx on 5/18/18.
//

#include "AbstractSorter.h"
#include "../../core/ItemAndGroupWrapper.h"

todo::AbstractSorter::AbstractSorter(bool desc) : desc(desc) {

}

int todo::AbstractSorter::compare(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
    return compare(ItemAndGroupWrapper(item1), ItemAndGroupWrapper(item2));
}
