#ifndef ITEMADDTAGWIDGET_H
#define ITEMADDTAGWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QCompleter>
#include <QPushButton>
#include <QDialog>
#include "../utils/ItemTagUtils.h"
#include "../data/DataCenter.h"

namespace Ui {
class ItemAddTagWidget;
}

class ItemAddTagWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ItemAddTagWidget(QWidget *parent = 0);
    ~ItemAddTagWidget();

    bool isAcceptFlag() const;

    const todo::ItemTag &getResultTag() const;

signals:
    void tagSelected(const todo::ItemTag &tag);

private:
    void setColorButtonColor(const QColor &color);

    Ui::ItemAddTagWidget *ui;
    QCompleter *tagCompleter;
    QList<todo::ItemTag> currTagLists;
    QMap<QString, todo::ItemTag> currTagMap;
    todo::DataCenter dataCenter;

    bool acceptFlag;
    todo::ItemTag resultTag;
    QColor tagColor;

private slots:
    void buttonBox_pressed(QAbstractButton *btn);
    void tagName_changed();
    void colorButton_pressed();
};

#endif // ITEMADDTAGWIDGET_H
