//
// Created by septemberhx on 7/18/18.
//

#ifndef TODOMANAGER_ITEMGROUPOVERVIEW_H
#define TODOMANAGER_ITEMGROUPOVERVIEW_H

namespace todo {

/**
 * Overview of a given ItemGroup
 * Such total items, total sub group, etc.
 */
class ItemGroupOverview {
public:
    int getSubGroupCount() const;

    void setSubGroupCount(int subGroupCount);

    int getSubGroupDoneCount() const;

    void setSubGroupDoneCount(int subGroupDoneCount);

    int getSubItemCount() const;

    void setSubItemCount(int subItemCount);

    int getSubItemDoneCount() const;

    void setSubItemDoneCount(int subItemDoneCount);

    int getTotalItemCount() const;

    void setTotalItemCount(int totalItemCount);

    int getTotalItemDoneCount() const;

    void setTotalItemDoneCount(int totalItemDoneCount);

    bool isDone() const;

private:
    int subGroupCount;  // only dir-child level
    int subGroupDoneCount;  // only dir-child level
    int subItemCount;  // only dir-child level
    int subItemDoneCount; // only dir-child level

    int totalItemCount;  // all items in this group, including items of sub-group.
    int totalItemDoneCount;  // all items in this group, including items of sub-group.
};

}

#endif //TODOMANAGER_ITEMGROUPOVERVIEW_H
