#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextCharFormat>

namespace Ui {
class MyTextEdit;
}

class MyTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit MyTextEdit(QWidget *parent = 0);
    ~MyTextEdit();
    void setReadOnly(bool readOnly);
    void setHtml(const QString &htmlStr);
    QString toHtml() const;

signals:
    void textChanged();

private:
    Ui::MyTextEdit *ui;
    int minFontSize;
    int maxFontSize;

private slots:
    void boldToolButton_clicked(bool checked);
    void underLineToolButton_clicked(bool checked);
    void italyToolButton_clicked(bool checked);
    void deleteLineToolButton_clicked(bool checked);
    void text_changed();
    void currentCharFormat_changed(const QTextCharFormat &f);
    void currentFont_changed(const QFont &f);
    void smallToolButton_clicked();
    void largeToolButton_clicked();
    void fontSizeComboBox_currentChanged();
    void colorToolButton_clicked();
};

#endif // MYTEXTEDIT_H
