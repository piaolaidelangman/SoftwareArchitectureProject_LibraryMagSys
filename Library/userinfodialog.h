#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include<QDebug>
#include<bookbut.h>

namespace Ui {
class userInfoDialog;
}

class userInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userInfoDialog(int id=1,QWidget *parent = 0);
    ~userInfoDialog();
    QSqlDatabase db;
    int userid;

private slots:

    void addbookinfo();


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_cancleBut_clicked();

    void on_saveBut_clicked();

private:
    Ui::userInfoDialog *ui;
};

#endif // USERINFODIALOG_H
