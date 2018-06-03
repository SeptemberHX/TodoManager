//
// Created by septemberhx on 6/3/18.
//

#include "SQLiteConfig.h"

const QString &todo::SQLiteConfig::getDbPath() const {
    return dbPath;
}

void todo::SQLiteConfig::setDbPath(const QString &dbPath) {
    SQLiteConfig::dbPath = dbPath;
}

todo::SQLiteConfig::SQLiteConfig(const QString &path)
    : dbPath(path) {

}
