//
// Created by septemberhx on 8/17/18.
//

#ifndef TODOMANAGER_STICKYNOTE_H
#define TODOMANAGER_STICKYNOTE_H

#include "StickyNoteDao.h"
#include "../utils/StringUtils.h"

namespace todo {

class StickyNote {
public:
    StickyNote();

    const QString &getId() const;

    void setId(const QString &id);

    bool isShown() const;

    void setShown(bool shown);

    const QPoint &getPos() const;

    void setPos(const QPoint &pos);

    StickyNoteType getType() const;

    void setType(StickyNoteType type);

    const QDateTime &getCreatedTime() const;

    void setCreatedTime(const QDateTime &createdTime);

    const QDateTime &getLastUpdatedTime() const;

    void setLastUpdatedTime(const QDateTime &lastUpdatedTime);

    const QColor &getFontColor() const;

    void setFontColor(const QColor &fontColor);

    const QColor &getBackgroundColor() const;

    void setBackgroundColor(const QColor &backgroundColor);

    const QString &getName() const;

    void setName(const QString &name);
private:
    StickyNoteDao dao;
};

}

#endif //TODOMANAGER_STICKYNOTE_H
