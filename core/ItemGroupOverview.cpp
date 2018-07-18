//
// Created by septemberhx on 7/18/18.
//

#include "ItemGroupOverview.h"

int todo::ItemGroupOverview::getSubGroupCount() const {
    return subGroupCount;
}

void todo::ItemGroupOverview::setSubGroupCount(int subGroupCount) {
    ItemGroupOverview::subGroupCount = subGroupCount;
}

int todo::ItemGroupOverview::getSubGroupDoneCount() const {
    return subGroupDoneCount;
}

void todo::ItemGroupOverview::setSubGroupDoneCount(int subGroupDoneCount) {
    ItemGroupOverview::subGroupDoneCount = subGroupDoneCount;
}

int todo::ItemGroupOverview::getSubItemCount() const {
    return subItemCount;
}

void todo::ItemGroupOverview::setSubItemCount(int subItemCount) {
    ItemGroupOverview::subItemCount = subItemCount;
}

int todo::ItemGroupOverview::getSubItemDoneCount() const {
    return subItemDoneCount;
}

void todo::ItemGroupOverview::setSubItemDoneCount(int subItemDoneCount) {
    ItemGroupOverview::subItemDoneCount = subItemDoneCount;
}

int todo::ItemGroupOverview::getTotalItemCount() const {
    return totalItemCount;
}

void todo::ItemGroupOverview::setTotalItemCount(int totalItemCount) {
    ItemGroupOverview::totalItemCount = totalItemCount;
}

int todo::ItemGroupOverview::getTotalItemDoneCount() const {
    return totalItemDoneCount;
}

void todo::ItemGroupOverview::setTotalItemDoneCount(int totalItemDoneCount) {
    ItemGroupOverview::totalItemDoneCount = totalItemDoneCount;
}

bool todo::ItemGroupOverview::isDone() const {
    return this->subGroupCount == this->subGroupDoneCount && this->subItemCount == this->subItemDoneCount;
}
