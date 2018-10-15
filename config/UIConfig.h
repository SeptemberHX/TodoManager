//
// Created by septemberhx on 6/10/18.
//

#ifndef TODOMANAGER_UICONFIG_H
#define TODOMANAGER_UICONFIG_H

#include <QColor>
#include <QMap>
#include <QFont>
#include <QFontMetrics>

namespace todo {

class UIConfig {
public:
    UIConfig();

    const QMap<int, QColor> &getListViewPrioriyBackgroundColorMap() const;

    void setListViewPrioriyBackgroundColorMap(const QMap<int, QColor> &listViewPrioriyBackgroundColorMap);

    const QFont &getDefaultFont() const;

    int getFontHeight() const;

private:
    // list view ui config
    QMap<int, QColor> listViewPrioriyBackgroundColorMap;

    QFont defaultFont;

    QFontMetrics defaultFontMetrics;
};

}

#endif //TODOMANAGER_UICONFIG_H
