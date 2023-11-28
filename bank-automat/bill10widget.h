#ifndef BILL10WIDGET_H
#define BILL10WIDGET_H

#include <QWidget>

namespace Ui {
class Bill10Widget;
}

class Bill10Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Bill10Widget(QWidget *parent = nullptr);
    ~Bill10Widget();

private:
    Ui::Bill10Widget *ui;

private slots:
    void btnClicked();
};

#endif // BILL10WIDGET_H
