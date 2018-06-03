#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>
#include "widgets/logwidget.h"
#include "widgets/todolistwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void logAppend(const QString &str);

private:
    void initConfig();

    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout;
    QSplitter *splitter;
    LogWidget *logWidget;
    TodoListWidget *todoListWidget;

public slots:
    void change_statusBar_text(const QString &str);
};

#endif // MAINWINDOW_H
