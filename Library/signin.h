#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>

namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = 0);
    ~signin();

private slots:
    void on_back_clicked();

    void on_signin2_clicked();

private:
    Ui::signin *ui;
};

#endif // SIGNIN_H
