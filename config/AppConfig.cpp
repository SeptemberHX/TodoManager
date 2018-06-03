//
// Created by septemberhx on 6/3/18.
//

#include "AppConfig.h"

const QString &todo::AppConfig::getAppName() const {
    return appName;
}

todo::AppConfig::AppConfig(const QString &name)
    : appName(name) {

}
