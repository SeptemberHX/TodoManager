//
// Created by septemberhx on 7/7/18.
//

#ifndef TODOMANAGER_DRAWUTILS_H
#define TODOMANAGER_DRAWUTILS_H

#include <QPainter>
#include <QFont>
#include <QColor>
#include <QString>
#include <QRect>

namespace todo {

class DrawUtils {
public:
    /**
     * draw rect with two half circle, and draw text
     * and fill background with given color according to percent(like progress bar).
     * @param painter: painter
     * @param font: text font
     * @param fontColor: text font color
     * @param string: text string
     * @param rect: rect which all things will draw within
     * @param rectBgColor: background color
     * @param percent: progress.
     */
    static void drawRectWithCircle(QPainter &painter,
                                   const QFont &font,
                                   const QColor &fontColor,
                                   const QString &string,
                                   const QRect &rect,
                                   const QColor &rectBgColor,
                                   double percent = 1.0);
};

}

#endif //TODOMANAGER_DRAWUTILS_H
