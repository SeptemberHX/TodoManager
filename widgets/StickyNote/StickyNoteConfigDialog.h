#ifndef STICKYNOTECONFIGDIALOG_H
#define STICKYNOTECONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "../../config/StickyNoteConfig.h"

namespace Ui {
class StickyNoteConfigDialog;
}

class StickyNoteConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StickyNoteConfigDialog(QWidget *parent = nullptr);
    ~StickyNoteConfigDialog();
    todo::StickyNoteConfig collectConfig();
    void loadConfig(const todo::StickyNoteConfig &config);

private:
    Ui::StickyNoteConfigDialog *ui;

    bool isInputValid();

    QString projectId;
    QString tagId;

private slots:
    void projectToolButton_clicked();
    void tagToolButton_clicked();
    void buttonBox_clicked(QAbstractButton *button);
};

#endif // STICKYNOTECONFIGDIALOG_H
