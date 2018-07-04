//
// Created by septemberhx on 7/4/18.
//

#include "TagModeListItemDelegate.h"
#include <QPainter>
#include "../../core/ItemTag.h"

TagModeListItemDelegate::TagModeListItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

void
TagModeListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // draw background
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor("#7dd1c8"));
    } else {
        painter->fillRect(option.rect, Qt::white);
    }

    // get ItemDetail Object
    todo::ItemTag itemTag = index.data(Qt::UserRole + 1).value<todo::ItemTag>();

    QBrush oldBrush = painter->brush();
    // draw tag color in circle
    painter->setRenderHint(QPainter::Antialiasing, true);
    QColor tagColor = itemTag.getColor();
    int circleWidth = 20;
    QMargins tagColorCircleMargin(5, 5, 5, 5);
    QPoint circleTopLeft = option.rect.topLeft() + QPoint(tagColorCircleMargin.left(), tagColorCircleMargin.top());
    QRect colorRect(circleTopLeft, circleTopLeft + QPoint(circleWidth, circleWidth));

    painter->setBrush(tagColor);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(colorRect);

    // draw tag name
    painter->setPen(Qt::black);
    painter->setBrush(oldBrush);
    QMargins tagNameMargin(5, 5, 5, 5);
    int nameHeight = 20;
    QPoint nameTopLeft = QPoint(
            colorRect.right() + tagColorCircleMargin.right() + tagNameMargin.left(),
            tagNameMargin.top() + option.rect.top()
    );
    int nameWidth = option.rect.width() - colorRect.width() - tagColorCircleMargin.left()
            - tagColorCircleMargin.right() - tagNameMargin.left() - tagNameMargin.right();
    QRect tagNameRect(nameTopLeft, nameTopLeft + QPoint(nameWidth, nameHeight));
    painter->drawText(tagNameRect, Qt::AlignLeft | Qt::AlignVCenter,
                  this->elideText(itemTag.getName(), painter->fontMetrics(), nameWidth));
}

QSize TagModeListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(100, 30);
}

QString TagModeListItemDelegate::elideText(const QString &str, const QFontMetrics &fontMetrics, int maxWidth) const {
    QString newStr(str);
    newStr.replace('\n', ' ');

    if (fontMetrics.width(newStr) > maxWidth) {
        return fontMetrics.elidedText(newStr, Qt::ElideRight, maxWidth);
    }
    return newStr;
}
