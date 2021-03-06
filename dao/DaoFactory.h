//
// Created by septemberhx on 5/17/18.
//

#ifndef TODOMANAGER_DAOFACTORY_H
#define TODOMANAGER_DAOFACTORY_H

#include <QMap>
#include "AbstractDao.h"
#include "SQLDao.h"

namespace todo {

enum DaoType {
    MySQLDao,
    SQLiteDao
};

class DaoFactory {
public:
    static DaoFactory* getInstance();
    AbstractDao* getSQLDao();
    AbstractDao* getSQLDao(DaoType type);
    ~DaoFactory();

private:
    DaoFactory();
    void initDaoMap();
    static DaoFactory *instance;
    QMap<QString, AbstractDao*> daoPtrMap;
    static QString daoMethod;
};

}

#endif //TODOMANAGER_DAOFACTORY_H
