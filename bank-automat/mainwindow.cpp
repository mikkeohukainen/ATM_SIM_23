#include "mainwindow.hpp"
#include <QScopedPointer>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new QNetworkAccessManager(this))
    , bitcoinAccount(false)
{
    ui->setupUi(this);

    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");

    setUpLoginLabels();
    connectLoginBtns();
}

MainWindow::~MainWindow()
{
    manager->deleteLater();
    delete ui;
}

void MainWindow::thisClose()
{
    this->close();
}

void MainWindow::number_btn_clicked()
{
    if (state != 0)
    {
        QPushButton * button = qobject_cast<QPushButton*>(sender());
        QString name = button->objectName();

        loginTXT += name.right(1);

        if (state == 1)
        {
            ui->txt_right1->setText(loginTXT);
        }
        else
        {
            ui->txt_right2->setText(loginTXT);
        }
    }
}

void MainWindow::btnEnterClicked()
{
    if (state != 0)
    {
        if (state == 1)
        {
            loginTXT = "";
            ui->txt_right2->setFocus();
            state = 2;
        }
        else
        {
            loginTXT = "";
            state = 0;
            startLogin();
        }
    }
}

void MainWindow::btnCancelClicked()
{
    setUpLoginLabels();
}

void MainWindow::btnClearClicked()
{
    if(state == 1)
    {
        loginTXT.chop(1);
        ui->txt_right1->setText(loginTXT);
    }
    else if(state == 2)
    {
        loginTXT.chop(1);
        ui->txt_right2->setText(loginTXT);
    }
}

void MainWindow::startLogin()
{
    if(ui->txt_right1->text() == "" || ui->txt_right2->text() == "")
    {
        qDebug()<<"Kortin numero tai PIN puuttuu";
        ui->label_top->setText("NUMERO TAI PIN PUUTTUU");
        reset();
    }

    else
    {
        idcard = ui->txt_right1->text();
        PIN = ui->txt_right2->text();

        qDebug()<<idcard;
        qDebug()<<PIN;

        QJsonObject jsonObjLogin;
        jsonObjLogin.insert("idcard", idcard);
        jsonObjLogin.insert("pin", PIN);

        QString url = "http://localhost:3000/login";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = manager->post(request, QJsonDocument(jsonObjLogin).toJson());
        connect(reply, &QNetworkReply::finished, this, &MainWindow::loginSlot);
    }
}

void MainWindow::loginSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();

    qDebug() << response;

    if (response.length() < 2) {
        qDebug()<<"Palvelin ei vastaa";
        ui->label_top->setText("PALVELIN EI VASTAA");
        reset();
    } else if (response == "-4078") {
        qDebug()<<"Virhe tietokantayhteydessä";
        ui->label_top->setText("VIRHE TIETOKANTAYHTEYDESSÄ");
        reset();
    } else if (response != "false" && response.length() > 20) {
        qDebug()<<"Login Ok";
        token = "Bearer " + response;

        getCardInfo();
    } else {
        qDebug()<<"Tunnus ja salasana eivät täsmää";
        ui->label_top->setText("VÄÄRÄ NUMERO TAI PIN KOODI");
        reset();
    }
}

void MainWindow::setUpLoginLabels()
{
    state = 1;
    loginTXT = "";

    ui->label_top->setText("TERVETULOA\n\nKIRJAUDU SISÄÄN");
    ui->label_left3->setText("");
    ui->label_right3->setText("SULJE");
    ui->txt_right1->setText("");
    ui->txt_right2->setText("");

    ui->txt_right2->setEchoMode(QLineEdit::Password);


    ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->txt_right1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->label_left1->setText("KORTIN NUMERO:  ");
    ui->label_left2->setText("PIN KOODI:  ");

    ui->txt_right1->setFocus();

    disconnect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::setUpLoginLabels);
    connect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::thisClose);

    clearData();
}

void MainWindow::connectLoginBtns()
{
    connect(ui->btn_num0, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num1, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num2, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num3, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num4, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num5, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num6, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num7, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num8, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);
    connect(ui->btn_num9, &QPushButton::clicked, this, &MainWindow::number_btn_clicked);

    connect(ui->btnEnter, &QPushButton::clicked, this, &MainWindow::btnEnterClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MainWindow::btnCancelClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::btnClearClicked);

    connect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::thisClose);
}

void MainWindow::reset()
{
    state = 0;

    ui->label_right3->setText("TAKAISIN");

    connect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::setUpLoginLabels);
    disconnect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::thisClose);
}

void MainWindow::getCardInfo()
{
    QString url = "http://localhost:3000/card/" + idcard;
    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::getCardInfoSlot);
}

void MainWindow::getCardInfoSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();
    qDebug() << "DATA : " + response;

    QJsonDocument json_doc = QJsonDocument::fromJson(response);
    QJsonObject json_obj = json_doc.object();
    card_type = json_obj["card_type"].toString();
    idcustomer = QString::number(json_obj["idcustomer"].toInt());

    qDebug() << "card type : " + card_type;
    qDebug() << "idcustomer : " + idcustomer;

    getCustomerInfo();
}

void MainWindow::getCustomerInfo()
{
    QString url = "http://localhost:3000/customer/" + idcustomer;
    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::getCustomerInfoSlot);
}

void MainWindow::getCustomerInfoSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();
    qDebug() << "DATA : " + response;

    QJsonDocument json_doc = QJsonDocument::fromJson(response);
    QJsonObject json_obj = json_doc.object();
    fname = json_obj["fname"].toString();
    lname = json_obj["lname"].toString();

    qDebug() << fname << lname;

    getAccessInfo();
}

void MainWindow::getAccessInfo()
{
    QString url = "http://localhost:3000/account_access/" + idcard;
    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::getAccessInfoSlot);
}

void MainWindow::getAccessInfoSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();
    qDebug() << "DATA : " + response;

    QJsonDocument json_doc = QJsonDocument::fromJson(response);
    QJsonObject json_obj = json_doc.object();
    QString accountsString = json_obj["accounts"].toString();
    accountIDs = accountsString.split(',');

    qDebug() << accountIDs;

    handleDualCard();
}

void MainWindow::handleDualCard()
{
    qDebug() << "accountIDs length: " << accountIDs.length();

    if(card_type != "dual" && accountIDs.length() < 2)
    {
        idaccount = accountIDs.first();
        getAccountInfo();
    }
    else if(card_type == "dual")
    {
        if (!accountIDs.isEmpty()) {
            getAccountType(accountIDs.takeFirst());
        }
        selectAccount();
    }
    else if (card_type != "dual" && accountIDs.length() > 1)
    {
        if (!accountIDs.isEmpty()) {
            getAccountType(accountIDs.takeFirst());
        }


    }
}

void MainWindow::selectAccount()
{
    ui->label_top->setText("VALITSE TILI");
    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left1->setText("DEBIT");
    ui->label_left2->setText("CREDIT");

    connect(ui->btn_left1, &QPushButton::clicked, this, &MainWindow::onDebitSelected);
    connect(ui->btn_left2, &QPushButton::clicked, this, &MainWindow::onCreditSelected);
}

void MainWindow::getAccountType(QString accountID)
{
    QString url = "http://localhost:3000/account/" + accountID;
    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::getAccountTypeSlot);
}

void MainWindow::getAccountTypeSlot()
{
    qDebug() << "Entered getAccountTypeSlot";
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(response);
    QJsonObject json_obj = json_doc.object();

    QString accountId = QString::number(json_obj["idaccount"].toInt());
    QString accountType = json_obj["account_type"].toString();

    accountDetails[accountId] = accountType;

    qDebug() << accountDetails;

    disconnect(manager, &QNetworkAccessManager::finished, this, &MainWindow::getAccountTypeSlot);

    if (!accountIDs.isEmpty()) {
        getAccountType(accountIDs.takeFirst());
    }
    if (accountIDs.isEmpty() && card_type != "dual")
    {
        checkForBitcoin();
    }
}

void MainWindow::onDebitSelected()
{
    for (auto it = accountDetails.begin(); it != accountDetails.end(); ++it) {
        if (it.value() == "bitcoin") {
            bitcoinAccount = true;
            qDebug() << it.value();
            qDebug() << bitcoinAccount;
        }
        if (it.value() == "debit") {
            qDebug() << it.key();
            idaccount = it.key();
            //break;
        }
    }
    disconnect(ui->btn_left1, &QPushButton::clicked, this, &MainWindow::onDebitSelected);
    disconnect(ui->btn_left2, &QPushButton::clicked, this, &MainWindow::onCreditSelected);
    getAccountInfo();
}

void MainWindow::onCreditSelected()
{
    for (auto it = accountDetails.begin(); it != accountDetails.end(); ++it) {
        if (it.value() == "bitcoin") {
            bitcoinAccount = true;
            qDebug() << it.value();
            qDebug() << bitcoinAccount;
        }
        if (it.value() == "credit") {
            qDebug() << it.key();
            idaccount = it.key();
            //break;
        }
    }
    disconnect(ui->btn_left1, &QPushButton::clicked, this, &MainWindow::onDebitSelected);
    disconnect(ui->btn_left2, &QPushButton::clicked, this, &MainWindow::onCreditSelected);
    getAccountInfo();
}

void MainWindow::checkForBitcoin()
{
    qDebug() << "Entered check for bitcoin";
    qDebug() << accountDetails.count();

    if (accountDetails.count() > 1)
    {
        qDebug() << "Parsing accounts";
        for (auto it = accountDetails.begin(); it != accountDetails.end(); ++it) {
            if (it.value() == "credit") {
                qDebug() << it.key();
                idaccount = it.key();
            }
            if (it.value() == "debit") {
                qDebug() << it.key();
                idaccount = it.key();
            }
            if (it.value() == "bitcoin") {
                qDebug() << it.value();
                bitcoinAccount = true;
                qDebug() << bitcoinAccount;
                //break;
            }
        }
        getAccountInfo();
    }
}

void MainWindow::getAccountInfo()
{
    QString url = "http://localhost:3000/account/" + idaccount;
    QNetworkRequest request((url));
    request.setRawHeader(QByteArray("Authorization"), token);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::getAccountInfoSlot);
}

void MainWindow::getAccountInfoSlot()
{
    QScopedPointer<QNetworkReply> reply(qobject_cast<QNetworkReply *>(sender()));
    QByteArray response = reply->readAll();
    qDebug() << "Account data : " + response;

    QJsonDocument json_doc = QJsonDocument::fromJson(response);
    QJsonObject json_obj = json_doc.object();
    account_type = json_obj["account_type"].toString();
    account_name = json_obj["account_name"].toString();
    balance = json_obj["balance"].toString();

    qDebug() << account_type;
    qDebug() << account_name;
    qDebug() << balance;

    setUpMenu();
}

void MainWindow::clearData()
{
    token.clear();
    bitcoinAccount = false;
    idcard.clear();
    PIN.clear();
    card_type.clear();
    idcustomer.clear();
    fname.clear();
    lname.clear();
    accountIDs.clear();
    idaccount.clear();
    account_type.clear();
    account_name.clear();
    balance.clear();
    account_owner.clear();
    accountDetails.clear();
    bitcoin_balance.clear();
    bitcoin_account_name.clear();
}

void MainWindow::disconnectLoginBtns()
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

void MainWindow::setUpMenu()
{
    objMenu = new Menu(this);
    objMenu->setAll(idcard,
                    PIN,
                    card_type,
                    idcustomer,
                    fname,
                    lname,
                    idaccount,
                    account_type,
                    account_name,
                    balance,
                    token,
                    bitcoinAccount,
                    bitcoin_balance,
                    bitcoin_account_name);
    objMenu->setUpMenuTxt();
    objMenu->showFullScreen();
    setUpLoginLabels();
    disconnectLoginBtns();
    connectLoginBtns();
}
