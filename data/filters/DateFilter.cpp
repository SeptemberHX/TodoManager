//
// Created by septemberhx on 5/30/18.
//

#include "DateFilter.h"

QList<todo::ItemDetail> todo::DateFilter::filter(const QList<todo::ItemDetail> &itemDetails) {
    QList<todo::ItemDetail> results;
    for (auto &itemDetail : itemDetails) {
        if (itemDetail.getTargetDate() == this->targetDate) {
            results.append(itemDetail);
        }
    }
    return results;
}

todo::DateFilter::DateFilter(const QDate &date)
    : targetDate(date) {

}
