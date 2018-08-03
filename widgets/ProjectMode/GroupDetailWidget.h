#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include "../../data/DataCenter.h"
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
    QColor currentColor;
    todo::DataCenter dataCenter;

    // ui change between view and edit mode
    void changeReadOnly(bool readOnly);
    bool readOnlyFlag;

    QString rootGroupID;
    QString directGroupID;

    void connectModifiedSignal();
private slots:
    void item_modified();
    void changeToViewMode();
    void changeToEditMode();
    void delete_button_clicked();
    void buttonBox_clicked(QAbstractButton *btn);
    void color_button_clicked();
};

#endif // GROUPDETAILWIDGET_H
