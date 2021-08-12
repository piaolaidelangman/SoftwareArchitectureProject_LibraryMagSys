#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(int t=1,int id=1,QString bookname="",QWidget *parent = 0);
    ~BookDialog();

private slots:
    void on_borrow_clicked();

    void on_order_clicked();

    void on_cancle_clicked();

    void on_xujie_clicked();

    void on_returnBut_clicked();

private:
    Ui::BookDialog *ui;
    QSqlDatabase db;

    QString name;
    int remain;//剩余图书数量
    int userid;//用户id
    int type;//图书类型（1为搜索时展示的图书资料，2为用户借阅信息里的图书资料）
    double  fine;//欠款
    int credit ;//信誉值


};

#endif // BOOKDIALOG_H
