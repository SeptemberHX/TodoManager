#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/logger.h"
#include "widgets/FetchConfigFilePathWidget.h"
#include "./config/TodoConfig.h"
#include "./functions/TaskOnTimeNotifier.h"
#include "./functions/AppSystemTrayIcon.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QMutexLocker>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initConfig();

    this->todoListWidget = new TodoListWidget(this);
    this->inboxViewWidget = new TodoListWidget(this, TodoListWidgetMode::INBOX);
    this->tagModeWidget = new TagModeWidget(this);
    this->calendarModeWidget = new CalendarModeWidget(this);
    this->projectModeWidget = new ProjectModeWidget(this);
    this->stickyNoteModeWidget = new StickyNoteModeWidget(this);

    this->logWidget = new LogWidget();
    Logger::getInstance()->init(this->logWidget);
//    this->splitter = new QSplitter(Qt::Vertical, this);
//    splitter->addWidget(this->todoListWidget);
//    splitter->addWidget(this->logWidget);
//    this->setCentralWidget(this->splitter);

    // multi view mode support
    this->viewButtonGroup = new QButtonGroup(this);
    this->viewButtonGroup->addButton(ui->inboxModePushButton);
    this->viewButtonGroup->addButton(ui->dailyModePushButton);
    this->viewButtonGroup->addButton(ui->tagModePushButton);
    this->viewButtonGroup->addButton(ui->calendarPushButton);
    this->viewButtonGroup->addButton(ui->projectPushButton);
    this->viewButtonGroup->addButton(ui->stickyNotePushButton);
    dailyMode = ui->stackedWidget->addWidget(this->todoListWidget);
    inboxMode = ui->stackedWidget->addWidget(this->inboxViewWidget);
    tagMode = ui->stackedWidget->addWidget(this->tagModeWidget);
    calendarMode = ui->stackedWidget->addWidget(this->calendarModeWidget);
    projectMode = ui->stackedWidget->addWidget(this->projectModeWidget);
    stickyNoteMode = ui->stackedWidget->addWidget(this->stickyNoteModeWidget);

    connect(this->inboxViewWidget, &TodoListWidget::databaseModified, this, &MainWindow::database_modified);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this, &MainWindow::database_modified);
    connect(this->tagModeWidget, &TagModeWidget::databaseModified, this, &MainWindow::database_modified);
    connect(this->projectModeWidget, &ProjectModeWidget::databaseModified, this, &MainWindow::database_modified);
    connect(this->stickyNoteModeWidget, &StickyNoteModeWidget::databaseModified, this, &MainWindow::database_modified);

    connect(this->viewButtonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &MainWindow::modeBtn_clicked);
    this->currentMode = -1;  // initialization
    ui->inboxModePushButton->click();

    // jump
    connect(this->calendarModeWidget, &CalendarModeWidget::itemClicked, this, &MainWindow::item_clicked);
    connect(this->calendarModeWidget, &CalendarModeWidget::targetDayClicked, this, &MainWindow::targetDay_clicked);
    connect(this->inboxViewWidget, &TodoListWidget::jumpToGroup, this, &MainWindow::jump_to_specific_group);
    connect(this->todoListWidget, &TodoListWidget::jumpToGroup, this, &MainWindow::jump_to_specific_group);
    connect(this->tagModeWidget, &TagModeWidget::jumpToGroup, this, &MainWindow::jump_to_specific_group);

    connect(this->inboxViewWidget, &TodoListWidget::jumpToTag, this, &MainWindow::jump_to_specific_tag);
    connect(this->todoListWidget, &TodoListWidget::jumpToTag, this, &MainWindow::jump_to_specific_tag);
    connect(this->projectModeWidget, &ProjectModeWidget::jumpToTag, this, &MainWindow::jump_to_specific_tag);
    // end

    // notification timer
    todo::TaskOnTimeNotifier::getInstance()->init();
    connect(todo::TaskOnTimeNotifier::getInstance(), &todo::TaskOnTimeNotifier::notifyUser,
            this, &MainWindow::notify_user);
    // end

    // Task archiving time recorder
    todo::TaskArchivingTimeRecorder::getInstance()->init();
    connect(this, &MainWindow::taskArchivingOperation,
            todo::TaskArchivingTimeRecorder::getInstance(), &todo::TaskArchivingTimeRecorder::operate);
    connect(todo::TaskArchivingTimeRecorder::getInstance(), &todo::TaskArchivingTimeRecorder::itemDetailTimeModified,
            this, &MainWindow::database_modified);
    // end

    // tray icon
    AppSystemTrayIcon::getInstance()->init(this);
    connect(AppSystemTrayIcon::getInstance(), &AppSystemTrayIcon::quitClicked, this, &MainWindow::click_exit);
    connect(AppSystemTrayIcon::getInstance(), &AppSystemTrayIcon::activated, this, &MainWindow::trayIcon_clicked);
    connect(AppSystemTrayIcon::getInstance(), &AppSystemTrayIcon::archivingOperated, this, &MainWindow::archiving_operated);

    QApplication::setWindowIcon(QIcon(":/icons/tray.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logAppend(const QString &str)
{
    this->logWidget->append(str);
}

void MainWindow::initConfig() {
    todo::TodoConfig::getInstance()->setAppConfig(todo::AppConfig());
    QDir configDir(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)[0]);
    if (!configDir.exists()) {
        configDir.mkdir(configDir.path());
    }

    QString sqlConfigFileName("sql_config.ini");
    QString fullSqlConfigFilePath = QDir::cleanPath(configDir.path() + QDir::separator() + sqlConfigFileName);
    QSettings sqlConfigSettings(fullSqlConfigFilePath, QSettings::IniFormat);
    if (!sqlConfigSettings.contains("SQLite/db_path")) {
        sqlConfigSettings.beginGroup("SQLite");
        FetchConfigFilePathWidget fcfpw(this);
        fcfpw.setWindowTitle(tr("Set sqlite db path"));
        fcfpw.exec();
        sqlConfigSettings.setValue("db_path", fcfpw.getFilePath());
        sqlConfigSettings.endGroup();
    }

    todo::SQLiteConfig sqLiteConfig;
    sqLiteConfig.setDbPath(sqlConfigSettings.value("SQLite/db_path").toString());
    todo::TodoConfig::getInstance()->setSqLiteConfig(sqLiteConfig);
}

void MainWindow::click_exit() {
    AppSystemTrayIcon::getInstance()->hide();
    QApplication::closeAllWindows();  // No need to worry about those Sticky Note Widget whose parents are nullptr. :)
}

void MainWindow::trayIcon_clicked() {
    if (!this->isVisible()) {
        this->show();
    } else if (this->isMinimized()) {
        this->showNormal();
    } else {
        this->hide();
    }
}

void MainWindow::modeBtn_clicked(QAbstractButton *button) {
    QPushButton *btn = dynamic_cast<QPushButton*>(button);
    if (btn == ui->inboxModePushButton && this->currentMode != this->inboxMode) {
        if (this->todoListWidget->isCurrentItemEdited()) {
            QMessageBox::information(this, tr("Can't switch view mode !"), tr("Current item is under editing !"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(this->inboxMode);
        this->currentMode = this->inboxMode;
    } else if (btn == ui->dailyModePushButton && this->currentMode != this->dailyMode) {
        if (this->inboxViewWidget->isCurrentItemEdited()) {
            QMessageBox::information(this, tr("Can't switch view mode !"), tr("Current item is under editing !"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(this->dailyMode);
        this->currentMode = this->dailyMode;
    } else if (btn == ui->tagModePushButton && this->currentMode != this->tagMode) {
        if (this->inboxViewWidget->isCurrentItemEdited()) {
            QMessageBox::information(this, tr("Can't switch view mode !"), tr("Current item is under editing !"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(this->tagMode);
        this->currentMode = this->tagMode;
    } else if (btn == ui->calendarPushButton && this->currentMode != this->calendarMode) {
        if (this->inboxViewWidget->isCurrentItemEdited()) {
            QMessageBox::information(this, tr("Can't switch view mode !"), tr("Current item is under editing !"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(this->calendarMode);
        this->currentMode = this->calendarMode;
    } else if (btn == ui->projectPushButton && this->currentMode != this->projectMode) {
        if (this->inboxViewWidget->isCurrentItemEdited()) {
            QMessageBox::information(this, tr("Can't switch view mode !"), tr("Current item is under editing !"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(this->projectMode);
        this->currentMode = this->projectMode;
    } else if (btn == ui->stickyNotePushButton && this->currentMode != this->stickyNoteMode) {
        ui->stackedWidget->setCurrentIndex(this->stickyNoteMode);
        this->currentMode = this->stickyNoteMode;
    }
}

void MainWindow::database_modified(const QString &senderObjectName) {
    todo::TaskOnTimeNotifier::getInstance()->update_timer();
    AppSystemTrayIcon::getInstance()->update_menu();

    if (senderObjectName != this->todoListWidget->objectName()) {
        this->todoListWidget->refresh_current_items();
    }

    if (senderObjectName != this->inboxViewWidget->objectName()) {
        this->inboxViewWidget->refresh_current_items();
    }

    if (senderObjectName != this->tagModeWidget->objectName()) {
        this->tagModeWidget->refresh_current_items();
    }

    if (senderObjectName != this->calendarModeWidget->objectName()) {
        this->calendarModeWidget->refresh_current_items();
    }

    if (senderObjectName != this->projectModeWidget->objectName()) {
        this->projectModeWidget->refresh_current_items();
    }

    this->stickyNoteModeWidget->refresh_curr_item(senderObjectName);
}

void MainWindow::item_clicked(const todo::ItemDetail &item) {
    ui->dailyModePushButton->click();
    this->todoListWidget->jump_to_specific_item(item);
}

void MainWindow::targetDay_clicked(const QDate &targetDay) {
    ui->dailyModePushButton->click();
    this->todoListWidget->jump_to_specific_day(targetDay);
}

void MainWindow::jump_to_specific_group(const QString &groupID) {
    ui->projectPushButton->click();
    this->projectModeWidget->jump_to(groupID);
}

void MainWindow::jump_to_specific_tag(const QString &itemID) {
    ui->tagModePushButton->click();
    this->tagModeWidget->jump_to_tag(itemID);
}

void MainWindow::notify_user(const QString &titleStr, const QString &bodyStr) {
    AppSystemTrayIcon::getInstance()->showMessage(titleStr, bodyStr);
}

void MainWindow::archiving_operated(const QString &itemID, const todo::TaskArchivingOperation &operation) {
    qDebug() << itemID << operation;
    // todo::TaskArchivingTimeRecorder::getInstance()->operate(itemID, operation);
}
