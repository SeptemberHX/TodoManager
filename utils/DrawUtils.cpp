//
// Created by septemberhx on 7/7/18.
//

#include "DrawUtils.h"
#include <QDebug>
#include <QtMath>
#include "StringUtils.h"

void todo::DrawUtils::drawRectWithCircle(QPainter &painter, const QFont &font, const QColor &fontColor,
                                         const QString &string, const QRect &rect, const QColor &rectBgColor,
                                         double percent) {
    if (rect.width() < rect.height()) {
        qDebug() << "rect.width less than height !";
        qDebug() << "stop drawing.";
        return;
    }

    int circleRadius = rect.height() / 2;
    QPainterPath painterPath;
    QRect leftCircleRect = QRect(rect.topLeft(), rect.topLeft() + QPoint(2 * circleRadius, 2 * circleRadius));
    QRect rightCircleRect = QRect(rect.topRight() + QPoint(-2 * circleRadius, 0), rect.bottomRight());

    // fill background according to percent
    painter.setPen(Qt::NoPen);
    int progressWidth = int(rect.width() * percent);
    QPainterPath fillPath;
    if (progressWidth < circleRadius) {
        qreal halfArc = qRadiansToDegrees(qAcos((circleRadius - progressWidth) * 1.0 / circleRadius));
        fillPath.arcMoveTo(leftCircleRect, 180 - halfArc);
        fillPath.arcTo(leftCircleRect, 180 - halfArc, 2 * halfArc);
        fillPath.closeSubpath();
    } else if (rect.width() - progressWidth > circleRadius) {
        fillPath.moveTo(leftCircleRect.topRight() + QPoint(-circleRadius, 0));
        fillPath.arcTo(leftCircleRect, 90, 180);
        fillPath.lineTo(rect.bottomLeft() + QPoint(progressWidth, 0));
        fillPath.lineTo(rect.topLeft() + QPoint(progressWidth, 0));
        fillPath.closeSubpath();
    } else {
        qreal halfArc = qRadiansToDegrees(qAcos((circleRadius - rect.width() + progressWidth) * 1.0 / circleRadius));
        fillPath.arcMoveTo(rightCircleRect, halfArc);
        fillPath.arcTo(rightCircleRect, halfArc, 90 - halfArc);
        fillPath.lineTo(leftCircleRect.topRight() + QPoint(-circleRadius, 0));
        fillPath.arcTo(leftCircleRect, 90, 180);
        fillPath.lineTo(rightCircleRect.bottomRight() + QPoint(-circleRadius, 0));

        fillPath.arcTo(rightCircleRect, -90, 90 - halfArc);
        fillPath.closeSubpath();
    }
    painter.fillPath(fillPath, rectBgColor);

    // draw the bound
    painter.setPen(QPen(Qt::gray, 2));
    painterPath.moveTo(leftCircleRect.topRight() + QPoint(-circleRadius, 0));
    painterPath.arcTo(leftCircleRect, 90, 180);
    painterPath.lineTo(rightCircleRect.bottomRight() + QPoint(-circleRadius, 0));
    painterPath.arcTo(rightCircleRect, -90, 180);
    painterPath.closeSubpath();

    painter.drawPath(painterPath);

    // draw text
    QRect textRect(leftCircleRect.topRight() + QPoint(-circleRadius, 0),
                   rightCircleRect.bottomRight() + QPoint(-circleRadius, 0));
    painter.setFont(font);
    painter.setPen(fontColor);
    auto elidedStr = StringUtils::elideText(string, painter.fontMetrics(), rect.width() - 2 * circleRadius);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedStr);
}
