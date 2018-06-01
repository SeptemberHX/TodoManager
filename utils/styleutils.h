#ifndef STYLEUTILS_H
#define STYLEUTILS_H


#include <QFile>
#include <QApplication>

class StyleUtils
{
public:
    static void setStyle(const QString &style);
private:
    StyleUtils();
};

#endif // STYLEUTILS_H
