#include "bitcoin.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include "ui_bitcoin.h"

Bitcoin::Bitcoin(const QString &debit_account_id,
                 const QString &debit_account_name,
                 const QByteArray &token,
                 QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Bitcoin)
    , manager(new QNetworkAccessManager(this))
    , token(token)
    , state(State::MAIN)
    , debit_account_id(debit_account_id)
    , debit_account_name(debit_account_name)

{
    ui->setupUi(this);

    ui->mycentralwidget->setStyleSheet(
        "QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");

    connect(this, &Bitcoin::bitcoinAccountReceived, this, &Bitcoin::setLabels);
    connect(this, &Bitcoin::bitcoinPriceReceived, this, &Bitcoin::setLabels);
    connect(this, &Bitcoin::eurosToBitcoinsReceived, this, &Bitcoin::setLabels);
    connect(this, &Bitcoin::stateChanged, this, &Bitcoin::setLabels);
    connect(this, &Bitcoin::bitcoinsBought, this, &Bitcoin::setLabels);

    connectMainBtns();
    setLabels();
    getBitcoinAccount();
    getBitcoinPrice();
}

Bitcoin::~Bitcoin()
{
    delete ui;
}

void Bitcoin::setLabels()
{
    ui->label_right3->setText("TAKAISIN");

    switch (state) {
    case State::MAIN: {
        disconnectBtns();
        connectMainBtns();
        // Remove trailing zeros
        while (bitcoin_balance.endsWith("0")) {
            bitcoin_balance.chop(1);
        }
        ui->label_top->setText(("TILI: " + bitcoin_account_name + "\n\nSALDO: " + bitcoin_balance
                                + "₿ (" + bitcoin_balance_eur + "€)"
                                + "\n\nKURSSI: " + bitcoin_price + "€")
                                   .toUpper());
        ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->txt_right2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        connect(ui->btn_left1, &QPushButton::clicked, this, &Bitcoin::btn_left1_clicked);
        ui->label_left1->setText("OSTA BITCOINEJA");
        ui->label_left2->clear();
        ui->label_left3->clear();
        ui->txt_right1->clear();
        ui->txt_right2->clear();
        amount_txt.clear();
        btc_amount_txt.clear();
        break;
    }
    case State::BUY: {
        disconnectBtns();
        connectBuyBtns();
        connect(ui->btn_right1, &QPushButton::clicked, this, &Bitcoin::btn_right1_clicked);
        connect(ui->btn_right3, &QPushButton::clicked, this, &Bitcoin::btn_right3_clicked);
        ui->label_top->setText("SYÖTÄ SUMMA EUROINA JA PAINA ENTER");
        ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->label_left1->setText("SUMMA:  ");
        ui->txt_right1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->txt_right1->setFocus();
        break;
    }
    case State::CONFIRM:

        ui->label_left1->clear();
        ui->txt_right1->clear();
        ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->txt_right1->setText("VAHVISTA");
        ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        ui->label_top->setText("VAHVISTA OSTO\n\nSUMMA:  " + amount_txt + "€"
                               + "\nMÄÄRÄ: " + btc_amount_txt
                               + "₿"
                                 "\nKURSSI:  "
                               + bitcoin_price
                               + "€\n"
                                 "VELOITETTAVA TILI:  "
                               + debit_account_name);
        break;
    case State::NO_FUNDS:
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Bitcoin::btn_right3_clicked);
        ui->label_left1->clear();
        ui->txt_right1->clear();
        ui->label_top->setText("KATE EI RIITÄ");
        break;
    case State::DATABASE_CONN_ERR:
        ui->label_left1->clear();
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Bitcoin::btn_right3_clicked);
        ui->txt_right1->setText("");
        ui->label_right3->setText("SULJE");
        ui->label_top->setText("VIRHE TIETOKANTAYHTEYDESSÄ");
        break;
    case State::NETWORK_CONN_ERR:
        ui->label_left1->clear();
        disconnectBtns();
        connect(ui->btn_right3, &QPushButton::clicked, this, &Bitcoin::btn_right3_clicked);
        ui->txt_right1->setText("");
        ui->label_right3->setText("SULJE");
        ui->label_top->setText("PALVELIN EI VASTAA");
        break;
    }
}

void Bitcoin::connectMainBtns()
{
    connect(ui->btn_left1, &QPushButton::clicked, this, &Bitcoin::btn_left1_clicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Bitcoin::btn_right3_clicked);
}

void Bitcoin::disconnectBtns()
{
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

    ui->btnEnter->disconnect();
    ui->btnCancel->disconnect();
    ui->btnClear->disconnect();

    ui->btn_left1->disconnect();
    ui->btn_left2->disconnect();
    ui->btn_left3->disconnect();
    ui->btn_right1->disconnect();
    ui->btn_right2->disconnect();
    ui->btn_right3->disconnect();
}

void Bitcoin::connectBuyBtns()
{
    connect(ui->btn_num0, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num1, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num2, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num3, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num4, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num5, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num6, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num7, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num8, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);
    connect(ui->btn_num9, &QPushButton::clicked, this, &Bitcoin::number_btn_clicked);

    connect(ui->btnEnter, &QPushButton::clicked, this, &Bitcoin::enter_btn_clicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &Bitcoin::cancel_btn_clicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &Bitcoin::clear_btn_clicked);
}

void Bitcoin::getBitcoinAccount()
{
    QString url = "http://localhost:3000/bitcoin/account/" + debit_account_id;
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Bitcoin::getBitcoinAccountSlot);
}

void Bitcoin::getBitcoinAccountSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray data = reply->readAll();
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    bitcoin_account_name = obj["account_name"].toString();
    bitcoin_balance = obj["bitcoin_balance"].toString();
    bitcoin_balance_eur = obj["bitcoin_balance_eur"].toString();
    emit bitcoinAccountReceived();
}

void Bitcoin::getBitcoinPrice()
{
    QString url = "http://localhost:3000/bitcoin/lastprice";
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Bitcoin::getBitcoinPriceSlot);
}

void Bitcoin::getBitcoinPriceSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    bitcoin_price = reply->readAll();
    emit bitcoinPriceReceived();
}

void Bitcoin::getEurosToBitcoins()
{
    QUrl url("http://localhost:3000/bitcoin/tobtc");
    QUrlQuery query;
    query.addQueryItem("euros", amount_txt);
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Bitcoin::getEurosToBitcoinsSlot);
}

void Bitcoin::getEurosToBitcoinsSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray data = reply->readAll();
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    btc_amount_txt = obj["btc"].toString();
    emit eurosToBitcoinsReceived();
}

void Bitcoin::buyBitcoins()
{
    QString url = "http://localhost:3000/bitcoin/buy";
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Authorization"), token);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["idaccount"] = debit_account_id.toInt();
    json["euros"] = amount_txt.toInt();
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &Bitcoin::buyBitcoinsSlot);
}

void Bitcoin::buyBitcoinsSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    auto data = reply->readAll();
    auto obj = QJsonDocument::fromJson(data).object();

    if (obj["error"].toString() == "Insufficient balance") {
        state = State::NO_FUNDS;
        emit stateChanged();
    } else if (obj["error"].toString() == "Error connecting to the database") {
        state = State::DATABASE_CONN_ERR;
        emit stateChanged();
    } else if (data.length() < 2) {
        state = State::NETWORK_CONN_ERR;
        emit stateChanged();
    } else {
        bitcoin_balance = obj["new_bitcoin_balance"].toString();
        getEurosToBitcoins();
        emit bitcoinsBought();
    }
}

void Bitcoin::enter_btn_clicked()
{
    if (!amount_txt.isEmpty() && state == State::BUY) {
        getEurosToBitcoins();
        state = State::CONFIRM;
        emit stateChanged();
    }
}

void Bitcoin::cancel_btn_clicked()
{
    amount_txt.clear();
    ui->txt_right1->setText(amount_txt);
}

void Bitcoin::clear_btn_clicked()
{
    amount_txt.chop(1);
    ui->txt_right1->setText(amount_txt);
}

void Bitcoin::number_btn_clicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString name = button->objectName();

    if (amount_txt.length() < 7) {
        amount_txt += name.rightRef(1);
    }
    ui->txt_right1->setText(amount_txt);
}

void Bitcoin::btn_left1_clicked()
{
    disconnectBtns();
    if (state == State::MAIN) {
        state = State::BUY;
        connectBuyBtns();
        emit stateChanged();
    }
}

void Bitcoin::btn_right1_clicked()
{
    if (state == State::BUY && !amount_txt.isEmpty()) {
        state = State::CONFIRM;
        emit stateChanged();
    } else if (state == State::CONFIRM) {
        buyBitcoins();
        state = State::MAIN;
        emit stateChanged();
    }
}

void Bitcoin::btn_right3_clicked()
{
    amount_txt.clear();
    btc_amount_txt.clear();
    disconnectBtns();
    if (state == State::MAIN || state == State::DATABASE_CONN_ERR
        || state == State::NETWORK_CONN_ERR) {
        this->close();
    } else if (state == State::BUY) {
        connectMainBtns();
        state = State::MAIN;
        emit stateChanged();
    } else if (state == State::CONFIRM || state == State::NO_FUNDS) {
        ui->txt_right1->clear();
        connectBuyBtns();
        state = State::BUY;
        emit stateChanged();
    }
}
