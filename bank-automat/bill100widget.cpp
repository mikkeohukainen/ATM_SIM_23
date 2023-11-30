#include "bill100widget.h"
#include "ui_bill100widget.h"

Bill100Widget::Bill100Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill100Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Bill100Widget::btnClicked);

    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/new/prefix1/img/100euro.jpg);}");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->move(860, 570);
}

Bill100Widget::~Bill100Widget()
{
    delete ui;
}

void Bill100Widget::btnClicked()
{
    this->close();
}
