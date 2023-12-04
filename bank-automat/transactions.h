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

namespace Ui {
class transactions;
}

class transactions : public QMainWindow
{
    Q_OBJECT

public:
    explicit transactions(QWidget *parent = nullptr);
    ~transactions();

    void setVariables(const QString &newIdaccount,const QByteArray &newToken);
    void showTransactions();
private:
    Ui::transactions *ui;

    QNetworkAccessManager *getManager;
    QNetworkReply *reply_transaction;
    QByteArray response_data_transactions;

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
    void ShowTransactionsSlot(QNetworkReply *reply_transaction);
};

#endif // TRANSACTIONS_H
