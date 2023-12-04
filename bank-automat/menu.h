#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QMainWindow>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

#include "bitcoin.h"
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

    void setAll(const QString &newIdcard,
                const QString &newPIN,
                const QString &newCardType,
                const QString &newIdcustomer,
                const QString &newFname,
                const QString &newLname,
                const QString &newIdaccount,
                const QString &newAccountType,
                const QString &newAccountName,
                const QString &newBalance,
                const QByteArray &newToken,
                bool newBitcoinAccount,
                const QString &newBitcoinBalance,
                const QString &newBitcoinAccountName);

    void setUpMenuTxt();

private:
    Ui::Menu *ui;
    Withdraw *objWithdraw;
    Bitcoin *objBitcoin;
    Transfer *objTransfer;

    QString idcard, PIN, card_type, idcustomer, fname, lname, idaccount, account_type, account_name,
        balance, bitcoin_balance, bitcoin_account_name;

    transactions * objTransactions;

    QByteArray token;

    QNetworkAccessManager *manager;

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
    void updateBalanceSlot();
};
#endif // MENU_H
