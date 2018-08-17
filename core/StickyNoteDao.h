//
// Created by septemberhx on 8/17/18.
//

#ifndef TODOMANAGER_STICKYNOTEDAO_H
#define TODOMANAGER_STICKYNOTEDAO_H

#include <QString>
#include <QPoint>
#include <QDateTime>
#include <QColor>

namespace todo {

enum StickyNoteType {
    DAILY_STICKY,
    TAG_STICKY,
    PROJECT_STICKY,
    CUSTOM_STICKY
};

class StickyNoteDao {
public:
    StickyNoteDao() = default;

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
    QString id;
    QString name;
    bool shown;
    QPoint pos;
    StickyNoteType type;
    QDateTime createdTime;
    QDateTime lastUpdatedTime;
    QColor fontColor;
    QColor backgroundColor;
};

}


#endif //TODOMANAGER_STICKYNOTEDAO_H
