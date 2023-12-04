#ifndef BILLWIDGET_H
#define BILLWIDGET_H

#include <QPoint>
#include <QWidget>

namespace Ui {
class BillWidget;
}

class BillWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BillWidget(QWidget *parent, const QString &path, const QPoint &position);
    ~BillWidget();

private:
    Ui::BillWidget *ui;

private slots:
    void clicked();
};

#endif // BILLWIDGET_H
