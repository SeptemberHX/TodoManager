//
// Created by septemberhx on 8/7/18.
//

#include "CommonAction.h"

CommonAction *CommonAction::instancePtr = nullptr;

CommonAction *CommonAction::getInstance() {
    if (CommonAction::instancePtr == nullptr) {
        CommonAction::instancePtr = new CommonAction();
    }
    return CommonAction::instancePtr;
}

CommonAction::CommonAction() {
    this->type2ActionPtrMap[CommonActionType::TAG_REMOVE_ONLY] = new QAction("Remove tag only");
    this->type2ActionPtrMap[CommonActionType::TAG_REMOVE_COMPLETE] = new QAction("Remove tag and tasks");
}

QList<QAction *> CommonAction::getTagActions(bool multiItems) {
    QList<QAction*> resultList;
    resultList.append(this->type2ActionPtrMap[CommonActionType::TAG_REMOVE_ONLY]);
    resultList.append(this->type2ActionPtrMap[CommonActionType::TAG_REMOVE_COMPLETE]);
    return resultList;
}

CommonActionType CommonAction::getActionType(QAction *actionPtr) {
    return this->type2ActionPtrMap.key(actionPtr, CommonActionType::ERROR);
}
