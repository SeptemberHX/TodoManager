#ifndef LOGGER_H
#define LOGGER_H

#include "logwidget.h"

class Logger
{
public:
    static Logger *instance;
    static Logger *getInstance();
    void init(LogWidget *logWidget);
    void log(const QString &str);

private:
    Logger();
    LogWidget *logWidget;
};

#endif // LOGGER_H
