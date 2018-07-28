#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>
#include <QAbstractButton>
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
    bool isEditing();

    /**
     * Only collect current group's details.
     * Will not collect relationship between it and items.
     * @return
     */
    todo::ItemGroup collectData() const;
signals:
    void itemModified(const todo::ItemAndGroupWrapper &itemWrapper);
    void deleteButtonClicked();
    void saveActionTriggered(const todo::ItemAndGroupWrapper &itemWrapper);
private:
    Ui::GroupDetailWidget *ui;
    todo::ItemGroup rawItemGroup;  // backup for the original itemGroup

    // ui change between view and edit mode
    void changeReadOnly(bool readOnly);
    bool readOnlyFlag;

    void connectModifiedSignal();
private slots:
    void item_modified();
    void changeToViewMode();
    void changeToEditMode();
    void delete_button_clicked();
    void buttonBox_clicked(QAbstractButton *btn);
};

#endif // GROUPDETAILWIDGET_H
