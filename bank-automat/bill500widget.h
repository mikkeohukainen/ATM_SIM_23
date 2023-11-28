#ifndef BILL500WIDGET_H
#define BILL500WIDGET_H

#include <QWidget>

namespace Ui {
class Bill500Widget;
}

class Bill500Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Bill500Widget(QWidget *parent = nullptr);
    ~Bill500Widget();

private:
    Ui::Bill500Widget *ui;

private slots:
    void btnClicked();
};

#endif // BILL500WIDGET_H
