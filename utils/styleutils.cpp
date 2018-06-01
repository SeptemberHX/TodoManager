#include "styleutils.h"

void StyleUtils::setStyle(const QString &style)
{
   QFile qss(style);
   qss.open(QFile::ReadOnly);
   qApp->setStyleSheet(qss.readAll());
   qss.close();
}

StyleUtils::StyleUtils()
{

}
