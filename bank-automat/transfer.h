#ifndef TRANSFER_H
#define TRANSFER_H

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>


namespace Ui {
class Transfer;
}

class Transfer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Transfer(QWidget *parent = nullptr);
    ~Transfer();

    void setVars(const QString &newSendingAccountId, const QByteArray &newToken);

signals:
    void updateSenderBalance(); // Päivittää lähettäjän saldon.

private:
    Ui::Transfer *ui;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QByteArray response_data;

    QString sendingAccountId, receivingAccountId,
        transferAmount, receiverName, formattedAmount;

    QByteArray token;

    // Tilan seuranta
    enum TransferStatus {
        ENTER_ACCOUNT,
        ENTER_AMOUNT,
        WAITING_CONFIRMATION,
        TRANSFER_COMPLETE,
        ERROR
    };
    TransferStatus status;

    // Verkkopyyntöjen tyypit
    enum RequestType {
        NONE,
        RECEIVER_INFO,
        TRANSFER
    };
    RequestType currentRequestType;
    bool isRequestPending; // Onko verkkopyyntö meneillään

    void setLabels(); // Asettaa tarvittavat tekstit ja asettelut käyttöliittymään.
    void updateLabels(); // Päivittää käyttöliittymän tekstejä tilan mukaisesti.
    void connectBtns(); // Yhdistää käyttöliittymän napit niiden toimintoihin.
    void startOver(); // Palauttaa tilisiirron alkutilaan.
    bool isAmountValid(const QString &amount); // Tarkistaa, onko syötetty summa kelvollinen.
    void formatAmountText(const QString &amount); // Muotoilee syötetyn summan tulostusta varten.

    void prepareForConfirmation(); // Valmistelee siirron vahvistusprosessin.
    void handleError(const QString &errorMessage); // Käsittelee tilisiirron aikana ilmenevät virheet.


private slots:
    void left3ButtonClicked();
    void right3ButtonClicked();
    void enterButtonClicked();
    void clearButtonClicked();
    void cancelButtonClicked();
    void numberButtonClicked();
    void decimalButtonClicked();

    void networkManagerFinished(QNetworkReply* reply); // Käsittelee valmiin verkkopyynnön.

    void getReceiverInfo(); // Hakee vastaanottajan tilin tiedot.
    void getReceiverInfoSlot(QNetworkReply *reply); // Käsittelee vastaanottajan tilin tietojen vastaanoton.

    void startTransfer(); // Aloittaa tilisiirron.
    void transferSlot(QNetworkReply *reply); // Käsittelee tilisiirron tulokset
};

#endif // TRANSFER_H
