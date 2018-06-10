//
// Created by septemberhx on 6/2/18.
//

#include "TodoConfig.h"

todo::TodoConfig* todo::TodoConfig::instance = nullptr;

todo::TodoConfig *todo::TodoConfig::getInstance() {
    if (TodoConfig::instance == nullptr) {
        TodoConfig::instance = new TodoConfig();
    }
    return TodoConfig::instance;
}

const todo::SQLiteConfig &todo::TodoConfig::getSqLiteConfig() const {
    return sqLiteConfig;
}

void todo::TodoConfig::setSqLiteConfig(const todo::SQLiteConfig &sqLiteConfig) {
    TodoConfig::sqLiteConfig = sqLiteConfig;
}

const todo::AppConfig &todo::TodoConfig::getAppConfig() const {
    return appConfig;
}

void todo::TodoConfig::setAppConfig(const todo::AppConfig &appConfig) {
    TodoConfig::appConfig = appConfig;
}

const todo::UIConfig &todo::TodoConfig::getUiConfig() const {
    return uiConfig;
}

void todo::TodoConfig::setUiConfig(const todo::UIConfig &uiConfig) {
    TodoConfig::uiConfig = uiConfig;
}
