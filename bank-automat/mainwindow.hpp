#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Menu *objMenu;
    QNetworkAccessManager *manager;

    QString idcard, PIN;
    QByteArray token;
    QString card_type, idcustomer;
    QString fname, lname;
    QStringList accountIDs;
    QString idaccount;
    QString account_type, account_name, balance,
            account_owner;

    QMap<QString, QString> accountDetails;

    bool bitcoinAccount;
    QString bitcoin_balance, bitcoin_account_name;

    QString loginTXT;
    int state;

    void connectLoginBtns();
    void disconnectLoginBtns();
    void startLogin();
    void reset();
    void setUpMenu();
    void getCardInfo();
    void getCustomerInfo();
    void getAccessInfo();
    void getAccountInfo();
    void handleDualCard();
    void selectAccount();
    void getAccountType(QString accountID);
    void checkForBitcoin();
    void clearData();
    void disconnectNetworks();

private slots:
    void thisClose();
    void setUpLoginLabels();
    void number_btn_clicked();
    void btnEnterClicked();
    void btnCancelClicked();
    void btnClearClicked();
    void loginSlot();
    void getCardInfoSlot();
    void getCustomerInfoSlot();
    void getAccessInfoSlot();
    void getAccountInfoSlot();
    void getAccountTypeSlot();
    void onDebitSelected();
    void onCreditSelected();

};
#endif // MAINWINDOW_HPP
