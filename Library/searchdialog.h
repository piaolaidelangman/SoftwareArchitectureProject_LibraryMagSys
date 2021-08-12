#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include<QLabel>
#include<QPushButton>
#include<bookbut.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(int id=1,QString book="时间简史",QWidget *parent = 0);
    ~searchDialog();
    void addbookinfo(const QString i="123",const QString n="时间简史",const double p=50);

    QSqlDatabase db;
    QString bookname;
    int n;
    int userid;

private slots:
    void on_returnBut_clicked();

private:
    Ui::searchDialog *ui;
};

#endif // SEARCHDIALOG_H
