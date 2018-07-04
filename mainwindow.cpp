#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/logger.h"
#include "widgets/FetchConfigFilePathWidget.h"
#include "utils/NofityUtils.h"
#include "./config/TodoConfig.h"

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
    dailyMode = ui->stackedWidget->addWidget(this->todoListWidget);
    inboxMode = ui->stackedWidget->addWidget(this->inboxViewWidget);
    tagMode = ui->stackedWidget->addWidget(this->tagModeWidget);
    connect(this->inboxViewWidget, &TodoListWidget::databaseModified, this->todoListWidget, &TodoListWidget::refresh_current_items);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this->inboxViewWidget, &TodoListWidget::refresh_current_items);

    connect(this->viewButtonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &MainWindow::modeBtn_clicked);
    this->currentMode = -1;  // initialization
    ui->inboxModePushButton->click();
//    ui->stackedWidget->setCurrentIndex(dailyMode);

    // notification timer
    this->timer = new QTimer();
    this->interval = 60000;
    connect(this->timer, &QTimer::timeout, this, &MainWindow::update_notification_timer);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this, &MainWindow::update_notification_timer);
    connect(this->inboxViewWidget, &TodoListWidget::databaseModified, this, &MainWindow::update_notification_timer);
    this->update_notification_timer();
    // end

    // tray icon
    this->trayIcon = new QSystemTrayIcon(QIcon(":/icons/tray.png"), this);
    QMenu *trayMenu = new QMenu(this);
    QAction *quitAction = new QAction("Quit", this);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    connect(quitAction, &QAction::triggered, this, &MainWindow::click_exit);
    connect(this->trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIcon_clicked);

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

void MainWindow::change_statusBar_text(const QString &str) {
    statusBar()->showMessage(str);
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

void MainWindow::update_notification_timer() {
    QMutexLocker locker(&this->notificationListMutex);
    for (auto const &item : this->targetItemDetails) {
        if (QTime::currentTime().msecsTo(item.getFromTime()) <= 0) {
//            NofityUtils::push(tr("Task begins!"), item.getTitle());
            this->trayIcon->showMessage(tr("Task begins!"), item.getTitle(), QIcon(":/icons/tray.png"));
        }
    }
    this->targetItemDetails.clear();

    // select most recently items, and set it to targetItemDetails
    this->targetItemDetails = this->dataCenter.selectNextNotifiedItemDetail();
    for (auto const &item : this->targetItemDetails) {
        qDebug() << "Next item to notify: " << item.getTitle();
    }

    // calculate next timeout round
    qint64 nextInterval = this->interval;
    if (!this->targetItemDetails.empty()) {
        QDateTime nextItemTime;
        nextItemTime.setDate(this->targetItemDetails[0].getTargetDate());
        nextItemTime.setTime(this->targetItemDetails[0].getFromTime());
        nextInterval = QDateTime::currentDateTime().msecsTo(nextItemTime);
    }
    this->timer->start(int(nextInterval));
    qDebug() << "Wait for " << int(nextInterval) / 1000 << " secs to notify next item";
}

void MainWindow::click_exit() {
    this->trayIcon->hide();  // Must hide it, or the app will not quit.
    this->close();
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
    }
}
