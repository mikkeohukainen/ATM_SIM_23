#include "transfer.h"
#include "ui_transfer.h"

Transfer::Transfer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Transfer),

    status(ENTER_ACCOUNT)

{
    ui->setupUi(this);
    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");
    setLabels();
    connectBtns();
}

Transfer::~Transfer()
{
    delete ui;
}

void Transfer::setVars(const QString &newSendingAccountId, const QByteArray &newToken)
{
    sendingAccountId = newSendingAccountId;
    token = newToken;
}

void Transfer::setLabels()
{
    ui->label_top->setText("SYÖTÄ SAAJAN TILINUMERO JA SIIRRETTÄVÄ SUMMA");
    ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_left1->setText("Saajan tilinumero:");
    ui->label_left2->setText("Siirrettävä summa");
    ui->label_left3->setText("");

    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_right3->setText("TAKAISIN");
    ui->txt_right1->setFocus();
}

void Transfer::connectBtns()
{
    connect(ui->btn_right1, &QPushButton::clicked, this, &Transfer::functionButtonClicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Transfer::functionButtonClicked);
    connect(ui->btn_num0, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num1, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num2, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num3, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num4, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num5, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num6, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num7, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num8, &QPushButton::clicked, this, &Transfer::numberButtonClicked);
    connect(ui->btn_num9, &QPushButton::clicked, this, &Transfer::numberButtonClicked);

    connect(ui->btnEnter, &QPushButton::clicked, this, &Transfer::enterButtonClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &Transfer::cancelButtonClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &Transfer::clearButtonClicked);
}

void Transfer::disconnectBtns()
{

}

void Transfer::inputReceiverAccount()
{

}

void Transfer::inputTransferAmount()
{

}

void Transfer::confirmTransfer()
{

}

void Transfer::completeTransfer()
{

}

void Transfer::transferFailed(const QString &errorMessage)
{
    ui->label_top->setText(errorMessage);
}

void Transfer::functionButtonClicked()
{

}

void Transfer::enterButtonClicked()
{
    if (status == ENTER_ACCOUNT && receivingAccountId != "") {

        status = ENTER_AMOUNT;
        ui->txt_right2->setFocus();
    }

    else if (status == ENTER_AMOUNT) {
        status = VALIDATE_DETAILS;
        transferFailed(receiverName);
    }
}

void Transfer::clearButtonClicked()
{

}

void Transfer::cancelButtonClicked()
{

}

void Transfer::numberButtonClicked()
{
    QPushButton * button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    if (status == ENTER_ACCOUNT) {
        receivingAccountId += name.right(1);
        ui->txt_right1->setText(receivingAccountId);
        ui->txt_right1->setFocus();
    }

    else if (status == ENTER_AMOUNT) {
        transferAmount += name.right(1);
        ui->txt_right2->setText(transferAmount);
        ui->txt_right2->setFocus();
    }
}


void Transfer::startTransfer()
{

}

void Transfer::transferMoneySlot(QNetworkReply *reply)
{

}
