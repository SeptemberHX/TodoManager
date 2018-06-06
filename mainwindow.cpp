#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/logger.h"
#include "widgets/FetchConfigFilePathWidget.h"
#include "./config/TodoConfig.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>

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
