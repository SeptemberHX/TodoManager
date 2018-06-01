#include "taglabelwidget.h"
#include "ui_taglabelwidget.h"

TagLabelWidget::TagLabelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagLabelWidget)
{
    ui->setupUi(this);
    this->tagMargin = QSize(20, 0);
    this->font = QFont("Arias");
}

TagLabelWidget::~TagLabelWidget()
{
    delete ui;
}

void TagLabelWidget::setText(const QString &str)
{
    ui->tagLabel->setText(str);
    ui->tagLabel->setFont(this->font);
    auto textSize = ui->tagLabel->fontMetrics().size(Qt::TextShowMnemonic, ui->tagLabel->text());
    ui->tagLabel->setFixedSize(textSize + this->tagMargin);
}

void TagLabelWidget::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

QString TagLabelWidget::text() const {
    return ui->tagLabel->text();
}

void TagLabelWidget::setTagColor(const QColor &color)
{
    QString values = QString("%1, %2, %3, %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->tagLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
}
