#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class UserMainWindow;
}

class UserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserMainWindow(int id=1,QWidget *parent = 0);
    ~UserMainWindow();

    void addbookinfo(const QString i,const QString b,const double p);


    QSqlDatabase db;
    int userid;
    double fine;


private slots:
    void on_searchButton_clicked();

    void on_pushButton_clicked();

    void on_pay_clicked();

    void on_rec_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UserMainWindow *ui;
};

#endif // USERMAINWINDOW_H
