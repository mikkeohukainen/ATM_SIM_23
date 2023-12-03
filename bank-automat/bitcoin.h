#ifndef BITCOIN_H
#define BITCOIN_H

#include <QMainWindow>
#include <QNetworkAccessManager>

namespace Ui {
class Bitcoin;
}

class Bitcoin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Bitcoin(const QString &debit_account_id,
                     const QString &debit_account_name,
                     const QByteArray &token,
                     QWidget *parent = nullptr);
    ~Bitcoin();

signals:
    void bitcoinAccountReceived();
    void bitcoinPriceReceived();
    void eurosToBitcoinsReceived();
    void bitcoinsBought();
    void stateChanged();

private:
    void setLabels();
    void connectMainBtns();
    void connectBuyBtns();
    void disconnectBtns();
    void getBitcoinAccount();
    void getBitcoinPrice();
    void getEurosToBitcoins();
    void buyBitcoins();

    enum class State { MAIN, BUY, CONFIRM, NO_FUNDS, DATABASE_CONN_ERR, NETWORK_CONN_ERR };

    Ui::Bitcoin *ui;
    QNetworkAccessManager *manager;
    QByteArray token;
    State state;

    QString debit_account_id;
    QString debit_account_name;
    QString bitcoin_price;
    QString bitcoin_account_name;
    QString bitcoin_balance;
    QString bitcoin_balance_eur;
    QString amount_txt;
    QString btc_amount_txt;

private slots:
    void btn_left1_clicked();
    void btn_right1_clicked();
    void btn_right3_clicked();

    void number_btn_clicked();

    void enter_btn_clicked();
    void clear_btn_clicked();
    void cancel_btn_clicked();

    void getBitcoinAccountSlot();
    void getBitcoinPriceSlot();
    void getEurosToBitcoinsSlot();
    void buyBitcoinsSlot();
};

#endif // BITCOIN_H
