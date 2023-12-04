#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QMainWindow>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

#include "withdraw.h"

#include "transactions.h"

#include "transfer.h"


namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

    void setAll(const QString &newIdcard, const QString &newPIN, const QString &newCardType,
                    const QString &newIdcustomer, const QString &newFname, const QString &newLname,
                    const QString &newIdaccount, const QString &newAccountType,
                    const QString &newAccountName, const QString &newBalance,
                    const QByteArray &newToken, bool newBitcoinAccount);

    void setUpMenuTxt();

private:
    Ui::Menu *ui;
    Withdraw *objWithdraw;

    transactions * objTransactions;

    Transfer *objTransfer;


    QString idcard, PIN, card_type, idcustomer,
            fname, lname, idaccount, account_type,
            account_name, balance;

    QByteArray token;

    QNetworkAccessManager *getBalanceManager;
    QNetworkReply *reply_balance;
    QByteArray response_data_balance;

    bool bitcoinAccount;

    void connectMenuBtns();

private slots:
    void btn_left1_clicked();
    void btn_left2_clicked();
    void btn_left3_clicked();
    void btn_right1_clicked();
    void btn_right2_clicked();
    void btn_right3_clicked();
    void updateBalance();
    void updateBalanceSlot(QNetworkReply *reply);
};
#endif // MENU_H
