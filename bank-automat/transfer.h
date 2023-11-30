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
    void updateSenderBalance();

private:
    Ui::Transfer *ui;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QByteArray response_data;

    QString sendingAccountId, receivingAccountId,
        transferAmount, receiverName, formattedAmount;

    QByteArray token;

    enum TransferStatus {
        ENTER_ACCOUNT,
        ENTER_AMOUNT,
        WAITING_CONFIRMATION,
        TRANSFER_COMPLETE,
        ERROR
    };

    TransferStatus status;

    void setLabels();
    void updateLabels();
    void connectBtns();
    void startOver();
    bool isAmountValid(const QString &amount);
    void formatAmountText(const QString &amount);

    void startConfirm();
    void handleError(const QString &errorMessage);


private slots:
//    void functionButtonClicked();
    void left3ButtonClicked();
    void right3ButtonClicked();
    void enterButtonClicked();
    void clearButtonClicked();
    void cancelButtonClicked();
    void numberButtonClicked();
    void decimalButtonClicked();

    void getReceiverInfo();
    void getReceiverInfoSlot(QNetworkReply *reply);
    void startTransfer();
    void transferSlot(QNetworkReply *reply);
};

#endif // TRANSFER_H
