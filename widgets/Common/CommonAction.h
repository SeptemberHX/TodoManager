//
// Created by septemberhx on 8/7/18.
//

#ifndef TODOMANAGER_COMMONACTION_H
#define TODOMANAGER_COMMONACTION_H


#include <QAction>
#include <QMap>

enum CommonActionType {
    TAG_REMOVE_ONLY,
    TAG_REMOVE_COMPLETE,
    ERROR
};

class CommonAction {
public:
    static CommonAction *getInstance();
    QList<QAction*> getTagActions(bool multiItems);
    CommonActionType getActionType(QAction *actionPtr);
private:
    static CommonAction *instancePtr;
    CommonAction();
    QMap<CommonActionType, QAction*> type2ActionPtrMap;
};


#endif //TODOMANAGER_COMMONACTION_H
