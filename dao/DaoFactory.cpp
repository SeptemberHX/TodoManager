//
// Created by septemberhx on 5/17/18.
//

#include "DaoFactory.h"

todo::DaoFactory *todo::DaoFactory::instance = nullptr;
QString todo::DaoFactory::daoMethod = "SQLiteDao";

todo::AbstractDao *todo::DaoFactory::getSQLDao() {
    return this->getSQLDao(DaoType::SQLiteDao);
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

}

todo::AbstractDao *todo::DaoFactory::getSQLDao(todo::DaoType type) {
    switch (type) {
        case DaoType::MySQLDao:
            if (this->daoPtrMap.find("MYSQLDao") == this->daoPtrMap.end()) {
                AbstractDao *mySQLDao = new SQLDao(0);
                mySQLDao->init();
                this->daoPtrMap.insert("MYSQLDao", mySQLDao);
            }
            return this->daoPtrMap["MYSQLDao"];
        case DaoType ::SQLiteDao:
            if (this->daoPtrMap.find("SQLiteDao") == this->daoPtrMap.end()) {
                AbstractDao *sqliteDao = new SQLDao(1);
                sqliteDao->init();
                this->daoPtrMap.insert("SQLiteDao", sqliteDao);
            }
            return this->daoPtrMap["SQLiteDao"];
    }
    return nullptr;
}
