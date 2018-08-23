#ifndef ITEMDETAILWIDGET_H
#define ITEMDETAILWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QStandardItemModel>
#include "../data/DataCenter.h"
#include "../core/ItemDetail.h"
#include "../core/ItemTag.h"
#include "tagwidget.h"
#include "MyTextEdit.h"

namespace Ui {
class ItemDetailWidget;
}

class ItemDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemDetailWidget(QWidget *parent = 0);
    ~ItemDetailWidget();
    void loadItemDetail(const todo::ItemDetail &itemDetail);
    todo::ItemDetail collectItemDetail();
    bool checkIfInputLegal();

    enum ItemDetailWidgetMode {
        EDIT,
        VIEW
    };

    void setReadOnly(bool isReadOnly);
    bool isEditing() const;

signals:
    void ui_item_edited(const todo::ItemDetail &newItemDetail);  // info shown on list changed
    void saveButtonClicked(const todo::ItemDetail &newItemDetail);
    void markDoneClicked(bool isDone);
    void deleteButtonClicked();
    void jumpTo(const QString &itemID);
    void tagClicked(const QString &tagID);

public slots:
    void modeButtonToggled();

private:
    todo::DataCenter dataCenter;

    void changeToSimpleMode();
    void changeToScheduleMode();
    void changeToRecursionMode();

    void changeToEditMode();
    void changeToViewMode();

    void addTagButton(const todo::ItemTag &tag);

    void reloadCurrItemDetail();

    void saveBtn_clicked();
    void cancelBtn_clicked();

    QString directGroupID;
    QString rootGroupID;

    Ui::ItemDetailWidget *ui;
    QButtonGroup *itemModeBtnGroup;
    TagWidget *tagWidget;

    todo::ItemMode widgetMode;
    ItemDetailWidgetMode editMode;

    todo::ItemDetail *currItemPtr;
    MyTextEdit *descriptionTextEdit;

    QStandardItemModel *itemModel;

private slots:
    void editBtn_clicked();
    void scheduleTime_changed();
    void uiItemEdited();
    void buttonBox_clicked(QAbstractButton *btn);
    void markDoneBtn_clicked();
    void deleteButton_clicked();
    void try_jump_to(const QString &itemID);
    void deleteRelationButton_clicked();
    void editRelationButton_clicked();
    void item_tag_clicked(const QString &itemID);
};

#endif // ITEMDETAILWIDGET_H
