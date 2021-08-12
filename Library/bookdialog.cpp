#include "bookdialog.h"
#include "ui_bookdialog.h"
#include<QMessageBox>
#include<QDebug>
#include<QDateTime>

BookDialog::BookDialog(int t,int id,QString bookname,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{

    ui->setupUi(this);
    name=bookname;
    type=t;
    userid=id;

    db = QSqlDatabase::addDatabase("QSQLITE", "Library");//加载数据库
    qDebug() << db.connectionName();
    db.setDatabaseName("Library.db");
    qDebug() << db.databaseName();
    if (!db.open())//打开数据库
    {
        qDebug() << "open fail";
        qDebug() << db.lastError().text();
    }

    QSqlQuery query(db);

    QString sql="select * from books where name='"+name+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    if(query.next()){
        ui->IDLabel->setText(query.value(0).toString());
        ui->nameLabel->setText(query.value(1).toString());
        ui->priceLabel->setText(query.value(3).toString());
        ui->pressLabel->setText(query.value(5).toString());
        ui->writerLabel->setText(query.value(2).toString());
        remain=query.value(4).toInt();
    }

    if(remain>0){
        ui->order->setDisabled(true);
    }
    else
    {
        ui->borrow->setDisabled(true);
    }

    if(type==1){
        ui->xujie->setDisabled(true);
        ui->returnBut->setDisabled(true);

    }
    else if(type==2)
    {
        ui->borrow->setDisabled(true);
        ui->order->setDisabled(true);
    }

    sql="select * from users where id="+QString::number(userid)+";";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    if(query.next()){
        fine=query.value(8).toDouble();
        credit=query.value(6).toInt();
    }

}

BookDialog::~BookDialog()
{
    delete ui;
}

void BookDialog::on_borrow_clicked()
{

    if(remain>0&&credit>0&&fine==0)//如果信誉和欠款满足条件则借阅成功，库存减一，用户借阅状态变化
    {
        QSqlQuery query(db);
        QString sql="insert into waterflow"
                    "(operation,bookid,userid,"
                    "bDate,dDate, day,ifhandle,result,recbook)"
                    "values(?,?,?,?,?,?,?,?,?)";
        query.prepare(sql);
        query.addBindValue(1);
        query.addBindValue(ui->IDLabel->text().toInt());
        query.addBindValue(userid);
        QDateTime borrowTime =QDateTime::currentDateTime();
        QDateTime DeadTime=borrowTime.addDays(30);
        query.addBindValue(borrowTime.toString("yyyy-MM-dd"));
        query.addBindValue(DeadTime.toString("yyyy-MM-dd"));
        query.addBindValue(30);
        query.addBindValue(false);
        query.addBindValue(1);
        query.addBindValue(ui->nameLabel->text());

        if(!query.exec())
        {
            qDebug()<<query.lastError();
            QMessageBox::information(this,"借阅失败","《"+this->name+"》货存不足。",QMessageBox::Yes ,QMessageBox::Yes);
        }
        else
        {
            remain--;
            if(!query.exec("update books set num="+QString::number(remain)+" where barcode='"+ui->IDLabel->text()+"';"))
            {
                qDebug()<<query.lastError();
            }

            QMessageBox::information(this,"借阅成功","成功借阅《"+this->name+"》。",QMessageBox::Yes ,QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::information(this,"借阅失败","您的信誉分不够或者尚有欠款未支付，或者此书货存不足。",QMessageBox::Yes ,QMessageBox::Yes);
    }
}

void BookDialog::on_order_clicked()
{
    if(credit>0&&fine==0)//如果信誉和欠款满足条件即可以预约
    {
        QSqlQuery query(db);
        QString sql="insert into waterflow"
                    "(operation,bookid,userid,"
                    "day,ifhandle,result,recbook)"
                    "values(?,?,?,?,?,?,?)";
        query.prepare(sql);
        query.addBindValue(2);
        query.addBindValue(ui->IDLabel->text().toInt());
        query.addBindValue(userid);
        query.addBindValue(30);
        query.addBindValue(false);
        query.addBindValue(0);
        query.addBindValue(ui->nameLabel->text());

        if(!query.exec())
        {
            qDebug()<<query.lastError();
            QMessageBox::information(this,"预约失败","出错，预约失败。",QMessageBox::Yes ,QMessageBox::Yes);
        }
        else
        {
            QMessageBox::information(this,"提交成功","成功提交预约《"+this->name+"》的申请。（默认预约30天）",QMessageBox::Yes ,QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::information(this,"预约失败","您的信誉分不够或者尚有欠款未支付。",QMessageBox::Yes ,QMessageBox::Yes);
    }
}

void BookDialog::on_cancle_clicked()
{
    this->hide();
}

void BookDialog::on_xujie_clicked()
{
    if(credit>0&&fine==0) //如果信誉和欠款满足条件即可以预约
    {

        QSqlQuery query(db);
        QString sql="insert into waterflow"
                    "(operation,bookid,userid,"
                    "day,ifhandle,recbook)"
                    "values(?,?,?,?,?,?)";
        query.prepare(sql);
        query.addBindValue(3);
        query.addBindValue(ui->IDLabel->text().toInt());
        query.addBindValue(userid);
        query.addBindValue(30);
        query.addBindValue(false);
        query.addBindValue(ui->nameLabel->text());

        if(!query.exec())
        {
            qDebug()<<query.lastError();
            QMessageBox::information(this,"续借失败","续借《"+this->name+"》失败。",QMessageBox::Yes ,QMessageBox::Yes);
        }
        else
        {
            QMessageBox::information(this,"提交成功","成功提交续借《"+this->name+"》的申请。",QMessageBox::Yes ,QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::information(this,"续借失败","您的信誉分不够或者尚有欠款未支付。",QMessageBox::Yes ,QMessageBox::Yes);
    }
}



void BookDialog::on_returnBut_clicked()
{
    QSqlQuery query(db);
    QString sql="insert into waterflow"
                "(operation,bookid,userid,"
                "rDate,ifhandle,recbook)"
                "values(?,?,?,?,?,?)";
    query.prepare(sql);
    query.addBindValue(4);
    query.addBindValue(ui->IDLabel->text().toInt());
    query.addBindValue(userid);
    QDateTime returntime=QDateTime::currentDateTime();
    query.addBindValue(returntime);
    query.addBindValue(false);
    query.addBindValue(ui->nameLabel->text());

    if(!query.exec())
    {
        qDebug()<<query.lastError();
        QMessageBox::information(this,"还书失败","还书《"+this->name+"》失败。",QMessageBox::Yes ,QMessageBox::Yes);
    }
    else
    {
        remain++;
        if(!query.exec("update books set num="+QString::number(remain)+" where barcode='"+ui->IDLabel->text()+"';"))
        {
            qDebug()<<query.lastError();
        }
        if(!query.exec("delete from waterflow where operation=1 and bookid="+ui->IDLabel->text()+" and userid="+QString::number(userid)+";"))
        {
            qDebug()<<query.lastError();
        }
        QMessageBox::information(this,"还书成功","成功归还《"+this->name+"》。",QMessageBox::Yes ,QMessageBox::Yes);
        ui->returnBut->setDisabled(true);
    }
}
