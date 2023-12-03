#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , bitcoinAccount(false)
{
    ui->setupUi(this);

    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");

    setUpLoginLabels();
    connectLoginBtns();

    postManager = new QNetworkAccessManager(this);
    getCardManager = new QNetworkAccessManager(this);
    getCustomerManager = new QNetworkAccessManager(this);
    getAccessManager = new QNetworkAccessManager(this);
    getTypeManager = new QNetworkAccessManager(this);
    getAccountManager = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    postManager->deleteLater();
    getCardManager->deleteLater();
    getCustomerManager->deleteLater();
    getAccessManager->deleteLater();
    getTypeManager->deleteLater();
    getAccountManager->deleteLater();

    delete ui;
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

            disconnect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
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
        jsonObjLogin.insert("idcard",idcard);
        jsonObjLogin.insert("pin",PIN);

        QString site_url = "http://localhost:3000/login";
        QNetworkRequest request((site_url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));

        reply_login = postManager->post(request, QJsonDocument(jsonObjLogin).toJson());
    }
}

void MainWindow::loginSlot(QNetworkReply *reply)
{
    response_data_login=reply->readAll();

    qDebug() << response_data_login;

    if(response_data_login.length()<2){
        qDebug()<<"Palvelin ei vastaa";
        ui->label_top->setText("PALVELIN EI VASTAA");
        reset();
    }
    else if(response_data_login == "-4078")
    {
        qDebug()<<"Virhe tietokantayhteydessä";
        ui->label_top->setText("VIRHE TIETOKANTAYHTEYDESSÄ");
        reset();
    }
    else if(response_data_login!="false" && response_data_login.length()>20)
    {
        qDebug()<<"Login Ok";
        token="Bearer "+response_data_login;

        getCardInfo();
    }
    else
    {
        qDebug()<<"Tunnus ja salasana eivät täsmää";
        ui->label_top->setText("VÄÄRÄ NUMERO TAI PIN KOODI");
        reset();
    }

    reply->deleteLater();
}

void MainWindow::setUpLoginLabels()
{
    state = 1;
    loginTXT = "";

    ui->label_top->setText("TERVETULOA\n\nKIRJAUDU SISÄÄN");
    ui->label_left3->setText("");
    ui->label_right3->setText("");
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
}

void MainWindow::reset()
{
    state = 0;

    ui->label_right3->setText("TAKAISIN");

    connect(ui->btn_right3, &QPushButton::clicked, this, &MainWindow::setUpLoginLabels);
}

void MainWindow::getCardInfo()
{
    QString site_url = "http://localhost:3000/card/"+idcard;
    QNetworkRequest request((site_url));

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    connect(getCardManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCardInfoSlot(QNetworkReply*)));

    reply_card = getCardManager->get(request);
}

void MainWindow::getCardInfoSlot(QNetworkReply *reply)
{
    response_data_card = reply->readAll();
    qDebug() << "DATA : "+response_data_card;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data_card);
    QJsonObject json_obj = json_doc.object();
    card_type = json_obj["card_type"].toString();
    idcustomer = QString::number(json_obj["idcustomer"].toInt());

    qDebug() << "card type : "+card_type;
    qDebug() << "idcustomer : "+idcustomer;

    reply->deleteLater();

    getCustomerInfo();
}

void MainWindow::getCustomerInfo()
{
    QString site_url = "http://localhost:3000/customer/"+idcustomer;
    QNetworkRequest request((site_url));

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    connect(getCustomerManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCustomerInfoSlot(QNetworkReply*)));

    reply_customer = getCustomerManager->get(request);
}

void MainWindow::getCustomerInfoSlot(QNetworkReply *reply)
{
    response_data_customer = reply->readAll();
    qDebug() << "DATA : "+response_data_customer;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data_customer);
    QJsonObject json_obj = json_doc.object();
    fname = json_obj["fname"].toString();
    lname = json_obj["lname"].toString();

    qDebug() << fname << lname;

    reply->deleteLater();

    getAccessInfo();
}

void MainWindow::getAccessInfo()
{
    QString site_url = "http://localhost:3000/account_access/"+idcard;
    QNetworkRequest request((site_url));

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    connect(getAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccessInfoSlot(QNetworkReply*)));

    reply_access = getAccessManager->get(request);
}

void MainWindow::getAccessInfoSlot(QNetworkReply *reply)
{
    response_data_access = reply->readAll();
    qDebug() << "DATA : "+response_data_access;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data_access);
    QJsonObject json_obj = json_doc.object();
    QString accountsString = json_obj["accounts"].toString();
    accountIDs = accountsString.split(',');

    qDebug() << accountIDs;

    reply->deleteLater();

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
    qDebug() << "Entered getAccountType";

    QString site_url = "http://localhost:3000/account/"+accountID;
    QNetworkRequest request((site_url));

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    connect(getTypeManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccountTypeSlot(QNetworkReply*)));

    reply_type = getTypeManager->get(request);

    qDebug() << "Sending request for account ID:" << accountID;

}

void MainWindow::getAccountTypeSlot(QNetworkReply *reply)
{
    qDebug() << "Entered getAccountTypeSlot";

    response_data_type = reply->readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data_type);
    QJsonObject json_obj = json_doc.object();

    QString accountId = QString::number(json_obj["idaccount"].toInt());
    QString accountType = json_obj["account_type"].toString();

    accountDetails[accountId] = accountType;

    qDebug() << accountDetails;

    disconnect(getTypeManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccountTypeSlot(QNetworkReply*)));

    reply->deleteLater();

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
    QString site_url = "http://localhost:3000/account/"+idaccount;
    QNetworkRequest request((site_url));

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    connect(getAccountManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccountInfoSlot(QNetworkReply*)));

    reply_account = getAccountManager->get(request);
}

void MainWindow::getAccountInfoSlot(QNetworkReply *reply)
{
    response_data_account = reply->readAll();
    qDebug() << "Account data : "+response_data_account;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data_account);
    QJsonObject json_obj = json_doc.object();
    account_type = json_obj["account_type"].toString();
    account_name = json_obj["account_name"].toString();
    balance = json_obj["balance"].toString();

    qDebug() << account_type;
    qDebug() << account_name;
    qDebug() << balance;

    reply->deleteLater();

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

    response_data_login.clear();
    response_data_card.clear();
    response_data_customer.clear();
    response_data_access.clear();
    response_data_account.clear();
    response_data_type.clear();
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

void MainWindow::disconnectNetworks()
{
    disconnect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    disconnect(getCardManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCardInfoSlot(QNetworkReply*)));
    disconnect(getCustomerManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCustomerInfoSlot(QNetworkReply*)));
    disconnect(getAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccessInfoSlot(QNetworkReply*)));
    disconnect(getTypeManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccountTypeSlot(QNetworkReply*)));
    disconnect(getAccountManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAccountInfoSlot(QNetworkReply*)));
}

void MainWindow::setUpMenu()
{
    disconnectNetworks();
    objMenu = new Menu(this);
    objMenu->setAll(idcard, PIN, card_type, idcustomer,
                    fname, lname, idaccount, account_type,
                    account_name, balance, token, bitcoinAccount);
    objMenu->setUpMenuTxt();
    objMenu->showMaximized();
    //clearData();
    setUpLoginLabels();
    disconnectLoginBtns();
    connectLoginBtns();
}
