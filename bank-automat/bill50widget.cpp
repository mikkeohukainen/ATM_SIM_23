#include "bill50widget.h"
#include "ui_bill50widget.h"

Bill50Widget::Bill50Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill50Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Bill50Widget::btnClicked);

    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/new/prefix1/img/50euro.jpg);}");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->move(820, 530);
}

Bill50Widget::~Bill50Widget()
{
    delete ui;
}

void Bill50Widget::btnClicked()
{
    this->close();
}
