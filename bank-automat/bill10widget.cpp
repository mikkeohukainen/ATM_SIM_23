#include "bill10widget.h"
#include "ui_bill10widget.h"

Bill10Widget::Bill10Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill10Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Bill10Widget::btnClicked);

    ui->widget->setStyleSheet("QWidget#widget{image: url(:/new/prefix1/img/10euro.jpg);}");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->move(740, 450);
}

Bill10Widget::~Bill10Widget()
{
    delete ui;
}

void Bill10Widget::btnClicked()
{
    this->close();
}
