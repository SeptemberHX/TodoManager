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
    this->initType2NameMap();
}

QList<CommonActionType> CommonAction::getTagActions(bool multiItems) {
    QList<CommonActionType> resultList;
    resultList.append(CommonActionType::TAG_REMOVE_ONLY);
    resultList.append(CommonActionType::TAG_REMOVE_COMPLETE);
    return resultList;
}

QList<CommonActionType> CommonAction::getItemDetailActions(bool multiItems) {
    QList<CommonActionType> resultList;
    resultList.append(CommonActionType::ITEMDETAIL_ASSIGN_TO_PROJECT);
    return resultList;
}

void CommonAction::initType2NameMap() {
    this->actionType2Name[CommonActionType::TAG_REMOVE_ONLY] = "Remove tag only";
    this->actionType2Name[CommonActionType::TAG_REMOVE_COMPLETE] = "Remove tag and tasks";
    this->actionType2Name[CommonActionType::ITEMDETAIL_ASSIGN_TO_PROJECT] = "Assign to project";
}

QString CommonAction::getActionName(const CommonActionType &actionType) {
    return this->actionType2Name[actionType];
}

CommonActionType CommonAction::getActionType(const QString &name) {
    foreach (auto const &actionType, this->actionType2Name.keys()) {
        if (this->actionType2Name[actionType] == name) {
            return actionType;
        }
    }
    return CommonActionType::ERROR;
}
