#ifndef BILL100WIDGET_H
#define BILL100WIDGET_H

#include <QWidget>

namespace Ui {
class Bill100Widget;
}

class Bill100Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Bill100Widget(QWidget *parent = nullptr);
    ~Bill100Widget();

private:
    Ui::Bill100Widget *ui;

private slots:
    void btnClicked();
};

#endif // BILL100WIDGET_H
