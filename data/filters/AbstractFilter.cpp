//
// Created by septemberhx on 5/18/18.
//

#include "AbstractFilter.h"

QList<todo::ItemDetail> todo::AbstractFilter::filter(const QList<todo::ItemDetail> &itemDetails) {
    QList<todo::ItemDetail> results;
    for (auto &itemDetail : itemDetails) {
        if (this->check(itemDetail)) {
            results.append(itemDetail);
        }
    }
    return results;
}
