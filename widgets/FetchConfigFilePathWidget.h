#ifndef FETCHCONFIGFILEPATHWIDGET_H
#define FETCHCONFIGFILEPATHWIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>
#include <QDialog>

namespace Ui {
class FetchConfigFilePathWidget;
}

class FetchConfigFilePathWidget : public QDialog
{
    Q_OBJECT

public:
    explicit FetchConfigFilePathWidget(QWidget *parent = 0);
    ~FetchConfigFilePathWidget();

    const QString& getFilePath();

private slots:
    void on_newFileCheckBox_stateChanged(int arg1);

    void on_chooseFileCheckBox_stateChanged(int arg1);

    void on_newFileToolButton_clicked();

    void on_chooseFileToolButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::FetchConfigFilePathWidget *ui;
    QString filePath;
};

#endif // FETCHCONFIGFILEPATHWIDGET_H
