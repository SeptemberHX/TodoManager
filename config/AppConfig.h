//
// Created by septemberhx on 6/3/18.
//

#ifndef TODOMANAGER_APPCONFIG_H
#define TODOMANAGER_APPCONFIG_H

#include <QString>

namespace todo {

class AppConfig {
public:
    AppConfig(const QString& name = "TodoManager");

    const QString &getAppName() const;

private:
    QString appName;
};

}

#endif //TODOMANAGER_APPCONFIG_H
