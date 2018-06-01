#include "logger.h"

Logger *Logger::instance = nullptr;

Logger *Logger::getInstance()
{
    if (Logger::instance == nullptr) {
        Logger::instance = new Logger();
    }

    return Logger::instance;
}

void Logger::init(LogWidget *logWidget)
{
    this->logWidget = logWidget;
}

void Logger::log(const QString &str)
{
    if (this->logWidget != nullptr) {
        this->logWidget->append(str);
    }
}

Logger::Logger()
{
    this->logWidget = nullptr;
}
