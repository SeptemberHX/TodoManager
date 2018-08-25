//
// Created by septemberhx on 7/27/18.
//

#include "ItemUtils.h"
#include <QUuid>
#include <QTextDocumentFragment>

todo::DataCenter todo::ItemUtils::dataCenter;

todo::ItemDetail
todo::ItemUtils::generateNewItemDetail(const QString &title, const QString &rootItemID, const QString &directItemID) {
    todo::ItemDetail newItemDetail(title);
    newItemDetail.setRootGroupID(rootItemID);
    newItemDetail.setDirectGroupID(directItemID);
    newItemDetail.setCreatedTime(QDateTime::currentDateTime());
    newItemDetail.setLastUpdatedTime(QDateTime::currentDateTime());
    newItemDetail.setDone(false);
    newItemDetail.setMode(ItemMode::SIMPLE);
    return newItemDetail;
}

QColor todo::ItemUtils::getRootGroupColor(const todo::ItemAndGroupWrapper &wrapper) {
    if (wrapper.hasRootGroup()) {
        return ItemUtils::dataCenter.selectItemGroupByID(wrapper.getRootGroupID())[0].getColor();
    } else {
        return Qt::transparent;
    }
}

QString todo::ItemUtils::generateToolTip(const todo::ItemAndGroupWrapper &wrapper) {
    QString tooltipStr;
    tooltipStr += wrapper.getTitle();

    QString plainDescritpion = ItemUtils::getPlainDescription(wrapper);
    if (!plainDescritpion.isEmpty()) {
        tooltipStr += "\n\n" + plainDescritpion;
    }

    return tooltipStr;
}

QString todo::ItemUtils::getPlainDescription(const todo::ItemAndGroupWrapper &wrapper) {
    return QTextDocumentFragment::fromHtml(wrapper.getDescription()).toPlainText();
}
