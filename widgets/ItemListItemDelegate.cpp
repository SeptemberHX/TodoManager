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

ItemListItemDelegate::ItemListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    auto defaultFont = todo::TodoConfig::getInstance()->getUiConfig().getDefaultFont();
    this->titleFont = todo::TodoConfig::getInstance()->getUiConfig().getItemListTitleFont();

    auto smallFont = defaultFont;
    smallFont.setPointSize(defaultFont.pointSize() - 1);
    this->descriptionFont = smallFont;
    this->infoFont = smallFont;
    this->tagFont = smallFont;

    this->titleMargins = QMargins(10, 5, 10, 2);
    this->tagGroupMargins = QMargins(30, 0, 10, 0);
    this->infoMargins = QMargins(2, 5, 5, 2);
    this->descriptionMargins = QMargins(20, 5, 5, 5);

    QFontMetrics infoMetrics(this->infoFont);
    this->percentWidth = infoMetrics.width("__100%__");
    this->percentHeight = infoMetrics.height() + 4;

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
    QFontMetrics titleFontMetrics(this->titleFont);
    QFontMetrics tagFontMetrics(this->tagFont);
    QFontMetrics descriptionMetrics(this->descriptionFont);
    QFontMetrics infoMetrics(this->infoFont);

    int height = this->titleMargins.top() + titleFontMetrics.height() + this->titleMargins.bottom()
                    + this->tagGroupMargins.top() + tagFontMetrics.height() + this->tagGroupMargins.bottom()
                    + this->infoMargins.top() + infoMetrics.height() * 2 + this->infoMargins.bottom();

    return QSize(ItemListItemDelegate::getDefaultWidth(), height);
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
    int defaultFontHeight = todo::TodoConfig::getInstance()->getUiConfig().getFontHeight();

    painter->setFont(this->titleFont);
    painter->setPen(Qt::black);
    QBrush oldBrush = painter->brush();
    int priorityNumWidth = defaultFontHeight;
    int titleSpace = 5;
    int titleWidth =
            option.rect.width() - 2 * arcLength - titleMargins.left() - titleMargins.right() - priorityNumWidth -
            titleSpace;

    int titleHeight = defaultFontHeight;
    auto titleRect = QRect(option.rect.topLeft() + QPoint(titleMargins.left() + arcLength, titleMargins.top()),
                           QSize(titleWidth, titleHeight));
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemDetail.getTitle(), painter->fontMetrics(), titleWidth));

    // draw priority circle
    if (taskState == todo::TaskArchivingState::NOT_START) {
        painter->setFont(this->titleFont);
        QRect priorityNumRect(titleRect.right() + titleSpace, titleRect.top(), priorityNumWidth, priorityNumWidth);
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
    QRect tagGroupRect(
            option.rect.left() + arcLength + tagGroupMargins.left(),
            option.rect.top() + titleMargins.top() + titleRect.height() + titleMargins.bottom() +
            tagGroupMargins.top(),
            option.rect.width() - arcLength * 2 - tagGroupMargins.left() - tagGroupMargins.right(),
            defaultFontHeight
    );
    QRect availableRect(tagGroupRect);
    int fixedWidth = painter->fontMetrics().width("...");

    painter->setFont(this->tagFont);
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
    int infoWidth = 100;
    int infoHeight = defaultFontHeight * 2;
    int infoRectTop = tagGroupRect.bottom() + tagGroupMargins.bottom() + infoMargins.top();
    painter->setFont(this->infoFont);
    painter->setPen(Qt::gray);
    QRect dateRect(
            option.rect.right() - arcLength - infoMargins.right() - infoWidth,
            infoRectTop,
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
    int descriptionHeight = timeRect.bottom() - infoRectTop - descriptionMargins.top() - descriptionMargins.bottom();
    QRect descriptionRect(
            option.rect.left() + arcLength + descriptionMargins.left(),
            infoRectTop,
            option.rect.width() - arcLength - infoMargins.left() - infoMargins.right() - infoWidth -
                    descriptionMargins.right() - descriptionMargins.left() - arcLength,
            descriptionHeight
    );
    painter->setPen(Qt::gray);
    painter->setFont(this->descriptionFont);
    painter->drawText(descriptionRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(
                              todo::ItemUtils::getPlainDescription(itemDetail),
                              painter->fontMetrics(),
                              descriptionRect.width()
                              )
                      );

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
    QSize percentSize(this->percentWidth, this->percentHeight);
    QMargins percentMargin(5, 10, 5, 5);
    QRect percentRect(option.rect.right() - this->arcLength - percentMargin.right() - percentSize.width(),
                      option.rect.top() + percentMargin.top(),
                      percentSize.width(),
                      percentSize.height()
    );
    todo::ItemGroupOverview overview = todo::ItemGroupUtils::getGroupOverview(itemGroup);
    double percent = overview.getTotalItemDoneCount() * 1.0 / overview.getTotalItemCount();
    todo::DrawUtils::drawRectWithCircle(*painter, this->tagFont, Qt::white,
            QString("%1\%").arg(QString::number(int(percent * 100))), percentRect, QColor("#665bff"), percent);

    // draw title
    painter->setFont(this->titleFont);
    painter->setPen(Qt::black);
    QBrush oldBrush = painter->brush();
    int priorityNumWidth = 20;
    int titleWidth =
            option.rect.width() - 2 * arcLength - titleMargins.left() - titleMargins.right() - priorityNumWidth
            - percentSize.width() - percentMargin.left() - percentMargin.right();
    int titleHeight = painter->fontMetrics().height();
    auto titleRect = QRect(option.rect.topLeft() + QPoint(titleMargins.left() + arcLength, titleMargins.top()),
                           QSize(titleWidth, titleHeight));
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                      todo::StringUtils::elideText(itemGroup.getTitle(), painter->fontMetrics(), titleWidth));

    // draw milestone flag
    if (itemGroup.isMileStone()) {
        painter->setFont(this->tagFont);
        painter->setPen(Qt::white);
        auto mRect = painter->fontMetrics().boundingRect("Milestone");
        QMargins milestoneMargin(5, 2, 5, 0);
        QRect milestoneRect(option.rect.right() - this->arcLength - milestoneMargin.right() - mRect.width() - 5,
                            percentRect.bottom() + milestoneMargin.top(),
                            mRect.width() + 2,
                            mRect.height() + 2
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
    painter->setFont(this->infoFont);
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
    painter->setPen(Qt::gray);
    painter->setFont(this->descriptionFont);
    int maxHeight = painter->fontMetrics().height() * 2 + 2;  // 2 line
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

int ItemListItemDelegate::getDefaultWidth() {
    QFontMetrics titleFontMetrics(todo::TodoConfig::getInstance()->getUiConfig().getItemListTitleFont());
    return titleFontMetrics.width("一") * 24;
}
