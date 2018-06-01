#ifndef TAGLABELWIDGET_H
#define TAGLABELWIDGET_H

#include <QWidget>
#include <QFont>
#include <QString>
#include <QPoint>

namespace Ui {
class TagLabelWidget;
}

class TagLabelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagLabelWidget(QWidget *parent = 0);
    ~TagLabelWidget();

    void setText(const QString &str);
    QString text() const;

    void setTagColor(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();

private:
    Ui::TagLabelWidget *ui;
    QFont font;
    QSize tagMargin;

    void setCustomStyle();
};

#endif // TAGLABELWIDGET_H
