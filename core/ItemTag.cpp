//
// Created by septemberhx on 4/24/18.
//

#include "ItemTag.h"

todo::ItemTag::ItemTag(const QString &name)
    : name(name), color(QColor(Qt::gray)) {

}

QString todo::ItemTag::getName() const {
    return this->name;
}

bool todo::ItemTag::operator==(const todo::ItemTag &o) const {
    return this->getName() == o.getName();
}

todo::ItemTag::ItemTag(const char *str)
    : color(QColor(Qt::gray)) {
    this->name = QString(str);
}

bool todo::ItemTag::operator!=(const todo::ItemTag &o) const {
    return !this->operator==(o);
}

QColor todo::ItemTag::getColor() const {
    return color;
}

void todo::ItemTag::setColor(QColor color) {
    ItemTag::color = color;
}

QString todo::ItemTag::getId() const {
    return this->getName();
}

todo::ItemTag::ItemTag(const todo::ItemTag &tag) {
    this->name = tag.getName();
    this->color = tag.getColor();
}

todo::ItemTag::ItemTag() {
    this->name = "#323232";
    this->color = "default";
}
