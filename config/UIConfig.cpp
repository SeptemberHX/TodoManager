//
// Created by septemberhx on 6/10/18.
//

#include "UIConfig.h"

const QMap<int, QColor> &todo::UIConfig::getListViewPrioriyBackgroundColorMap() const {
    return listViewPrioriyBackgroundColorMap;
}

void todo::UIConfig::setListViewPrioriyBackgroundColorMap(const QMap<int, QColor> &listViewPrioriyBackgroundColorMap) {
    UIConfig::listViewPrioriyBackgroundColorMap = listViewPrioriyBackgroundColorMap;
}

todo::UIConfig::UIConfig() :
    defaultFontMetrics(defaultFont)
{
    this->listViewPrioriyBackgroundColorMap[1] = QColor("#d84c31");
    this->listViewPrioriyBackgroundColorMap[2] = QColor("#ffd202");
    this->listViewPrioriyBackgroundColorMap[3] = QColor("#478fe1");
    this->listViewPrioriyBackgroundColorMap[4] = QColor("#ae88f2");
    this->listViewPrioriyBackgroundColorMap[5] = QColor("#aaaa7f");

    this->itemListTitleFont = defaultFont;
    this->itemListTitleFont.setPointSize(this->itemListTitleFont.pointSize() + 3);
}

const QFont &todo::UIConfig::getDefaultFont() const {
    return this->defaultFont;
}

int todo::UIConfig::getFontHeight() const {
    return this->defaultFontMetrics.height();
}

const QFont &todo::UIConfig::getItemListTitleFont() const {
    return this->itemListTitleFont;
}
