//
// Created by septemberhx on 5/17/18.
//

#include "DaoFactory.h"

todo::DaoFactory *todo::DaoFactory::instance = nullptr;
QString todo::DaoFactory::daoMethod = "SQLDao";

todo::AbstractDao *todo::DaoFactory::getSQLDao() {
    return this->daoPtrMap[DaoFactory::daoMethod];
}

todo::DaoFactory::DaoFactory() {
    this->initDaoMap();
}

todo::DaoFactory *todo::DaoFactory::getInstance() {
    if (DaoFactory::instance == nullptr) {
        DaoFactory::instance = new DaoFactory();
    }

    return DaoFactory::instance;
}

todo::DaoFactory::~DaoFactory() {
    for (auto daoPtr : this->daoPtrMap.values()) {
        delete daoPtr;
    }

    if (DaoFactory::instance != nullptr) {
        delete DaoFactory::instance;
        DaoFactory::instance = nullptr;
    }
}

void todo::DaoFactory::initDaoMap() {
    AbstractDao *dao = new MySQLDao();
    dao->init();
    this->daoPtrMap.insert("SQLDao", dao);
}
