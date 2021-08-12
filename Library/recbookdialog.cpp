#include "recbookdialog.h"
#include "ui_recbookdialog.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QDebug>
#include<QMessageBox>


recBookDialog::recBookDialog(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recBookDialog)
{
    ui->setupUi(this);
    userid=id;

}

recBookDialog::~recBookDialog()
{
    delete ui;
}

void recBookDialog::on_okBut_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Library");//加载数据库
    qDebug() << db.connectionName();
    db.setDatabaseName("Library.db");
    qDebug() << db.databaseName();
    if (!db.open())//打开数据库
    {
        qDebug() << "open fail";
        qDebug() << db.lastError().text();
    }
    //打开成功执行sql

    QSqlQuery query(db);

    QString sql = "insert into waterflow"
                  "(operation,userid,"
                  "ifhandle,recbook)"
                  "values(?,?,?,?)";
    query.prepare(sql);
    query.addBindValue(5);
    query.addBindValue(userid);
    query.addBindValue(false);
    query.addBindValue(ui->recLine->text());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    else
    {
        QMessageBox::information(this,"荐书成功","成功提交荐书申请。",QMessageBox::Yes);
    }
}

void recBookDialog::on_cancle_clicked()
{
    this->hide();
}
