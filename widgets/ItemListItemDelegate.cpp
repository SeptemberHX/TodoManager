//
// Created by septemberhx on 5/8/18.
//

#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include "ItemListItemDelegate.h"
#include "../core/ItemDetail.h"
#include "../config/TodoConfig.h"
#include "../utils/StringUtils.h"

ItemListItemDelegate::ItemListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {

}

void
ItemListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // draw background
    painter->fillRect(option.rect, Qt::white);

    // draw item bottom line
    QPoint lineStartPos = option.rect.bottomLeft() + QPoint(20, 0);
    QPoint lineEndPos = option.rect.bottomRight() + QPoint(-20, 0);
    painter->setPen(QPen(Qt::lightGray, 1));
    painter->drawLine(QLine(lineStartPos, lineEndPos));

    // get ItemDetail Object
    todo::ItemDetail itemDetail = index.data(Qt::UserRole + 1).value<todo::ItemDetail>();

    // draw tag color
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPainterPath painterPath;
    int arcLength = 10;
    auto const & itemTags = itemDetail.getTags();
    if (!itemTags.empty()) {
        auto firstTag = itemTags[0];
        auto tagColorRect = QRect(option.rect.topLeft() + QPoint(0, arcLength), option.rect.bottomLeft()
                + QPoint(arcLength, -arcLength));
        auto arcRectF = QRectF(tagColorRect.topLeft() + QPoint(-arcLength, 0), tagColorRect.topRight()
                + QPoint(0, arcLength * 2));
        painterPath.moveTo(arcRectF.center());
        painterPath.arcTo(arcRectF, 0, 90);
        painterPath.closeSubpath();
        painterPath.lineTo(tagColorRect.topLeft());
        painterPath.lineTo(tagColorRect.bottomLeft() + QPoint(0, -arcLength));
        auto arcRectF2 = QRectF(tagColorRect.bottomLeft() + QPoint(-arcLength, -arcLength * 2), tagColorRect.bottomRight());
        painterPath.arcTo(arcRectF2, -90, 90);

        painterPath.lineTo(tagColorRect.topRight() + QPoint(0, 10));
        painterPath.closeSubpath();
        painter->fillPath(painterPath, QBrush(firstTag.getColor()));
    }

    // draw title
    painter->setFont(QFont("Arias", 14));
    painter->setPen(Qt::black);
    QBrush oldBrush = painter->brush();
    QMargins titleMargins(10, 5, 10, 2);
    int priorityNumWidth = 20;
    int titleSpace = 5;
    int titleWidth = option.rect.width() - 2 * arcLength - titleMargins.left() - titleMargins.right() - priorityNumWidth - titleSpace;
    int titleHeight = 30;
    auto titleRect = QRect(option.rect.topLeft() + QPoint(titleMargins.left() + arcLength, titleMargins.top()),
            QSize(titleWidth, titleHeight));
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemDetail.getTitle(), painter->fontMetrics(), titleWidth));

    // draw priority circle
    painter->setFont(QFont("Arias", 11));
    QRect priorityNumRect(titleRect.right() + titleSpace, titleRect.top() + 5, priorityNumWidth, priorityNumWidth);
    oldBrush = painter->brush();

    QColor priorityBackgroundColor = QColor(Qt::red);
    auto pbcMap = todo::TodoConfig::getInstance()->getUiConfig().getListViewPrioriyBackgroundColorMap();
    if (pbcMap.find(itemDetail.getPriority()) != pbcMap.end()) {
        priorityBackgroundColor = pbcMap[itemDetail.getPriority()];
    }
    painter->setBrush(priorityBackgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(priorityNumRect);

    painter->setPen(Qt::white);
    painter->drawText(priorityNumRect, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(itemDetail.getPriority()));
    painter->setBrush(oldBrush);

    // draw tags
    QMargins tagMargins(3, 1, 3, 1);
    int tagSpacing = 5;
    QMargins tagGroupMargins(30, 0, 10, 0);
    QRect tagGroupRect(
            option.rect.left() + arcLength + tagGroupMargins.left(),
            option.rect.top() + titleMargins.top() + titleRect.height() + titleMargins.bottom() + tagGroupMargins.top(),
            option.rect.width() - arcLength * 2 - tagGroupMargins.left() - tagGroupMargins.right(),
            20
            );
    QRect availableRect(tagGroupRect);
    int fixedWidth = painter->fontMetrics().width("...");

    painter->setFont(QFont("Arias", 8));
    for (auto tag : itemDetail.getTags()) {
        int tagNameWidth = painter->fontMetrics().width(tag.getName());
        int curTagWidth = tagNameWidth + tagMargins.left() + tagMargins.right();
        QRect curTagRect(availableRect.topLeft(), availableRect.bottomLeft() + QPoint(curTagWidth, 0));
        if (curTagWidth + tagSpacing < availableRect.width() - fixedWidth) {
            painter->setBrush(tag.getColor());
            painter->setPen(Qt::NoPen);
            painter->drawRoundRect(curTagRect);

            painter->setBrush(oldBrush);
            painter->setPen(Qt::white);
            painter->drawText(curTagRect, Qt::AlignVCenter | Qt::AlignHCenter, tag.getName());

            availableRect.adjust(curTagRect.width() + tagSpacing, 0, 0, 0);
        } else {
            painter->setPen(Qt::black);
            painter->drawText(curTagRect, Qt::AlignLeft | Qt::AlignVCenter, "...");
            break;
        }
    }

    // draw date and time info
    painter->setFont(QFont("Arias", 8));
    painter->setPen(Qt::gray);
    QMargins infoMargins(5, 5, 5, 5);
    int infoRectTop = titleMargins.top() + titleRect.height() + titleMargins.bottom()
            + tagGroupMargins.top() + tagGroupRect.height() + tagGroupMargins.bottom() + infoMargins.top();
    int infoWidth = 100;
    int infoHeight = option.rect.height() - infoRectTop - infoMargins.bottom();
    QRect dateRect(
            option.rect.right() - arcLength - infoMargins.right() - infoWidth,
            option.rect.bottom() - infoMargins.bottom() - infoHeight,
            infoWidth,
            infoHeight / 2
    );
    painter->drawText(dateRect, Qt::AlignLeft | Qt::AlignBottom, itemDetail.getTargetDate().toString("yyyy/MM/dd"));

    // draw schedule time scope
    QRect timeRect(dateRect);
    if (itemDetail.getMode() != todo::ItemMode::SIMPLE) {
        painter->setPen(Qt::gray);
        timeRect.moveBottom(dateRect.bottom() + infoHeight / 2);
        painter->drawText(timeRect, Qt::AlignLeft | Qt::AlignBottom,
                          itemDetail.getFromTime().toString("hh:mm") + "-" + itemDetail.getToTime().toString("hh:mm"));
    }

    // draw description
    QMargins descriptionMargins(20, 5, 5, 5);
    int descriptionRectTop = titleMargins.top() + titleRect.height() + titleMargins.bottom()
            + tagGroupMargins.top() + tagGroupRect.height() + tagGroupMargins.bottom() + descriptionMargins.top();
    int descriptionHeight = option.rect.height() - descriptionRectTop - descriptionMargins.bottom();
    QRect descriptionRect(
            option.rect.left() + arcLength + descriptionMargins.left(),
            option.rect.bottom() - descriptionMargins.bottom() - descriptionHeight,
            option.rect.width() - arcLength - infoMargins.left() - infoMargins.right() - infoWidth - descriptionMargins.right() - descriptionMargins.left() - arcLength,
            descriptionHeight
    );
    painter->setPen(Qt::gray);
    painter->setFont(QFont("Arias", 10));
    painter->drawText(descriptionRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemDetail.getDescription(), painter->fontMetrics(), descriptionRect.width()));

    // draw isDone color label
    QPainterPath doneLabelPath;
    QRect isDoneColorRect(option.rect.topRight() + QPoint(-arcLength, arcLength),
                          option.rect.bottomRight() + QPoint(0, -arcLength));
    QRectF isDoneArcRect(isDoneColorRect.topLeft(), isDoneColorRect.topLeft() + QPoint(arcLength*2, arcLength*2));
    doneLabelPath.moveTo(isDoneArcRect.center());
    doneLabelPath.arcTo(isDoneArcRect, 90, 90);
    doneLabelPath.lineTo(isDoneColorRect.bottomLeft() + QPoint(0, -arcLength));

    QRect isDoneArcRect2(isDoneColorRect.bottomLeft() + QPoint(0, -arcLength*2),
                         isDoneColorRect.bottomRight() + QPoint(arcLength, 0));
    doneLabelPath.arcTo(isDoneArcRect2, 180, 90);
    doneLabelPath.lineTo(isDoneColorRect.topRight());
    if (itemDetail.isDone()) {
        painter->fillPath(doneLabelPath, QColor("#90EE90"));
    } else {
        painter->fillPath(doneLabelPath, Qt::gray);
    }

    // draw selection item's border
    if (option.state & QStyle::State_Selected) {
        painter->setPen(QPen(option.palette.color(QPalette::Highlight), 4));
        painter->drawRect(option.rect);
    }
}

QSize ItemListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(400, 100);
}