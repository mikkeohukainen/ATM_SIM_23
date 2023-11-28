#ifndef BILL50WIDGET_H
#define BILL50WIDGET_H

#include <QWidget>

namespace Ui {
class Bill50Widget;
}

class Bill50Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Bill50Widget(QWidget *parent = nullptr);
    ~Bill50Widget();

private:
    Ui::Bill50Widget *ui;

private slots:
    void btnClicked();
};

#endif // BILL50WIDGET_H
