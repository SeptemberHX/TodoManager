//
// Created by septemberhx on 6/3/18.
//

#ifndef TODOMANAGER_SQLITECONFIG_H
#define TODOMANAGER_SQLITECONFIG_H

#include <QString>

namespace todo {

class SQLiteConfig {
public:
    SQLiteConfig() = default;

    SQLiteConfig(const QString &path);

    const QString &getDbPath() const;

    void setDbPath(const QString &dbPath);

private:
    QString dbPath;
};

}

#endif //TODOMANAGER_SQLITECONFIG_H
