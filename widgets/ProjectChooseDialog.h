#ifndef PROJECTCHOOSEDIALOG_H
#define PROJECTCHOOSEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "../data/DataCenter.h"

namespace Ui {
class ProjectChooseDialog;
}

class ProjectChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectChooseDialog(QWidget *parent = 0);
    ~ProjectChooseDialog();

    const QPair<QString, QString> &getSelectedProjectIDPair() const;

private:
    Ui::ProjectChooseDialog *ui;
    todo::DataCenter dataCenter;
    QStandardItemModel *itemModel;
    QMap<QStandardItem*, QString> itemPtr2IDMap;
    QPair<QString, QString> selectedProjectIDPair;

    void loadProjectTree();
    void buildProjectTree(QStandardItem *parentItem, const QList<QString> &childrenID);

private slots:
    void treeItem_clicked(const QModelIndex &index);
};

#endif // PROJECTCHOOSEDIALOG_H
