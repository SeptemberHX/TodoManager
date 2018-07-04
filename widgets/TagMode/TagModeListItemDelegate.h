//
// Created by septemberhx on 7/4/18.
//

#ifndef TODOMANAGER_TAGMODELISTITEMDELEGATE_H
#define TODOMANAGER_TAGMODELISTITEMDELEGATE_H

#include <QStyledItemDelegate>

class TagModeListItemDelegate : public QStyledItemDelegate {
public:
    TagModeListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString elideText(const QString &str, const QFontMetrics &fontMetrics, int maxWidth) const;
};


#endif //TODOMANAGER_TAGMODELISTITEMDELEGATE_H
