//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_SORTERORGANIZE_H
#define TODOMANAGER_SORTERORGANIZE_H


#include <QList>
#include "./AbstractSorter.h"

namespace todo {

class SorterOrganize {
public:
    SorterOrganize(const QList<AbstractSorter*> &sorterPtrs);
    bool operator()(const todo::ItemDetail &item1, const todo::ItemDetail &item2);

private:
    QList<AbstractSorter*> sorterPtrs;
};

}


#endif //TODOMANAGER_SORTERORGANIZE_H
