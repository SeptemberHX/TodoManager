//
// Created by septemberhx on 4/24/18.
//

#ifndef TODOMANAGER_ITEMTAG_H
#define TODOMANAGER_ITEMTAG_H

#include <QString>
#include <QHash>
#include <QColor>

namespace todo {

class ItemTag {
public:
    explicit ItemTag(const QString &name);
    explicit ItemTag(const char *str);
    ItemTag(const ItemTag &tag);
    ItemTag();

    QColor getColor() const;
    void setColor(QColor color);

    bool operator==(const ItemTag &o) const;
    bool operator!=(const ItemTag &o) const;

    QString getName() const;
    QString getId() const;
private:
    QString name;
    QColor color;
};

inline uint qHash(const ItemTag &tag) {
    return qHash(tag.getName());
}
}

#endif //TODOMANAGER_ITEMTAG_H
