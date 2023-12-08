#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QMainWindow>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QFont>

#include "ui_transactions.h"

namespace Ui {
class Transactions;
}

class Transactions : public QMainWindow
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setVariables(const QString &newIdaccount,const QByteArray &newToken);
    void getTransactions();

private:
    Ui::Transactions *ui;

    QNetworkAccessManager *manager;

    QString idaccount;

    QByteArray token;

    int startIndex;
    int previousStartIndex;

    QFont font;

    void setLabel();
    void connectBtns();
   // void showTransactions();


private slots:
    void btn_left1_clicked();
    void btn_left2_clicked();
    void btn_left3_clicked();
    void btn_right1_clicked();
    void btn_right2_clicked();
    void btn_right3_clicked();
    void getTransactionsSlot();
};

#endif // TRANSACTIONS_H
