//
// Created by septemberhx on 5/30/18.
//

#ifndef TODOMANAGER_DATAFILTER_H
#define TODOMANAGER_DATAFILTER_H

#include <QDate>
#include "./AbstractFilter.h"

namespace todo {
class DateFilter : public AbstractFilter {
public:
    DateFilter(const QDate &date);
    bool check(const ItemDetail &itemDetails) override;
private:
    QDate targetDate;
};
}

#endif //TODOMANAGER_DATAFILTER_H
