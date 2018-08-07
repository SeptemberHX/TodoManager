#ifndef STICKYNOTEWIDGET_H
#define STICKYNOTEWIDGET_H

#include <QWidget>

namespace Ui {
class StickyNoteWidget;
}

class StickyNoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StickyNoteWidget(QWidget *parent = 0);
    ~StickyNoteWidget();

private:
    Ui::StickyNoteWidget *ui;

    void initWidgetStyle();
    QString getStyleSheet(const QColor &bgColor, const QColor &fontColor);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STICKYNOTEWIDGET_H
