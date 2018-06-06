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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initConfig();

    this->splitter = new QSplitter(Qt::Vertical, this);
    this->todoListWidget = new TodoListWidget(this);
    this->logWidget = new LogWidget(this);

    splitter->addWidget(this->todoListWidget);
    splitter->addWidget(this->logWidget);

    Logger::getInstance()->init(this->logWidget);

    this->setCentralWidget(this->splitter);

    // notification timer
    this->timer = new QTimer();
    this->interval = 60000;
    connect(this->timer, &QTimer::timeout, this, &MainWindow::update_notification_timer);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this, &MainWindow::update_notification_timer);
    this->update_notification_timer();
    // end
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
        if (QTime::currentTime().msec() - item.getFromTime().msec() >= 0
                && QTime::currentTime().msec() - item.getFromTime().msec() <= 30000) {
            todo::NofityUtils::push(tr("Task begins!"), item.getTitle());
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
