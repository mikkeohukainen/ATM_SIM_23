#include "transfer.h"
#include "ui_transfer.h"

Transfer::Transfer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Transfer),

    status(ENTER_ACCOUNT)

{
    ui->setupUi(this);
    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");
    ui->mycentralwidget->setFixedSize(1920, 1009);
    setLabels();
    connectBtns();
    updateLabels();
    qDebug() << "TRANSFER LUOTU";
    qDebug() << "LÄHETTÄJÄN TILI: " + sendingAccountId;

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
    ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_left1->setText("");
    ui->label_left2->setText("");
    ui->label_left3->setText("");

    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_right3->setText("TAKAISIN");

}

void Transfer::updateLabels()
{
    switch (status) {
    case ENTER_ACCOUNT:
      qDebug() << "ENTER_ACCOUNT";
      ui->label_top->setText("SYÖTÄ SAAJAN TILINUMERO\nJA SIIRRETTÄVÄ SUMMA");
      ui->label_left1->setText("Saajan tilinumero: ");
      ui->label_left2->setText("Siirrettävä summa: ");
      ui->txt_right1->setFocus();
      break;
    case ENTER_AMOUNT:
      qDebug() << "ENTER_AMOUNT";
      ui->label_top->setText("SYÖTÄ SAAJAN TILINUMERO\nJA SIIRRETTÄVÄ SUMMA");
      ui->label_left1->setText("Saajan tilinumero: ");
      ui->label_left2->setText("Siirrettävä summa: ");
      ui->txt_right1->setText(receivingAccountId);
      ui->txt_right2->setFocus();
      break;
    case CONFIRM_TRANSFER:
      qDebug() << "CONFIRM_TRANSFER";
      ui->label_top->setText("TARKISTA JA VAHVISTA TILISIIRTO");
      ui->label_left1->setText("Vastaanottajan nimi: ");
      ui->txt_right1->setText(receiverName.toUpper());
      ui->label_left2->setText("Siirrettävä summa: ");
      ui->txt_right2->setText(formattedAmount);
      ui->label_left3->setText("VAHVISTA");
      ui->label_right3->setText("PERUUTA");
      break;
    case TRANSFER_COMPLETE:
      qDebug() << "TRANSFER_COMPLETE";
      ui->label_top->setText("TILISIIRTO TEHTY\nTAPAHTUMAT TIEDOT:");
      ui->label_left1->setText("Vastaanottaja: ");
      ui->txt_right1->setText(receiverName.toUpper());
      ui->label_left2->setText("Siirretty summa: ");
      ui->txt_right2->setText(formattedAmount);
      ui->label_left3->setText("ALOITA ALUSTA");
      ui->label_right3->setText("PÄÄVALIKKO");
      break;
    case ERROR:
      qDebug() << "--ERROR--";
      ui->label_left1->setText("");
      ui->label_left2->setText("");
      ui->label_left3->setText("ALOITA ALUSTA");

      ui->txt_right1->setText("");
      ui->txt_right2->setText("");
      ui->label_right3->setText("PÄÄVALIKKO");
      break;
    default:
      // Käsittely oletustilanteelle tai virhetilanteelle
      break;
    }
}
void Transfer::connectBtns()
{
    connect(ui->btn_left3, &QPushButton::clicked, this, &Transfer::left3ButtonClicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &Transfer::right3ButtonClicked);
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
    connect(ui->btn_numDOT, &QPushButton::clicked, this, &Transfer::decimalButtonClicked);

    connect(ui->btnEnter, &QPushButton::clicked, this, &Transfer::enterButtonClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &Transfer::cancelButtonClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &Transfer::clearButtonClicked);
}

void Transfer::startOver()
{
    qDebug() << "ALOITETAAN ALUSTA";
    status = ENTER_ACCOUNT;
    receivingAccountId = "";
    transferAmount = "";
    setLabels();
    updateLabels();
}

void Transfer::formatAmountText(const QString amount) {
    formattedAmount = amount; // Kopioi alkuperäinen teksti formattedAmount-muuttujaan

    // Tarkista, sisältääkö formattedAmount piste-merkin.
    if (!formattedAmount.contains(".")) {
      // Jos ei sisällä, lisää ".00".
      formattedAmount += ".00 €";
    } else {
      // Jos sisältää piste-merkin, tarkista sen jälkeisten merkkien määrä.
      int decimalCount = formattedAmount.length() - formattedAmount.indexOf('.') - 1;
      if (decimalCount == 1) {
          // Jos on vain yksi merkki piste-merkin jälkeen, lisää "0".
          formattedAmount += "0 €";
      }
    }
}


//void Transfer::confirmTransfer()
//{
//    ui->label_left1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    ui->label_left2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    ui->label_top->setText("TARKISTA JA VAHVISTA TILISIIRTO");
//    ui->label_left1->setText("Saajan nimi: "+receiverName);
//    ui->label_left2->setText("Siirrettävä summa: "+transferAmount);
//}

void Transfer::completeTransfer()
{

}

void Transfer::transferFailed(const QString &errorMessage)
{   updateLabels();
    ui->label_top->setText(errorMessage);
}

void Transfer::left3ButtonClicked()
{
    //Vahvista tilisiiro
    if (status == CONFIRM_TRANSFER) {
      ui->label_top->setText("SUORITETAAN TILISIIRTO");
      QTimer::singleShot(3000, this, &Transfer::startTransfer);
    }

    //Tee uusi siirto
    else if (status == TRANSFER_COMPLETE || status == ERROR) {
      startOver();
    }

}

//void Transfer::right3ButtonClicked()
//{

//    if (status == ENTER_ACCOUNT || status == ENTER_AMOUNT) {
//      this->close();
//    }
//    else if (status == TRANSFER_COMPLETE) {
//      this->close();
//    }
//    else if (status == CONFIRM_TRANSFER){
//      startOver();
//    }
//}


void Transfer::right3ButtonClicked()
{
    if (status == CONFIRM_TRANSFER) startOver();
    else this->close();
}


void Transfer::enterButtonClicked()
{
    switch (status) {
    case ENTER_ACCOUNT:
        if (!receivingAccountId.isEmpty()) {
            status = ENTER_AMOUNT;
            qDebug() << "TILINUMERO TALLENNETTU";
            qDebug() << "VASTAANOTTAJAN TILINUMERO: " + receivingAccountId;
            updateLabels();
        }
        break;
    case ENTER_AMOUNT:
        if (!transferAmount.isEmpty() && !transferAmount.endsWith(".") && !transferAmount.startsWith("0")) {
            status = CONFIRM_TRANSFER;
            qDebug() << "SUMMA TALLENNETTU";
            qDebug() << "SUMMA : " + transferAmount;
            formatAmountText(transferAmount);
            qDebug() << "SUMMASTA TEHTY MUOTOILTU VERSIO";
            qDebug() << "MUOTOILTU SUMMA : " + formattedAmount;
            getReceiverInfo();
            updateLabels();
        }
        break;
    default:
        // Oletus tai virhetilanne
        break;
    }
}

void Transfer::clearButtonClicked()
{
    if (status == ENTER_ACCOUNT) {
        receivingAccountId = "";
        ui->txt_right1->setText(receivingAccountId);
        ui->txt_right1->setFocus();
        qDebug() << "TILINUMERO PYYHITTY";
        qDebug() << "VASTAANOTTAJAN TILINUMERO: " + receivingAccountId;
    }

    else if (status == ENTER_AMOUNT) {
        transferAmount = "";
        ui->txt_right2->setText(transferAmount);
        ui->txt_right2->setFocus();
        qDebug() << "SUMMA PYYHITTY";
        qDebug() << "SIIRTOSUMMA: " +transferAmount;
    }
}

void Transfer::cancelButtonClicked()
{
    startOver();
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
        if (transferAmount.contains(".")) {
            int index = transferAmount.indexOf(".");
            int decimalCount = transferAmount.length() - (index + 1);

            if (decimalCount < 2) { // Salli syöttää enintään kaksi desimaalia
                transferAmount += name.right(1);
                ui->txt_right2->setText(transferAmount);
                ui->txt_right2->setFocus();
            }
        }
        else {
            transferAmount += name.right(1);
            ui->txt_right2->setText(transferAmount);
            ui->txt_right2->setFocus();
        }
    }
}

void Transfer::decimalButtonClicked()
{
    if (status == ENTER_AMOUNT && !transferAmount.contains(".") && !transferAmount.isEmpty()) {
        transferAmount += ".";
        ui->txt_right2->setText(transferAmount);
        ui->txt_right2->setFocus();
    }
}



void Transfer::getReceiverInfo()
{
    qDebug() << "TARKISTETAAN VASTAANOTTAJAN TILI";

    QString site_url = "http://localhost:3000/transfer/receiver-info/"+receivingAccountId;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(token));
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReceiverInfoSlot(QNetworkReply*)));

    reply = networkManager->get(request);

}

void Transfer::getReceiverInfoSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    qDebug() << "RECEIVER DATA: "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    reply->deleteLater();

//    if (json_obj.contains("error")) {
//        // Aseta tilaksi ERROR ja käsittele virhe
//        status = ERROR;
//        QString error_message = json_obj["error"].toString();
//        qDebug() << "Error: " << error_message;
//        transferFailed("Virhe: " + error_message);
//    } else if (json_obj["accountType"].toString() == "bitcoin") {
//        // Käsittele erityistapaus, esimerkiksi bitcoin-tili
//        transferFailed("Vastaanottajan tili ei kelpaa.");
//    } else {
//        // Jos ei virhettä, jatka normaalisti
//        receiverName = json_obj["receiverName"].toString();
//        status = CONFIRM_TRANSFER;
//        updateLabels();
//    }

    if (json_obj["error"].toString() == "Receiver account not found") {
        transferFailed("Tiliä ei löydy.");
    }

    else if (json_obj["accountType"].toString() == "bitcoin") {
        transferFailed("Vastaanottajan tili ei kelpaa.");
    }

    else {
        receiverName = json_obj["receiverName"].toString();
        status = CONFIRM_TRANSFER;
        qDebug() << "TILI OK";
        updateLabels();
    }

}

void Transfer::startTransfer()
{
    qDebug() << "ALOITETAAN SIIRTO";
    QJsonObject jsonObjLogin;
    jsonObjLogin.insert("sendingAccountId", sendingAccountId);
    jsonObjLogin.insert("receivingAccountId", receivingAccountId);
    jsonObjLogin.insert("amount", transferAmount);
    QString site_url = "http://localhost:3000/transfer";

    QNetworkRequest request((site_url));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader(QByteArray("Authorization"), token);

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(transferSlot(QNetworkReply*)));

    reply = networkManager->post(request, QJsonDocument(jsonObjLogin).toJson());
}

void Transfer::transferSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug() << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    reply->deleteLater();
    networkManager->deleteLater();

    status = TRANSFER_COMPLETE;
    qDebug() << "SIIRTO SUORITETTU";
    updateLabels();
}
