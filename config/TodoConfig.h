//
// Created by septemberhx on 6/2/18.
//

#ifndef TODOMANAGER_TODOCONFIG_H
#define TODOMANAGER_TODOCONFIG_H

#include "SQLiteConfig.h"
#include "AppConfig.h"

namespace todo {

class TodoConfig {
public:
    static TodoConfig* getInstance();

    const SQLiteConfig &getSqLiteConfig() const;

    void setSqLiteConfig(const SQLiteConfig &sqLiteConfig);

    const AppConfig &getAppConfig() const;

    void setAppConfig(const AppConfig &appConfig);

    void loadAllConfig();

private:
    static TodoConfig *instance;
    SQLiteConfig sqLiteConfig;
    AppConfig appConfig;
};

}

#endif //TODOMANAGER_TODOCONFIG_H
