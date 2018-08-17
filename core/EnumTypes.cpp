//
// Created by septemberhx on 8/17/18.
//

#include "EnumTypes.h"

const QString todo::StickyNoteType::PROJECT("Project");
const QString todo::StickyNoteType::DAILY("Daily");
const QString todo::StickyNoteType::TAG("Tag");
const QString todo::StickyNoteType::CUSTOM("Custom");
QMap<int, QString> todo::StickyNoteType::type2StrMap = todo::StickyNoteType::generateType2StrMap();

todo::StickyNoteType::StickyNoteType(const QString &str) {
    this->typeNum = this->fromStr2TypeNum(str);
}

todo::StickyNoteType::StickyNoteType(int typeNum) {
    this->typeNum = typeNum;
}

QMap<int, QString> todo::StickyNoteType::generateType2StrMap() {
    QMap<int, QString> resultMap;
    map_init(resultMap)
            (0, StickyNoteType::DAILY)
            (1, StickyNoteType::TAG)
            (2, StickyNoteType::PROJECT)
            (3, StickyNoteType::CUSTOM)
    ;
    return resultMap;
}

int todo::StickyNoteType::fromStr2TypeNum(const QString &str) const {
    int result = -1;
    foreach (auto const &typeNum, StickyNoteType::type2StrMap.keys()) {
        if (StickyNoteType::type2StrMap[typeNum] == str) {
            result = typeNum;
            break;
        }
    }
    return result;
}

bool todo::StickyNoteType::operator==(const todo::StickyNoteType &other) const {
    return this->typeNum == other.typeNum;
}

bool todo::StickyNoteType::operator==(const QString &str) const {
    return this->typeNum == this->fromStr2TypeNum(str);
}

int todo::StickyNoteType::toTypeNum() const {
    return this->typeNum;
}

QString todo::StickyNoteType::toString() const {
    return this->type2StrMap[this->typeNum];
}

todo::StickyNoteType::StickyNoteType() {
    this->typeNum = -1;
}
