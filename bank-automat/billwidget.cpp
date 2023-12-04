#include "billwidget.h"
#include <QImageReader>
#include <QPixmap>
#include <QPoint>
#include "ui_billwidget.h"

BillWidget::BillWidget(QWidget *parent, const QString &fileName, const QPoint &position)
    : QWidget(parent)
    , ui(new Ui::BillWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &BillWidget::clicked);

    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/new/prefix1/img/" + fileName
                              + ");}");

    setWindowFlags(windowFlags() | Qt::Widget | Qt::FramelessWindowHint);
    move(position);
}

BillWidget::~BillWidget()
{
    delete ui;
}

void BillWidget::clicked()
{
    close();
}
