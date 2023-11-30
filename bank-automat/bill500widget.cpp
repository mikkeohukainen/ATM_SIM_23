#include "bill500widget.h"
#include "ui_bill500widget.h"

Bill500Widget::Bill500Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill500Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Bill500Widget::btnClicked);

    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/new/prefix1/img/500euro.jpg);}");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->move(900, 610);
}

Bill500Widget::~Bill500Widget()
{
    delete ui;
}

void Bill500Widget::btnClicked()
{
    this->close();
}
