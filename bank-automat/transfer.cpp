#include "transfer.h"
#include "ui_transfer.h"

//Konstruktori. Alustaa Transfer-ikkunan ja asettaa alkuasetukset.
Transfer::Transfer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Transfer)
    , status(ENTER_ACCOUNT)
{
    ui->setupUi(this);
    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");
    //    ui->mycentralwidget->setFixedSize(1920, 1080);
    setLabels();
    connectBtns();
    updateLabels();
}

// Destruktori. Vapauttaa käyttöliittymäresurssit.
Transfer::~Transfer()
{
    delete ui;
}

// Asettaa lähettäjän tilinumeron ja tokenin.
void Transfer::setVars(const QString &newSendingAccountId, const QByteArray &newToken)
{
    sendingAccountId = newSendingAccountId;
    token = newToken;
    qDebug() << "LÄHETTÄJÄN TILI: " + sendingAccountId;
}

// Asettaa tarvittavat tekstit ja asettelut käyttöliittymään.
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
    ui->label_right3->setText("PÄÄVALIKKO");
}

// Päivittää käyttöliittymän tekstejä tilan mukaisesti.
void Transfer::updateLabels()
{
    switch (status) {
    case ENTER_ACCOUNT:
        ui->label_top->setText("SYÖTÄ SAAJAN TILINUMERO\nJA SIIRRETTÄVÄ SUMMA");
        ui->label_left1->setText("Saajan tilinumero: ");
        ui->label_left2->setText("Siirrettävä summa: ");
        ui->txt_right1->setFocus();
        break;
    case ENTER_AMOUNT:
        ui->label_top->setText("SYÖTÄ SAAJAN TILINUMERO\nJA SIIRRETTÄVÄ SUMMA");
        ui->label_left1->setText("Saajan tilinumero: ");
        ui->label_left2->setText("Siirrettävä summa: ");
        ui->txt_right1->setText(receivingAccountId);
        ui->txt_right2->setFocus();
        break;
    case WAITING_CONFIRMATION:;
        ui->label_top->setText("TARKISTA JA VAHVISTA TILISIIRTO");
        ui->label_left1->setText("Vastaanottajan nimi: ");
        ui->txt_right1->setText(receiverName.toUpper());
        ui->label_left2->setText("Siirrettävä summa: ");
        ui->txt_right2->setText(formattedAmount);
        ui->label_left3->setText("VAHVISTA");
        ui->label_right3->setText("PERUUTA");
        break;
    case TRANSFER_COMPLETE:
        ui->label_top->setText("TILISIIRTO TEHTY\nTAPAHTUMAT TIEDOT:");
        ui->label_left1->setText("Vastaanottaja: ");
        ui->txt_right1->setText(receiverName.toUpper());
        ui->label_left2->setText("Siirretty summa: ");
        ui->txt_right2->setText(formattedAmount);
        ui->label_left3->setText("ALOITA ALUSTA");
        ui->label_right3->setText("PÄÄVALIKKO");
        break;
    case ERROR:
        ui->label_left1->setText("");
        ui->label_left2->setText("");
        ui->label_left3->setText("ALOITA ALUSTA");
        ui->txt_right1->setText("");
        ui->txt_right2->setText("");
        ui->label_right3->setText("PÄÄVALIKKO");
        break;
    }
}

// Yhdistää käyttöliittymän napit niiden toimintoihin.
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

// Palauttaa tilisiirron alkutilaan.
void Transfer::startOver()
{
    qDebug() << "ALOITETAAN ALUSTA";
    status = ENTER_ACCOUNT;
    receivingAccountId = "";
    transferAmount = "";
    setLabels();
    updateLabels();
}

// Tarkistaa, onko syötetty summa kelvollinen.
bool Transfer::isAmountValid(const QString &amount)
{
    if (amount.isEmpty()) {
        ui->txt_right2->setText("SUMMA EI VOI OLLA TYHJÄ");
        return false;
    } else if (amount.endsWith(".")) {
        ui->txt_right2->setText("SUMMA EI VOI PÄÄTTYÄ PISTEESEEN");
        return false;
    } else if (amount.startsWith("0")) {
        ui->txt_right2->setText("SUMMA EI VOI ALKAA NOLLALLA");
        return false;
    }
    return true;
}

// Muotoilee syötetyn summan tulostusta varten: Summan perään lisätään
// kaksi desimaalia ja €-merkki
void Transfer::formatAmountText(const QString &amount)
{
    formattedAmount = amount; // Kopioidaan alkuperäinen teksti formattedAmount-muuttujaan
    if (!formattedAmount.contains(".")) {
      formattedAmount += ".00";
    } else {
      // Jos sisältää piste-merkin, tarkistetaan sen jälkeisten merkkien määrä.
      // Lisätään tarvittaessa "0"
      int decimalCount = formattedAmount.length() - formattedAmount.indexOf('.') - 1;
      if (decimalCount == 1) {
          formattedAmount += "0";
      }
    }
    formattedAmount += " €";
}

// Aloittaa siirron vahvistusprosessin.
void Transfer::startConfirm()
{
    if (isAmountValid(transferAmount)) {
      ui->label_top->setText("HAETAAN VASTAANOTTAJAN TIETOJA");
      qDebug() << "SUMMA : " + transferAmount;
      formatAmountText(transferAmount);
      qDebug() << "MUOTOILTU SUMMA : " + formattedAmount;
      QTimer::singleShot(1000, this, &Transfer::getReceiverInfo);

    }
    else { // Jos summa ei ole kelvollinen, nollataan transferAmount.
      ui->txt_right2->setFocus();
      ui->label_left3->setText("");
      transferAmount = "";
      qDebug() << "transferAmount NOLLATTU";
    }
}

// Käsittelee tilisiirron aikana ilmenevät virheet.
void Transfer::handleError(const QString &errorMessage)
{
    status = ERROR;

    if (errorMessage == "Receiver account not found") {
      ui->label_top->setText("VIRHE: TILIÄ EI LÖYDY.");
    } else if (errorMessage == "Insufficient funds") {
      ui->label_top->setText("VIRHE: TILIN KATE EI RIITÄ.");
    } else if (errorMessage == "Error connecting to the database") {
      ui->label_top->setText("VIRHE: EI YHTEYTTÄ TIETOKANTAAN.");
    } else { // Jos jokin muu virhe, tulostetaan virheviesti sellaisenaan
      ui->label_top->setText("VIRHE: " + errorMessage + "\n\nTAPAHTUMA PERUUTETTU");
    }
    updateLabels();
}

// Määrittelee vasemman puolen alimman napin toiminnallisuuden.
void Transfer::left3ButtonClicked()
{
    // Napin teksti: VAHVISTA. Aloittaa tilisiirron suorittamisen.
    if (status == WAITING_CONFIRMATION) {
      ui->label_top->setText("SUORITETAAN TILISIIRTO");
      ui->label_left3->setText("");
      ui->label_left3->setText("");
      QTimer::singleShot(1500, this, &Transfer::startTransfer);
    }

    // Napin teksti: ALOITA ALUSTA. Palauttaa tilisiirto-toiminnon alkutilaan.
    else if (status == TRANSFER_COMPLETE || status == ERROR) {
      startOver();
    }

    // Napin teksti: JATKA. Siirtyy syötettyjen tietojen tarkistukseen
    else if (status == ENTER_AMOUNT) {
      startConfirm();
    }
}

// Määrittelee oikean puolen alimman napin toiminnallisuuden.
void Transfer::right3ButtonClicked()
{
    // Napin teksti: PERUUTA. Palauttaa tilisiirto-toiminnon alkutilaan
    if (status == WAITING_CONFIRMATION)
      startOver();

    // Napin teksti: PÄÄVALIKKO. Sulkee Tilisiirto-ikkunan
    else
      this->close();
}

// Määrittelee Enter-napin toiminnallisuuden.
void Transfer::enterButtonClicked()
{
    if (status == ENTER_ACCOUNT && !receivingAccountId.isEmpty()) {
      status = ENTER_AMOUNT;
      qDebug() << "TILINUMERO TALLENNETTU";
      qDebug() << "VASTAANOTTAJAN TILINUMERO: " + receivingAccountId;
      updateLabels();
    } else if (status == ENTER_AMOUNT) {
      startConfirm();
    }
}

// Määrittelee Clear-napin toiminnallisuuden.
void Transfer::clearButtonClicked()
{
    if (status == ENTER_ACCOUNT) {
      receivingAccountId = "";
      ui->txt_right1->setText(receivingAccountId);
      ui->txt_right1->setFocus();
      qDebug() << "TILINUMERO NOLLATTU";
    }

    else if (status == ENTER_AMOUNT) {
      transferAmount = "";
      ui->txt_right2->setText(transferAmount);
      ui->txt_right2->setFocus();
      ui->label_left3->setText("");
      qDebug() << "SUMMA NOLLATTU";
    }
}

// Määrittelee Cancel-napin toiminnallisuuden.
void Transfer::cancelButtonClicked()
{
    startOver();
}

// Määrittelee numeronappien toiminnallisuuden.
// Napin käytössä vain jos tila on ENTER_ACCOUNT tai ENTER_AMOUNT.
void Transfer::numberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString name = button->objectName();

    if (status == ENTER_ACCOUNT) {
        receivingAccountId += name.right(1);
        ui->txt_right1->setText(receivingAccountId);
        ui->txt_right1->setFocus();
    }

    else if (status == ENTER_AMOUNT) {
        // Sallitaan summaa syöttäessä vain kaksi desimaalia
        if (transferAmount.contains(".")) {
          int index = transferAmount.indexOf(".");
          int decimalCount = transferAmount.length() - (index + 1);

          if (decimalCount < 2) {
              transferAmount += name.right(1);
          }
        } else {
          transferAmount += name.right(1);
        }

        ui->txt_right2->setText(transferAmount);
        ui->txt_right2->setFocus();

        if (!transferAmount.isEmpty()) {
          ui->label_left3->setText("JATKA");
        } else {
          ui->label_left3->setText("");
        }
    }
}

// Määrittelee desimaalinapin toiminnallisuuden.
void Transfer::decimalButtonClicked()
{
    if (status == ENTER_AMOUNT && !transferAmount.contains(".") && !transferAmount.isEmpty()) {
        transferAmount += ".";
        ui->txt_right2->setText(transferAmount);
        ui->txt_right2->setFocus();
    }
}

// Hakee vastaanottajan tilin tiedot.
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

// Käsittelee vastaanottajan tilin tietojen vastaanoton.
void Transfer::getReceiverInfoSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    qDebug() << "RECEIVER DATA: " + response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    reply->deleteLater();
    networkManager->deleteLater();

    if (receivingAccountId == sendingAccountId) {
        handleError("VASTAANOTTAJAN TILI EI VOI\nOLLA SAMA KUIN LÄHETTÄJÄN");
    } else if (response_data.length() < 2) {
        handleError("PALVELIN EI VASTAA");
    } else if (json_obj.contains("error")) {
        QString error_message = json_obj["error"].toString();
        qDebug() << "Error: " << error_message;
        handleError(error_message);
    } else if (json_obj["accountType"].toString() == "bitcoin") {
        handleError("VASTAANOTTAJAN TILI EI VOI OLLA BITCOIN-TILI");
    } else if (json_obj.contains("receiverName")){
        // Jos ei virhettä ja receiverName löytyy, otetaan se talteen
        receiverName = json_obj["receiverName"].toString();
        qDebug() <<receiverName;
        status = WAITING_CONFIRMATION;
        updateLabels();
    }
}

// Aloittaa tilisiirron.
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

// Käsittelee tilisiirron tulokset
void Transfer::transferSlot(QNetworkReply *reply) {
    response_data = reply->readAll();
    qDebug() << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    reply->deleteLater();
    networkManager->deleteLater();

    if (json_obj["message"].toString() == "Transfer successful") {
        status = TRANSFER_COMPLETE;
        emit updateSenderBalance();
        qDebug() << "SIIRTO SUORITETTU";
        updateLabels();
    } else if (json_obj.contains("error")) {
        QString error_message = json_obj["error"].toString();
        qDebug() << "Error: " << error_message;
        handleError(error_message);
    } else if (response_data.length() < 2) {
        handleError("PALVELIN EI VASTAA");
    }
}
