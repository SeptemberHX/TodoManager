#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
