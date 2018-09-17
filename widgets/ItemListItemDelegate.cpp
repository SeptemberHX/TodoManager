//
// Created by septemberhx on 5/8/18.
//

#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QVariant>
#include <QSvgRenderer>
#include "ItemListItemDelegate.h"
#include "../config/TodoConfig.h"
#include "../utils/StringUtils.h"
#include "../utils/DrawUtils.h"
#include "../utils/ItemUtils.h"
#include "../utils/ItemGroupUtils.h"
#include "../functions/TaskArchivingTimeRecorder.h"

ItemListItemDelegate::ItemListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {
    this->arcLength = 10;
}

void
ItemListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    todo::ItemAndGroupWrapper itemAndGroupPair = index.data(Qt::UserRole + 1).value<todo::ItemAndGroupWrapper>();

    if (!itemAndGroupPair.isGroup()) {
        this->paintItemDetail(itemAndGroupPair.getItemDetail(), painter, option, index);
    } else {
        this->paintItemGroup(itemAndGroupPair.getItemGroup(), painter, option, index);
    }

    painter->restore();
}

QSize ItemListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(400, 100);
}

void ItemListItemDelegate::paintItemDetail(const todo::ItemDetail &itemDetail, QPainter *painter,
                                           const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // draw background
    painter->fillRect(option.rect, Qt::white);

    // draw done picture
    auto taskState = todo::TaskArchivingTimeRecorder::getInstance()->getTaskArchivingState(itemDetail.getId());
    int itemHeight = option.rect.height();
    int pictureSize = itemHeight / 3;
    QRect donePicRect(option.rect.topRight() - QPoint(pictureSize + 10, -10),
                      QSize(pictureSize, pictureSize));
    if (itemDetail.isDone()) {
        QSvgRenderer doneIconSvgRender(QString(":/icons/done.svg"));
        doneIconSvgRender.render(painter, donePicRect);
    } else if (taskState == todo::TaskArchivingState::PAUSE) {
        QSvgRenderer pauseIconSvgRender(QString(":/icons/pause.svg"));
        pauseIconSvgRender.render(painter, donePicRect);
    } else if (taskState == todo::TaskArchivingState::DOING) {
        QSvgRenderer doingIconSvgRender(QString(":/icons/doing.svg"));
        doingIconSvgRender.render(painter, donePicRect);
    }

    // draw item bottom line
    QPoint lineStartPos = option.rect.bottomLeft() + QPoint(20, 0);
    QPoint lineEndPos = option.rect.bottomRight() + QPoint(-10, 0);
    painter->setPen(QPen(Qt::lightGray, 1));
    painter->drawLine(QLine(lineStartPos, lineEndPos));

    // draw project color
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPainterPath painterPath;
    if (itemDetail.hasRootGroup()) {
        auto projectColorRect = QRect(option.rect.topLeft() + QPoint(0, arcLength * 4), option.rect.bottomLeft()
                                                                                + QPoint(arcLength, -arcLength));
        auto arcRectF = QRectF(projectColorRect.topLeft() + QPoint(-arcLength, 0), projectColorRect.topRight()
                                                                               + QPoint(0, arcLength * 2));
        painterPath.moveTo(arcRectF.center());
        painterPath.arcTo(arcRectF, 0, 90);
        painterPath.closeSubpath();
        painterPath.lineTo(projectColorRect.topLeft());
        painterPath.lineTo(projectColorRect.bottomLeft() + QPoint(0, -arcLength));
        auto arcRectF2 = QRectF(projectColorRect.bottomLeft() + QPoint(-arcLength, -arcLength * 2),
                                projectColorRect.bottomRight());
        painterPath.arcTo(arcRectF2, -90, 90);

        painterPath.lineTo(projectColorRect.topRight() + QPoint(0, 10));
        painterPath.closeSubpath();
        painter->fillPath(painterPath, QBrush(todo::ItemUtils::getRootGroupColor(itemDetail)));
    }

    // draw title
    painter->setFont(QFont("Arias", 14));
    painter->setPen(Qt::black);
    QBrush oldBrush = painter->brush();
    QMargins titleMargins(10, 5, 10, 2);
    int priorityNumWidth = 20;
    int titleSpace = 5;
    int titleWidth =
            option.rect.width() - 2 * arcLength - titleMargins.left() - titleMargins.right() - priorityNumWidth -
            titleSpace;
    int titleHeight = 30;
    auto titleRect = QRect(option.rect.topLeft() + QPoint(titleMargins.left() + arcLength, titleMargins.top()),
                           QSize(titleWidth, titleHeight));
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemDetail.getTitle(), painter->fontMetrics(), titleWidth));

    // draw priority circle
    if (taskState == todo::TaskArchivingState::NOT_START) {
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
        painter->drawText(priorityNumRect, Qt::AlignHCenter | Qt::AlignVCenter,
                          QString::number(itemDetail.getPriority()));
        painter->setBrush(oldBrush);
    }

    // draw tags
    QMargins tagMargins(3, 1, 3, 1);
    int tagSpacing = 5;
    QMargins tagGroupMargins(30, 0, 10, 0);
    QRect tagGroupRect(
            option.rect.left() + arcLength + tagGroupMargins.left(),
            option.rect.top() + titleMargins.top() + titleRect.height() + titleMargins.bottom() +
            tagGroupMargins.top(),
            option.rect.width() - arcLength * 2 - tagGroupMargins.left() - tagGroupMargins.right(),
            20
    );
    QRect availableRect(tagGroupRect);
    int fixedWidth = painter->fontMetrics().width("...");

    painter->setFont(QFont("Arias", 8));
    for (auto const &tag : itemDetail.getTags()) {
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
    QMargins infoMargins(5, 5, 5, 5);
    int infoRectTop = titleMargins.top() + titleRect.height() + titleMargins.bottom()
                  + tagGroupMargins.top() + tagGroupRect.height() + tagGroupMargins.bottom() +
                  infoMargins.top();
    int infoWidth = 100;
    int infoHeight = option.rect.height() - infoRectTop - infoMargins.bottom();
    painter->setFont(QFont("Arias", 8));
    painter->setPen(Qt::gray);
    QRect dateRect(
            option.rect.right() - arcLength - infoMargins.right() - infoWidth,
            option.rect.bottom() - infoMargins.bottom() - infoHeight,
            infoWidth,
            infoHeight / 2
    );
    painter->drawText(dateRect, Qt::AlignLeft | Qt::AlignBottom, itemDetail.getTargetDate().toString("yyyy/MM/dd"));

    // draw schedule time scope
    QRect timeRect(dateRect);
    painter->setPen(Qt::gray);
    timeRect.moveBottom(dateRect.bottom() + infoHeight / 2);
    if (itemDetail.getMode() != todo::ItemMode::SIMPLE) {
        painter->drawText(timeRect, Qt::AlignLeft | Qt::AlignBottom,
                          itemDetail.getFromTime().toString("hh:mm") + "-" +
                          itemDetail.getToTime().toString("hh:mm"));
    }

    // draw description
    QMargins descriptionMargins(20, 5, 5, 5);
    int descriptionRectTop = titleMargins.top() + titleRect.height() + titleMargins.bottom()
                             + tagGroupMargins.top() + tagGroupRect.height() + tagGroupMargins.bottom() +
                             descriptionMargins.top();
    int descriptionHeight = option.rect.height() - descriptionRectTop - descriptionMargins.bottom();
    QRect descriptionRect(
            option.rect.left() + arcLength + descriptionMargins.left(),
            option.rect.bottom() - descriptionMargins.bottom() - descriptionHeight,
            option.rect.width() - arcLength - infoMargins.left() - infoMargins.right() - infoWidth -
            descriptionMargins.right() - descriptionMargins.left() - arcLength,
            descriptionHeight
    );
    painter->setPen(Qt::gray);
    painter->setFont(QFont("Arias", 10));
    painter->drawText(descriptionRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(
                              todo::ItemUtils::getPlainDescription(itemDetail),
                              painter->fontMetrics(),
                              descriptionRect.width()
                              )
                      );

    // draw isDone color label
//    QPainterPath doneLabelPath;
//    QRect isDoneColorRect(option.rect.topRight() + QPoint(-arcLength, arcLength),
//                          option.rect.bottomRight() + QPoint(0, -arcLength));
//    QRectF isDoneArcRect(isDoneColorRect.topLeft(),
//                         isDoneColorRect.topLeft() + QPoint(arcLength * 2, arcLength * 2));
//    doneLabelPath.moveTo(isDoneArcRect.center());
//    doneLabelPath.arcTo(isDoneArcRect, 90, 90);
//    doneLabelPath.lineTo(isDoneColorRect.bottomLeft() + QPoint(0, -arcLength));
//
//    QRect isDoneArcRect2(isDoneColorRect.bottomLeft() + QPoint(0, -arcLength * 2),
//                         isDoneColorRect.bottomRight() + QPoint(arcLength, 0));
//    doneLabelPath.arcTo(isDoneArcRect2, 180, 90);
//    doneLabelPath.lineTo(isDoneColorRect.topRight());
//    if (itemDetail.isDone()) {
//        painter->fillPath(doneLabelPath, QColor("#90EE90"));
//    } else {
//        painter->fillPath(doneLabelPath, Qt::gray);
//    }

    // draw selection item's border
    if (option.state & QStyle::State_Selected) {
        painter->setPen(QPen(option.palette.color(QPalette::Highlight), 4));
        painter->drawRect(option.rect);
    }
}

void ItemListItemDelegate::paintItemGroup(const todo::ItemGroup &itemGroup, QPainter *painter,
                                          const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->setRenderHint(QPainter::Antialiasing, true);

    // draw background
    painter->fillRect(option.rect, QColor("#EAEAEF"));

    // draw item bottom line
    QPoint lineStartPos = option.rect.bottomLeft() + QPoint(20, 0);
    QPoint lineEndPos = option.rect.bottomRight() + QPoint(-20, 0);
    painter->setPen(QPen(Qt::lightGray, 1));
    painter->drawLine(QLine(lineStartPos, lineEndPos));

    // draw project color
    QPainterPath painterPath;
    {
        auto projectColorRect = QRect(option.rect.topLeft() + QPoint(0, arcLength * 4), option.rect.bottomLeft()
                                                                                + QPoint(arcLength, -arcLength));
        auto arcRectF = QRectF(projectColorRect.topLeft() + QPoint(-arcLength, 0), projectColorRect.topRight()
                                                                               + QPoint(0, arcLength * 2));
        painterPath.moveTo(arcRectF.center());
        painterPath.arcTo(arcRectF, 0, 90);
        painterPath.closeSubpath();
        painterPath.lineTo(projectColorRect.topLeft());
        painterPath.lineTo(projectColorRect.bottomLeft() + QPoint(0, -arcLength));
        auto arcRectF2 = QRectF(projectColorRect.bottomLeft() + QPoint(-arcLength, -arcLength * 2),
                                projectColorRect.bottomRight());
        painterPath.arcTo(arcRectF2, -90, 90);

        painterPath.lineTo(projectColorRect.topRight() + QPoint(0, 10));
        painterPath.closeSubpath();
        if (itemGroup.getType() == todo::ItemGroupType::PROJECT) {
            painter->fillPath(painterPath, QBrush(itemGroup.getColor()));
        } else {
            painter->fillPath(painterPath, todo::ItemUtils::getRootGroupColor(itemGroup));
        }
    }

    // draw task percent
    QSize percentSize(60, 20);
    QMargins percentMargin(5, 10, 5, 5);
    QRect percentRect(option.rect.right() - this->arcLength - percentMargin.right() - percentSize.width(),
                      option.rect.top() + percentMargin.top(),
                      percentSize.width(),
                      percentSize.height()
    );
    todo::ItemGroupOverview overview = todo::ItemGroupUtils::getGroupOverview(itemGroup);
    double percent = overview.getTotalItemDoneCount() * 1.0 / overview.getTotalItemCount();
    todo::DrawUtils::drawRectWithCircle(*painter, QFont("Arial", 8), Qt::white,
            QString("%1\%").arg(QString::number(int(percent * 100))), percentRect, QColor("#665bff"), percent);

    // draw title
    painter->setFont(QFont("Arial", 14));
    painter->setPen(Qt::black);
    QBrush oldBrush = painter->brush();
    QMargins titleMargins(10, 5, 0, 2);
    int priorityNumWidth = 20;
    int titleWidth =
            option.rect.width() - 2 * arcLength - titleMargins.left() - titleMargins.right() - priorityNumWidth
            - percentSize.width() - percentMargin.left() - percentMargin.right();
    int titleHeight = 30;
    auto titleRect = QRect(option.rect.topLeft() + QPoint(titleMargins.left() + arcLength, titleMargins.top()),
                           QSize(titleWidth, titleHeight));
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemGroup.getTitle(), painter->fontMetrics(), titleWidth));

    // draw milestone flag
    if (itemGroup.isMileStone()) {
        painter->setFont(QFont("Arial", 8));
        painter->setPen(Qt::white);
        auto mRect = painter->fontMetrics().boundingRect("Milestone");
        QMargins milestoneMargin(5, 5, 5, 5);
        QRect milestoneRect(option.rect.right() - this->arcLength - milestoneMargin.right() - mRect.width() - 5,
                            titleRect.bottom() + milestoneMargin.top(),
                            mRect.width() + 5,
                            mRect.height() + 5
        );
        painter->fillRect(milestoneRect, "#35b9ab");
        painter->drawText(milestoneRect, Qt::AlignCenter, "Milestone");
    }

    // draw date
    QString fromStr("F: %1");
    QString toStr("T: %1");
    fromStr = fromStr.arg(itemGroup.getFromDate().toString("yyyy/MM/dd"));
    toStr = toStr.arg(itemGroup.getToDate().toString("yyyy/MM/dd"));
    painter->setPen(Qt::gray);
    painter->setFont(QFont("Arial", 8));
    auto fRect = painter->fontMetrics().boundingRect(fromStr);
    auto tRect = painter->fontMetrics().boundingRect(toStr);
    int finalWidth = std::max(fRect.width(), tRect.width());
    QMargins dateLabelMargin(5, 5, 5, 3);
    QRect fromRect(option.rect.right() - this->arcLength - dateLabelMargin.right() - finalWidth,
                   option.rect.bottom() - dateLabelMargin.bottom() - fRect.height() - tRect.height(),
                   fRect.width(),
                   fRect.height()
    );
    painter->drawText(fromRect, Qt::AlignCenter, fromStr);
    QRect toRect(fromRect.left(), fromRect.bottom(), tRect.width(), tRect.height());
    painter->drawText(toRect, Qt::AlignCenter, toStr);

    // draw description
    QMargins descriptionMargins(0, 10, 0, 0);
    painter->setPen(Qt::gray);
    painter->setFont(QFont("Arial", 10));
    int maxHeight = painter->fontMetrics().height() * 2;  // 2 line
    QRect availableRect(QPoint(titleRect.left(), titleRect.bottom() + titleMargins.bottom() + descriptionMargins.top()),
                        toRect.bottomLeft() + QPoint(-dateLabelMargin.left(), 0));
    availableRect.setBottom(availableRect.bottom() - availableRect.height() + maxHeight);
    painter->drawText(availableRect, Qt::TextWordWrap | Qt::TextSingleLine | Qt::AlignTop,
            todo::ItemUtils::getPlainDescription(itemGroup), &availableRect);

    // draw selection item's border
    if (option.state & QStyle::State_Selected) {
        painter->setPen(QPen(option.palette.color(QPalette::Highlight), 4));
        painter->drawRect(option.rect);
    }
}
