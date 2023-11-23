#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");

    connectMenuBtns();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::setAll(const QString &newIdcard, const QString &newPIN, const QString &newCardType,
            const QString &newIdcustomer, const QString &newFname, const QString &newLname,
            const QString &newIdaccount, const QString &newAccountType,
            const QString &newAccountName, const QString &newBalance,
            const QByteArray &newToken, bool newBitcoinAccount)
{
    idcard = newIdcard;
    PIN = newPIN;
    card_type = newCardType;
    idcustomer = newIdcustomer;
    fname = newFname;
    lname = newLname;
    idaccount = newIdaccount;
    account_type = newAccountType;
    account_name = newAccountName;
    balance = newBalance;
    token = newToken;
    bitcoinAccount = newBitcoinAccount;
}

void Menu::setUpMenuTxt()
{
    ui->label_top->setText(("TILI: "+account_name+"\n\nSALDO: "+balance).toUpper());
    ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->txt_right2->setEchoMode(QLineEdit::Normal);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_left1->setText("KÄTEISNOSTO");
        ui->label_left2->setText("TILISIIRTO");
    ui->label_left3->setText("TILITAPAHTUMAT");

    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_right3->setText("KIRJAUDU ULOS");

    if(bitcoinAccount && account_type == "debit")
    {
        ui->txt_right1->setText("BITCOIN");
    }
}

void Menu::connectMenuBtns()
{
    connect(ui->btn_left1, &QPushButton::clicked, this, &Menu::btn_left1_clicked);
    connect(ui->btn_left2, &QPushButton::clicked, this, &Menu::btn_left2_clicked);
    connect(ui->btn_left3, &QPushButton::clicked, this, &Menu::btn_left3_clicked);
    connect(ui->btn_right1, &QPushButton::clicked, this, &Menu::btn_right1_clicked);
    connect(ui->btn_right2, &QPushButton::clicked, this, &Menu::btn_right2_clicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Menu::btn_right3_clicked);
}

void Menu::btn_left1_clicked()
{

}

void Menu::btn_left2_clicked()
{

}

void Menu::btn_left3_clicked()
{

}

void Menu::btn_right1_clicked()
{

}

void Menu::btn_right2_clicked()
{

}

void Menu::btn_right3_clicked()
{
    this->close();
}