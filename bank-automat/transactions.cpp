#include "transactions.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "ui_transactions.h"

Transactions::Transactions(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Transactions)
    , manager(new QNetworkAccessManager(this))
    , startIndex(0)
{
    ui->setupUi(this);
    ui->mycentralwidget->setStyleSheet(
        "QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");
    setLabel();
    connectBtns();
}

Transactions::~Transactions()
{
    delete manager;
    delete ui;
}

void Transactions::setVariables(const QString &newIdaccount, const QByteArray &newToken)
{
    idaccount = newIdaccount;
    token = newToken;
}

void Transactions::setLabel()
{
    //ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    //ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_left1->setText("SEURAAVA");
    ui->label_left2->setText("EDELLINEN");
    ui->label_left3->setText("");

    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_right3->setText("PÄÄVALIKKO");
}

void Transactions::connectBtns()
{
    connect(ui->btn_left1, &QPushButton::clicked, this, &Transactions::btn_left1_clicked);
    connect(ui->btn_left2, &QPushButton::clicked, this, &Transactions::btn_left2_clicked);
    connect(ui->btn_left3, &QPushButton::clicked, this, &Transactions::btn_left3_clicked);
    connect(ui->btn_right2, &QPushButton::clicked, this, &Transactions::btn_right2_clicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Transactions::btn_right3_clicked);
}

void Transactions::getTransactions()
{
    QJsonObject obj;
    obj.insert("idaccount", idaccount);
    obj.insert("startIndex", startIndex);

    QString url = QString("http://localhost:3000/transactions/%1/%2").arg(idaccount).arg(startIndex);

    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, &Transactions::getTransactionsSlot);
}

void Transactions::getTransactionsSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));

    const auto data = reply->readAll();
    const auto json = QJsonDocument::fromJson(data);

    if (json.isNull() || !json.isArray()) {
        ui->label_top->setText("VIRHE TILITAPAHTUMIEN HAUSSA");
        return;
    }

    QString displayText = "TILITAPAHTUMAT\n\n";
    auto transactions = json.array();

    if (transactions.isEmpty()) {
        ui->label_top->setText("EI TILITAPAHTUMIA");
        disconnect(ui->btn_left2, &QPushButton::clicked, this, &Transactions::btn_left2_clicked);
        startIndex = startIndex - 5;
        ui->label_left3->setText("TAKAISIN");
        ui->label_left1->setText("");
        ui->label_left2->setText("");
        return;
    }

    for (const auto &transaction : transactions) {
        QJsonObject obj = transaction.toObject();
        QString transaction_type = obj["transaction_type"].toString() == "withdraw"
                                       ? "[NOSTO]  "
                                       : "[TALLETUS]  ";
        displayText += transaction_type;

        auto datetime = QDateTime::fromString(obj["transaction_date"].toString(), Qt::ISODate)
                            .toLocalTime()
                            .toString("d.MM hh:mm");

        displayText += datetime + " ";

        displayText += obj["amount"].toString() + "€\n";
    }

    ui->label_top->setText(displayText);
}

void Transactions::btn_left1_clicked()
{
    startIndex = startIndex + 5;
    getTransactions();
}

void Transactions::btn_left2_clicked()
{
    if (startIndex > 0) {
        startIndex = startIndex - 5;
        getTransactions();
    }
}

void Transactions::btn_left3_clicked()
{
    startIndex = 0;
    getTransactions();
    ui->label_left1->setText("SEURAAVA");
    ui->label_left2->setText("EDELLINEN");
    ui->label_left3->setText("");
    connect(ui->btn_left2, &QPushButton::clicked, this, &Transactions::btn_left2_clicked);
}

void Transactions::btn_right1_clicked() {}

void Transactions::btn_right2_clicked() {}

void Transactions::btn_right3_clicked()
{
    this->close();
}
