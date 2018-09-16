//
// Created by septemberhx on 9/16/18.
//

#ifndef TODOMANAGER_STICKYNOTECONFIG_H
#define TODOMANAGER_STICKYNOTECONFIG_H

#include <QString>
#include <QDate>
#include <QList>

namespace todo {

class StickyNoteConfig {
public:
    StickyNoteConfig(const QString &stickyNoteId);

    void setDate(const QDate &fromDate, const QDate &toDate, bool flag = true);
    void setTagId(const QString &tagId, bool flag = true);
    void setProjectId(const QString &projectId, bool flag = true);

    void removeDate();
    void removeTagId();
    void removeProjectId();

    bool isSpecificDate() const;

    bool isSpecificTag() const;

    bool isSpecificProject() const;

    const QDate &getFromDate() const;

    const QDate &getToDate() const;

    const QString &getTagId() const;

    const QString &getProjectId() const;

    static QString DIR;

private:
    QString stickyNoteId;

    bool specificDate;
    QDate fromDate;
    QDate toDate;

    bool specificTag;
    QString tagId;

    bool specificProject;
    QString projectId;
};

}

#endif //TODOMANAGER_STICKYNOTECONFIG_H
