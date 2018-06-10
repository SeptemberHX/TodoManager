//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_UICONFIG_H
#define TODOMANAGER_UICONFIG_H

#include <QColor>
#include <QMap>

namespace todo {

class UIConfig {
public:
    UIConfig();

    const QMap<int, QColor> &getListViewPrioriyBackgroundColorMap() const;

    void setListViewPrioriyBackgroundColorMap(const QMap<int, QColor> &listViewPrioriyBackgroundColorMap);

private:
    // list view ui config
    QMap<int, QColor> listViewPrioriyBackgroundColorMap;
};

}

#endif //TODOMANAGER_UICONFIG_H
