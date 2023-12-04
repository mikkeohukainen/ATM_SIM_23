#include "transactions.h"
#include "ui_transactions.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

transactions::transactions(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::transactions)
{
    ui->setupUi(this);
    ui->label_top->setAttribute(Qt::WA_SetFont, true);
    ui->mycentralwidget->setStyleSheet("QWidget#mycentralwidget{image: url(:/new/prefix1/img/GUI_noBTNS.png);}");
    setLabel();
    connectBtns();
    startIndex = 0;
    qDebug()<<idaccount;
    qDebug()<<startIndex;

}

transactions::~transactions()
{
    delete ui;
}

void transactions::setVariables(const QString &newIdaccount, const QByteArray &newToken)
{
    idaccount = newIdaccount;
    token = newToken;
}

void transactions::setLabel()
{


    ui->label_top->setText("Tilitapahtumat");


    ui->label_left1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label_left3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->txt_right1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->txt_right2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_right3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->label_left1->setText("Seuraava");
    ui->label_left2->setText("Edellinen");
    ui->label_left3->setText("");

    ui->txt_right1->setText("");
    ui->txt_right2->setText("");
    ui->label_right3->setText("Takaisin");
}

void transactions::connectBtns()
{
    connect(ui->btn_left1, &QPushButton::clicked, this, &transactions::btn_left1_clicked);
    connect(ui->btn_left2, &QPushButton::clicked, this, &transactions::btn_left2_clicked);
    connect(ui->btn_left3, &QPushButton::clicked, this, &transactions::btn_left3_clicked);
    //connect(ui->btn_right1, &QPushButton::clicked, this, &transactions::btn_right1_clicked);
    connect(ui->btn_right2, &QPushButton::clicked, this, &transactions::btn_right2_clicked);
    connect(ui->btn_right3, &QPushButton::clicked, this, &transactions::btn_right3_clicked);
}

void transactions::showTransactions()
{
     qDebug()<<idaccount;
     qDebug()<<startIndex;

     QJsonObject jsonObjTransactions;
     jsonObjTransactions.insert("idaccount",idaccount);
     jsonObjTransactions.insert("startIndex",startIndex);
     QString url = QString("http://localhost:3000/transactions/%1/%2").arg(idaccount).arg(startIndex);



     QNetworkRequest request((url));
     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

     request.setRawHeader(QByteArray("Authorization"), token);
     getManager = new QNetworkAccessManager(this);

     connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ShowTransactionsSlot(QNetworkReply*)));

     reply_transaction = getManager->get(request);
}

void transactions::ShowTransactionsSlot(QNetworkReply *reply_transaction)
{
    QFont font;
        ui->label_top->setStyleSheet("");
        font.setPointSize(12);
        ui->label_top->setStyleSheet("color: green;");
        font.setFamily("Arial");


        ui->label_top->setFont(font);



    qDebug()<<idaccount;
    response_data_transactions=reply_transaction->readAll();
    qDebug() << response_data_transactions;



            QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data_transactions);

          if(jsonDoc.isNull() || !jsonDoc.isArray()) {
                ui->label_top->setText("Error: Unable to parse transaction data. Go back.");

                qDebug()<<"jep";
                return;
            }



            QJsonArray transactionsArray = jsonDoc.array();


            QString displayText;
            for (const QJsonValue &transactionValue : transactionsArray) {
                QJsonObject transactionObject = transactionValue.toObject();
                //displayText += QString::number(transactionObject["idtransaction"].toInt()) + " | ";
                displayText += transactionObject["transaction_type"].toString() + ": ";
                displayText += transactionObject["amount"].toString() + " | ";
                QDateTime dateTime = QDateTime::fromString(transactionObject["transaction_date"].toString(), Qt::ISODate);
                            QString formattedDate = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                            displayText += formattedDate + "\n";




                            if (transactionObject.contains("transaction_description")) {

                                QJsonValue descriptionValue = transactionObject["transaction_description"];
                                if (!descriptionValue.isNull()) {
                                    //displayText += "\n";
                                    displayText += descriptionValue.toString() + "\n";
                                }
                            }




                            //displayText += "\n";

                            //displayText += "-----------------------\n";
            }


            //displayText += "\n";

             ui->label_top->setText(displayText.isEmpty() ? "No transactions found. Go back." : displayText);
             if (!displayText.isEmpty()) {


                        qDebug() << "Connected btn_left1";

                 } else {
                     // Disconnect btn_left1's click signal if no transactions are found
                     disconnect(ui->btn_left2, &QPushButton::clicked, this, &transactions::btn_left2_clicked);
                     startIndex =  startIndex - 5;
                     ui->label_left3->setText("Palaa");
                     ui->label_left1->setText("");
                     ui->label_left2->setText("");

                      qDebug() << "Disconnected btn_left1";
                 }

}

void transactions::btn_left1_clicked()
{   qDebug() << "btn_left1_clicked called";
    qDebug() << "Current startIndex:" << startIndex;
    startIndex = startIndex + 5;
    //ui->txt_right1->setText(QString::number(startIndex));
    showTransactions();
    qDebug()<<"btn1";

}

void transactions::btn_left2_clicked()
{
    if(startIndex!=0)
    {
     startIndex = startIndex - 5;
        qDebug()<<"-5";
        showTransactions();
        //connect(ui->btn_left1, &QPushButton::clicked, this, &transactions::btn_left1_clicked);
    }

    //ui->txt_right1->setText(QString::number(startIndex));
    qDebug() << "Current startIndex:" << startIndex;
    //connect(ui->btn_left1, &QPushButton::clicked, this, &transactions::btn_left1_clicked);

}

void transactions::btn_left3_clicked()
{
    startIndex = 0;
    showTransactions();
    ui->label_left1->setText("Seuraava");
    ui->label_left2->setText("Edellinen");
    ui->label_left3->setText("");
    connect(ui->btn_left2, &QPushButton::clicked, this, &transactions::btn_left2_clicked);

}

void transactions::btn_right1_clicked()
{

}

void transactions::btn_right2_clicked()
{

}

void transactions::btn_right3_clicked()
{
    this->close();
}
