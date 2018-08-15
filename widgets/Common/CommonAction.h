//
// Created by septemberhx on 8/7/18.
//

#ifndef TODOMANAGER_COMMONACTION_H
#define TODOMANAGER_COMMONACTION_H


#include <QAction>
#include <QMap>

enum CommonActionType {
    TAG_REMOVE_ONLY = 0,
    TAG_REMOVE_COMPLETE,
    ITEMDETAIL_ASSIGN_TO_PROJECT,
    ERROR
};

class CommonAction {
public:
    static CommonAction *getInstance();
    QString getActionName(const CommonActionType &actionType);
    CommonActionType getActionType(const QString &name);
    QList<CommonActionType> getTagActions(bool multiItems);
    QList<CommonActionType> getItemDetailActions(bool multiItems);
private:
    static CommonAction *instancePtr;
    CommonAction();
    void initType2NameMap();
    QMap<CommonActionType, QString> actionType2Name;
};


#endif //TODOMANAGER_COMMONACTION_H
