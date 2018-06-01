//
// Created by septemberhx on 4/26/18.
//

#include "ItemTagUtils.h"
#include <QMap>

todo::ItemTag todo::ItemTagUtils::convertStr2Tag(const QString &tagStr) {
    static QMap<QString, QColor> cache;

    todo::ItemTag tagItem(tagStr);
    QColor color;
    if (cache.find(tagStr) != cache.end()) {
        color = cache[tagStr];
    } else {
        color = QColor::fromHsl(rand()%360,rand()%256,rand()%200);
        cache.insert(tagStr, color);
    }

    tagItem.setColor(color);
    return tagItem;
}

QList<todo::ItemTag> todo::ItemTagUtils::getAllTags() {
    QList<ItemTag> tags;
    // TODO: get all tags
//    tags.append("test");
//    tags.append("this");
//    tags.append("the");
//    tags.append("theory");
//    tags.append("group");

    return tags;
}

void todo::ItemTagUtils::addNewTag(const todo::ItemTag &tag) {
    // TODO: add new tag
}

void todo::ItemTagUtils::removeTag(const todo::ItemTag &tag) {
    // TODO: remove tag
}
