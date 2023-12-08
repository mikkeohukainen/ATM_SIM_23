#include "withdraw.h"
#include "billwidget.h"
#include "ui_withdraw.h"

Withdraw::Withdraw(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Withdraw)
    , manager(new QNetworkAccessManager(this))
    , state(1)
{
    ui->setupUi(this);

    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");

    setLabels();
    connectBtns();
}

Withdraw::~Withdraw()
{
    manager->deleteLater();
    delete ui;
}

void Withdraw::setVars(const QString &newIdaccount, const QString &newAccountType, const QByteArray &newToken)
{
    idaccount = newIdaccount;
    account_type = newAccountType;
    token = newToken;
}

void Withdraw::setLabels()
{
    ui->label_top->setText("VALITSE SUMMA");
    ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_left1->setText("20 EUR");
    ui->label_left2->setText("40 EUR");
    ui->label_left3->setText("50 EUR");

    ui->txt_right1->setText("100 EUR");
    ui->txt_right2->setText("MUU SUMMA");
    ui->label_right3->setText("TAKAISIN");
}

void Withdraw::connectBtns()
{
    connect(ui->btn_left1, &QPushButton::clicked, this, &Withdraw::btn_left1_clicked);
    connect(ui->btn_left2, &QPushButton::clicked, this, &Withdraw::btn_left2_clicked);
    connect(ui->btn_left3, &QPushButton::clicked, this, &Withdraw::btn_left3_clicked);
    connect(ui->btn_right1, &QPushButton::clicked, this, &Withdraw::btn_right1_clicked);
    connect(ui->btn_right2, &QPushButton::clicked, this, &Withdraw::btn_right2_clicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::btn_right3_clicked);
}

void Withdraw::btn_left1_clicked()
{
    if (state == 1)
    {
        amount = "20";
        confirm();
    }
}

void Withdraw::btn_left2_clicked()
{
    if (state == 1)
    {
        amount = "40";
        confirm();
    }
}

void Withdraw::btn_left3_clicked()
{
    if (state == 1)
    {
        amount = "50";
        confirm();
    }
}

void Withdraw::btn_right1_clicked()
{
    if (state == 1)
    {
        amount = "100";
        confirm();
    }
}

void Withdraw::btn_right2_clicked()
{
    state = 2;
    otherAmount();
}

void Withdraw::btn_right3_clicked()
{
    this->close();
}

void Withdraw::otherAmount()
{
    disconnectBtns();
    connectOtherAmntBtns();

    ui->label_top->setText("SYÖTÄ SUMMA (MAX 2000 EUR)\n10 EUR TARKKUUDELLA\nJA PAINA ENTER");

    ui->label_left2->setText("");
    ui->label_left3->setText("");
    ui->txt_right1->setText("");
    ui->txt_right2->setText("");

    ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left1->setText("SUMMA:  ");

    ui->txt_right1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setFocus();
}

void Withdraw::connectOtherAmntBtns()
{
    connect(ui->btn_num0, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num1, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num2, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num3, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num4, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num5, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num6, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num7, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num8, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);
    connect(ui->btn_num9, &QPushButton::clicked, this, &Withdraw::number_btn_clicked);

    connect(ui->btnEnter, &QPushButton::clicked, this, &Withdraw::btnEnterClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &Withdraw::btnCancelClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &Withdraw::btnClearClicked);

    connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::goBack);
}

void Withdraw::number_btn_clicked()
{
    QPushButton * button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();

    amountTxt += name.right(1);

    ui->txt_right1->setText(amountTxt);
}

void Withdraw::btnEnterClicked()
{
    if (amountTxt.toInt() % 10 == 0
        && amountTxt.toInt() <= 2000
        && amountTxt.toInt() > 0)
    {
        amount = amountTxt;
        confirm();
    }
    else
    {
        ui->label_top->setText("SUMMA EI KELPAA");
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::goBack);
        ui->txt_right1->setText("");
        ui->label_left1->setText("");
        ui->label_right3->setText("TAKAISIN");
    }
}

void Withdraw::btnCancelClicked()
{
    amountTxt = "";
    ui->txt_right1->setText(amountTxt);
}

void Withdraw::btnClearClicked()
{
    amountTxt.chop(1);
    ui->txt_right1->setText(amountTxt);
}

void Withdraw::goBack()
{
    clearAmount();
    disconnectBtns();
    setLabels();
    connectBtns();
    state = 1;
}

void Withdraw::clearAmount()
{
    amount.clear();
    amountTxt.clear();
}

void Withdraw::disconnectBtns()
{
    ui->btnEnter->disconnect();
    ui->btnCancel->disconnect();
    ui->btnClear->disconnect();
    ui->btn_num0->disconnect();
    ui->btn_num1->disconnect();
    ui->btn_num2->disconnect();
    ui->btn_num3->disconnect();
    ui->btn_num4->disconnect();
    ui->btn_num5->disconnect();
    ui->btn_num6->disconnect();
    ui->btn_num7->disconnect();
    ui->btn_num8->disconnect();
    ui->btn_num9->disconnect();

    ui->btn_left1->disconnect();
    ui->btn_left2->disconnect();
    ui->btn_left3->disconnect();
    ui->btn_right1->disconnect();
    ui->btn_right2->disconnect();
    ui->btn_right3->disconnect();
}

void Withdraw::connectConfirmBtns()
{
    connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::goBack);
    connect(ui->btn_right1, &QPushButton::clicked, this, &Withdraw::startWithdrawal);
}

void Withdraw::confirm()
{
    disconnectBtns();
    connectConfirmBtns();

    ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_top->setText("NOSTETAAN "+amount+" EUR");
    ui->label_left1->setText("");
    ui->label_left2->setText("");
    ui->label_left3->setText("");
    ui->txt_right2->setText("");
    ui->txt_right1->setText("VAHVISTA");
}

void Withdraw::startWithdrawal()
{
    ui->txt_right1->setText("");
    ui->label_right3->setText("");

    withdrawMoney();
}

void Withdraw::withdrawMoney()
{
    QJsonObject jsonObjLogin;
    jsonObjLogin.insert("idaccount", idaccount);
    jsonObjLogin.insert("amount", amount);
    QString url = account_type == "debit" ? "http://localhost:3000/withdrawal/debit"
                                          : "http://localhost:3000/withdrawal/credit";

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), token);

    QNetworkReply *reply = manager->post(request, QJsonDocument(jsonObjLogin).toJson());
    connect(reply, &QNetworkReply::finished, this, &Withdraw::withdrawMoneySlot);
}

void Withdraw::withdrawMoneySlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    auto data = reply->readAll();
    qDebug() << data;

    QJsonDocument json_doc = QJsonDocument::fromJson(data);
    QJsonObject json_obj = json_doc.object();

    if (json_obj["error"].toString() == "Insufficient funds")
    {
        noFunds();
    }
    else if (json_obj["error"].toString() == "Error connecting to the database")
    {
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::btn_right3_clicked);
        ui->txt_right1->setText("");
        ui->label_right3->setText("SULJE");
        ui->label_top->setText("VIRHE TIETOKANTAYHTEYDESSÄ");
    } else if (data.length() < 2) {
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::btn_right3_clicked);
        ui->txt_right1->setText("");
        ui->label_right3->setText("SULJE");
        ui->label_top->setText("PALVELIN EI VASTAA");
    } else if (json_obj["message"].toString() == "Withdrawal successful") {
        takeMoney();
        emit moneyWithdrawn();
    }
}

void Withdraw::noFunds()
{
    qDebug() << "No funds";

    disconnectBtns();
    connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::btn_right3_clicked);
    ui->txt_right1->setText("");
    ui->label_right3->setText("SULJE");
    ui->label_top->setText("KATE EI RIITÄ");

}

void Withdraw::takeMoney()
{
    disconnectBtns();
    ui->label_top->setText("RAHAT TULOSSA...");

    bills = calculateBills(amount.toInt());

    QTimer::singleShot(2000, this, &Withdraw::continueTakeMoney);
}

std::map<int, int> Withdraw::calculateBills(int wAmount)
{
    std::vector<int> denominations = {500, 100, 50, 20, 10};
    std::map<int, int> billsNeeded;

    for (int denom : denominations) {
        int count = wAmount / denom;
        if (count > 0) {
            billsNeeded[denom] = count;
            wAmount -= count * denom;
        }
    }

    return billsNeeded;
}

void Withdraw::continueTakeMoney()
{
    ui->label_top->setText("OTA RAHAT");
    ui->label_right3->setText("SULJE");

    connect(ui->btn_right3, &QPushButton::clicked, this, &Withdraw::btn_right3_clicked);

    billWidgetOffsets.clear();

    for (const auto &pair : bills) {
        int billType = pair.first;
        int billCount = pair.second;

        for (int i = 0; i < billCount; ++i) {

            int xOffset = billWidgetOffsets[billType] * 60;
            int yOffset = billWidgetOffsets[billType] * 60;

            switch (billType) {

            case 10: {
                BillWidget *bill = new BillWidget(this,
                                                  "10euro.jpg",
                                                  QPoint(740 - xOffset, 450 + yOffset));
                bill->show();
                billWidgetOffsets[billType]++;
                break;
                }
            case 20: {
                BillWidget *bill = new BillWidget(this,
                                                  "20euro.jpg",
                                                  QPoint(780 - xOffset, 490 + yOffset));
                bill->show();
                billWidgetOffsets[billType]++;
                break;
                }
            case 50: {
                BillWidget *bill = new BillWidget(this,
                                                  "50euro.jpg",
                                                  QPoint(820 - xOffset, 530 + yOffset));
                bill->show();
                billWidgetOffsets[billType]++;
                break;
                }
            case 100: {
                BillWidget *bill = new BillWidget(this,
                                                  "100euro.jpg",
                                                  QPoint(860 - xOffset, 570 + yOffset));
                bill->show();
                billWidgetOffsets[billType]++;
                break;
                }
            case 500: {
                BillWidget *bill = new BillWidget(this,
                                                  "500euro.jpg",
                                                  QPoint(900 - xOffset, 610 + yOffset));
                bill->show();
                billWidgetOffsets[billType]++;
                break;
                }
            }
        }
    }
}
