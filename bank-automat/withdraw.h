#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QMainWindow>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>

namespace Ui {
class Withdraw;
}

class Withdraw : public QMainWindow
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

    void setVars(const QString &newIdaccount,
                const QString &newAccountType,
                 const QByteArray &newToken);

private:
    Ui::Withdraw *ui;

    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;

    QString idaccount, account_type,
            amount, amountTxt;

    QByteArray token;

    int state;

    void setLabels();
    void connectBtns();
    void defaultAmounts();
    void otherAmount();
    void disconnectBtns();
    void connectOtherAmntBtns();
    void connectConfirmBtns();
    void clearAmount();
    void confirm();
    void withdrawMoney();
    void noFunds();
    void takeMoney();
    void continueTakeMoney();

private slots:
    void btn_left1_clicked();
    void btn_left2_clicked();
    void btn_left3_clicked();
    void btn_right1_clicked();
    void btn_right2_clicked();
    void btn_right3_clicked();

    void number_btn_clicked();
    void btnEnterClicked();
    void btnCancelClicked();
    void btnClearClicked();

    void goBack();

    void startWithdrawal();
    void withdrawMoneySlot(QNetworkReply *reply);
};

#endif // WITHDRAW_H
