#ifndef BILL20WIDGET_H
#define BILL20WIDGET_H

#include <QWidget>

namespace Ui {
class Bill20Widget;
}

class Bill20Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Bill20Widget(QWidget *parent = nullptr);
    ~Bill20Widget();

private:
    Ui::Bill20Widget *ui;

private slots:
    void btnClicked();
};

#endif // BILL20WIDGET_H
