//
// Created by septemberhx on 4/24/18.
//

#ifndef TODOMANAGER_ITEMTAG_H
#define TODOMANAGER_ITEMTAG_H

#include <QString>
#include <QHash>
#include <QColor>
#include <QMetaType>

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

    friend std::ostream &operator<<(std::ostream &os, const ItemTag &itemTag);

    friend QDataStream&operator<<(QDataStream &os, const ItemTag &itemTag);

    friend QDataStream&operator>>(QDataStream &os, ItemTag &itemTag);
private:
    QString name;
    QColor color;
};

inline uint qHash(const ItemTag &tag) {
    return qHash(tag.getName());
}
}

Q_DECLARE_METATYPE(todo::ItemTag);

#endif //TODOMANAGER_ITEMTAG_H
