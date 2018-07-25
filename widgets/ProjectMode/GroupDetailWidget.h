#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>
#include "../../core/ItemGroup.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace Ui {
class GroupDetailWidget;
}

class GroupDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupDetailWidget(QWidget *parent = 0);
    ~GroupDetailWidget();
    void loadItemGroup(const todo::ItemGroup &itemGroup);
signals:
    void itemModified(const todo::ItemAndGroupWrapper &itemWrapper);
private:
    Ui::GroupDetailWidget *ui;
    todo::ItemGroup rawItemGroup;  // backup for the original itemGroup

    /**
     * Only collect current group's details.
     * Will not collect relationship between it and items.
     * @return
     */
    todo::ItemGroup collectData() const;

    // ui change between view and edit mode
    void changeReadOnly(bool readOnly) const;

    void connectModifiedSignal();
private slots:
    void item_modified();
    void changeToViewMode();
    void changeToEditMode();
};

#endif // GROUPDETAILWIDGET_H
