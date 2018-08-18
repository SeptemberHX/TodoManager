#include "MyTextEdit.h"
#include "ui_MyTextEdit.h"
#include <QDebug>
#include <QColorDialog>

MyTextEdit::MyTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyTextEdit)
{
    ui->setupUi(this);
    this->minFontSize = 6;
    this->maxFontSize = 20;

    // init the font size combobox
    for (int s = this->minFontSize; s <= this->maxFontSize; ++s) {
        ui->fontSizeComboBox->addItem(QString::number(s), s);
    }

    // connect all things
    connect(ui->boldToolButton, &QToolButton::clicked, this, &MyTextEdit::boldToolButton_clicked);
    connect(ui->underLineToolButton, &QToolButton::clicked, this, &MyTextEdit::underLineToolButton_clicked);
    connect(ui->italyToolButton, &QToolButton::clicked, this, &MyTextEdit::italyToolButton_clicked);
    connect(ui->deleteLineToolButton, &QToolButton::clicked, this, &MyTextEdit::deleteLineToolButton_clicked);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MyTextEdit::text_changed);
    connect(ui->textEdit, &QTextEdit::currentCharFormatChanged, this, &MyTextEdit::currentCharFormat_changed);
    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, &MyTextEdit::currentFont_changed);
    connect(ui->smallToolButton, &QToolButton::clicked, this, &MyTextEdit::smallToolButton_clicked);
    connect(ui->largeToolButton, &QToolButton::clicked, this, &MyTextEdit::largeToolButton_clicked);
    connect(ui->fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MyTextEdit::fontSizeComboBox_currentChanged);
    connect(ui->colorToolButton, &QToolButton::clicked, this, &MyTextEdit::colorToolButton_clicked);
    connect(ui->lineToolButton, &QToolButton::clicked, this, &MyTextEdit::lineToolButton_clicked);
    connect(ui->listToolButton, &QToolButton::clicked, this, &MyTextEdit::listToolButton_clicked);

    // set icons
    ui->italyToolButton->setIcon(QIcon::fromTheme("format-text-italic"));
    ui->boldToolButton->setIcon(QIcon::fromTheme("format-text-bold"));
    ui->underLineToolButton->setIcon(QIcon::fromTheme("format-text-underline"));
    ui->deleteLineToolButton->setIcon(QIcon::fromTheme("format-text-strikethrough"));
    ui->smallToolButton->setIcon(QIcon::fromTheme("format-text-smaller"));
    ui->largeToolButton->setIcon(QIcon::fromTheme("format-text-larger"));
    ui->colorToolButton->setIcon(QIcon::fromTheme("format-text-color"));

    ui->fontComboBox->setCurrentFont(QFont("Arial", 12));
    ui->fontSizeComboBox->setCurrentText("12");
    ui->colorToolButton->setStyleSheet(QString("background:%1").arg(QColor(Qt::black).name()));
    ui->textEdit->setFocus();
}

MyTextEdit::~MyTextEdit()
{
    delete ui;
}

void MyTextEdit::boldToolButton_clicked(bool checked) {
    if (checked) {
        ui->textEdit->setFontWeight(QFont::Bold);
    } else {
        ui->textEdit->setFontWeight(QFont::Normal);
    }
}

void MyTextEdit::underLineToolButton_clicked(bool checked) {
    qDebug() << "italic " << checked;
    ui->textEdit->setFontUnderline(checked);
}

void MyTextEdit::italyToolButton_clicked(bool checked) {
    ui->textEdit->setFontItalic(checked);
}

void MyTextEdit::deleteLineToolButton_clicked(bool checked) {
    auto f = ui->textEdit->font();
    f.setStrikeOut(checked);
    ui->textEdit->setCurrentFont(f);
}

void MyTextEdit::setReadOnly(bool readOnly) {
    ui->operationWidget->setVisible(!readOnly);
    ui->textEdit->setReadOnly(readOnly);
}

void MyTextEdit::setHtml(const QString &htmlStr) {
    ui->textEdit->setHtml(htmlStr);
}

QString MyTextEdit::toHtml() const {
    return ui->textEdit->toHtml();
}

void MyTextEdit::text_changed() {
    emit textChanged();
}

void MyTextEdit::currentCharFormat_changed(const QTextCharFormat &f) {
    ui->italyToolButton->setChecked(f.fontItalic());
    ui->boldToolButton->setChecked(f.fontWeight() == QFont::Bold);
    ui->underLineToolButton->setChecked(f.fontUnderline());
    ui->deleteLineToolButton->setChecked(f.fontStrikeOut());
    ui->fontComboBox->setCurrentText(f.fontFamily());
    ui->fontSizeComboBox->setCurrentText(QString::number(f.font().pointSize()));
    ui->colorToolButton->setStyleSheet(QString("background:%1").arg(f.foreground().color().name()));

    if (f.font().pointSize() < this->maxFontSize) {
        ui->largeToolButton->setEnabled(true);
    }

    if (f.font().pointSize() > this->minFontSize) {
        ui->smallToolButton->setEnabled(true);
    }
}

void MyTextEdit::currentFont_changed(const QFont &f) {
    QFont fCopy(f);
    fCopy.setPointSize(ui->fontSizeComboBox->currentData(Qt::UserRole).toInt());
    ui->textEdit->setCurrentFont(fCopy);
}

void MyTextEdit::smallToolButton_clicked() {
    auto f = ui->textEdit->currentFont();
    f.setPointSize(ui->fontSizeComboBox->currentData(Qt::UserRole).toInt() - 1);
    ui->textEdit->setCurrentFont(f);
    ui->fontSizeComboBox->setCurrentText(QString::number(f.pointSize()));

    if (f.pointSize() == this->minFontSize) {
        ui->smallToolButton->setEnabled(false);
    }
    if (!ui->largeToolButton->isEnabled()) {
        ui->largeToolButton->setEnabled(true);
    }
}

void MyTextEdit::largeToolButton_clicked() {
    auto f = ui->textEdit->currentFont();
    f.setPointSize(ui->fontSizeComboBox->currentData(Qt::UserRole).toInt() + 1);
    qDebug() << f.pointSize();
    ui->textEdit->setCurrentFont(f);
    ui->fontSizeComboBox->setCurrentText(QString::number(f.pointSize()));

    if (f.pointSize() == this->maxFontSize) {
        ui->largeToolButton->setEnabled(false);
    }
    if (!ui->smallToolButton->isEnabled()) {
        ui->smallToolButton->setEnabled(true);
    }
}

void MyTextEdit::fontSizeComboBox_currentChanged() {
    if (ui->textEdit->textCursor().hasSelection()) return;

    int fs = ui->fontSizeComboBox->currentData(Qt::UserRole).toInt();
    ui->largeToolButton->setEnabled(fs < this->maxFontSize);
    ui->smallToolButton->setEnabled(fs > this->minFontSize);
    auto f = ui->textEdit->currentFont();
    f.setPointSize(fs);
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->hasFocus();
}

void MyTextEdit::colorToolButton_clicked() {
    auto newColor = QColorDialog::getColor();
    ui->textEdit->setTextColor(newColor);
    ui->colorToolButton->setStyleSheet(QString("background:%1").arg(newColor.name()));
}

void MyTextEdit::lineToolButton_clicked() {
    auto oldBlockFormat = ui->textEdit->textCursor().blockFormat();
    ui->textEdit->textCursor().insertHtml("<hr>");
    ui->textEdit->textCursor().insertBlock();
    ui->textEdit->textCursor().setBlockFormat(oldBlockFormat);
}

void MyTextEdit::listToolButton_clicked() {
    ui->textEdit->textCursor().insertList(QTextListFormat::ListDisc);
}
