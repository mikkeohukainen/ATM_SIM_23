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

    QNetworkAccessManager *postManager;
    QNetworkReply *reply_login;
    QByteArray response_data_login;
    QString idcard, PIN;

    QByteArray token;

    QNetworkAccessManager *getCardManager;
    QNetworkReply *reply_card;
    QByteArray response_data_card;
    QString card_type, idcustomer;

    QNetworkAccessManager *getCustomerManager;
    QNetworkReply *reply_customer;
    QByteArray response_data_customer;
    QString fname, lname;

    QNetworkAccessManager *getAccessManager;
    QNetworkReply *reply_access;
    QByteArray response_data_access;
    QStringList accountIDs;
    QString idaccount;

    QNetworkAccessManager *getAccountManager;
    QNetworkReply *reply_account;
    QByteArray response_data_account;
    QString account_type, account_name, balance,
            account_owner;
    QMap<QString, QString> accountDetails;
    bool bitcoinAccount;

    QNetworkAccessManager *getTypeManager;
    QNetworkReply *reply_type;
    QByteArray response_data_type;

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
    void loginSlot(QNetworkReply *reply);
    void getCardInfoSlot(QNetworkReply *reply);
    void getCustomerInfoSlot(QNetworkReply *reply);
    void getAccessInfoSlot(QNetworkReply *reply);
    void getAccountInfoSlot(QNetworkReply *reply);
    void getAccountTypeSlot(QNetworkReply *reply);
    void onDebitSelected();
    void onCreditSelected();

};
#endif // MAINWINDOW_HPP
