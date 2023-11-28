#include "bill20widget.h"
#include "ui_bill20widget.h"

Bill20Widget::Bill20Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill20Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Bill20Widget::btnClicked);

    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/new/prefix1/img/20euro.jpg);}");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->move(780, 490);
}

Bill20Widget::~Bill20Widget()
{
    delete ui;
}

void Bill20Widget::btnClicked()
{
    this->close();
}
