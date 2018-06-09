//
// Created by septemberhx on 5/30/18.
//

#include "DateFilter.h"

todo::DateFilter::DateFilter(const QDate &date)
    : targetDate(date) {

}

bool todo::DateFilter::check(const todo::ItemDetail &itemDetail) {
    if (itemDetail.getTargetDate() == this->targetDate) {
        return true;
    }

    return false;
}
