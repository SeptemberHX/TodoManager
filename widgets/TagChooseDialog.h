#ifndef TAGCHOOSEDIALOG_H
#define TAGCHOOSEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QPair>
#include <QAbstractButton>
#include "../data/DataCenter.h"

namespace Ui {
class TagChooseDialog;
}

class TagChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagChooseDialog(QWidget *parent = nullptr);
    ~TagChooseDialog();

    const QPair<QString, QString> &getSelectedTagNameAndId() const;

private:
    Ui::TagChooseDialog *ui;
    todo::DataCenter dataCenter;
    QPair<QString, QString> selectedTagNameAndId;

    QStandardItemModel *itemModel;

private slots:
    void listItem_clicked();
    void buttonBox_clicked(QAbstractButton *button);
};

#endif // TAGCHOOSEDIALOG_H
