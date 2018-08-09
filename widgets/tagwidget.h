#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <QWidget>
#include <QList>
#include <QFont>
#include "taglabelwidget.h"
#include "flowlayout.h"
#include "../core/ItemTag.h"

namespace Ui {
class TagWidget;
}

class TagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagWidget(QWidget *parent = 0);
    ~TagWidget();

    void addTag(const todo::ItemTag &tagItem);
    QList<todo::ItemTag> getTags();

    void setReadOnly(bool isReadOnly);
    void setTagFont(const QFont &font);
    void setTagMargin(int h, int v);
    void clear();

signals:
    void tag_changed();  // tell other widgets that tags changed. So other widgets can adjust there info view.
    void tagClicked(const QString &tagID);

private:
    Ui::TagWidget *ui;
    QList<TagLabelWidget*> tagLabelList;
    FlowLayout *tagLayout;
    TagLabelWidget *findTagButton(const QString &tagStr);
    void resizeAll();
    QFont showFont;
    QSize tagMargin;
    bool readOnly;
    QMap<QString, todo::ItemTag> tagMap;

private slots:
    void tagLabel_clicked();
    void addTagBtn_clicked();
};

#endif // TAGWIDGET_H
