#ifndef TRANSFER_H
#define TRANSFER_H

#include <QMainWindow>
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

private:
    Ui::Transfer *ui;
    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;

    QString sendingAccountId, receivingAccountId,
        transferAmount, receiverName;

    QByteArray token;

    enum TransferStatus {
        ENTER_ACCOUNT,
        ENTER_AMOUNT,
        VALIDATE_DETAILS,
        CONFIRM_TRANSFER,
        TRANSFER_RESULT
    };

    TransferStatus status;

    void setLabels();
    void connectBtns();
    void disconnectBtns();

    void inputReceiverAccount();
    void inputTransferAmount();
    void confirmTransfer();
    void completeTransfer();
    void transferFailed(const QString &errorMessage);

private slots:
    void functionButtonClicked();
    void enterButtonClicked();
    void clearButtonClicked();
    void cancelButtonClicked();
    void numberButtonClicked();

    void startTransfer();
    void transferMoneySlot(QNetworkReply *reply);
};

#endif // TRANSFER_H
