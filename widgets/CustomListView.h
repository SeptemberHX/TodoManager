//
// Created by septemberhx on 7/12/18.
//

#ifndef TODOMANAGER_CUSTOMLISTVIEW_H
#define TODOMANAGER_CUSTOMLISTVIEW_H


#include <QListView>


namespace Ui {
class CustomListView;
}

class CustomListView : public QListView {
    Q_OBJECT
public:
    CustomListView(QWidget *parent=0);

signals:
    void currentIndexChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override {
        QListView::currentChanged(current, previous);
        emit currentIndexChanged(current, previous);
    }
};


#endif //TODOMANAGER_CUSTOMLISTVIEW_H
