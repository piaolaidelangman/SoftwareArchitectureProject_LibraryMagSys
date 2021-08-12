#include "userinfodialog.h"
#include "ui_userinfodialog.h"
#include<QMessageBox>
#include<bookbut.h>

userInfoDialog::userInfoDialog(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfoDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_4->setReadOnly(true);
    ui->lineEdit_5->setReadOnly(true);
    ui->lineEdit_6->setReadOnly(true);
    ui->lineEdit_7->setReadOnly(true);
    ui->lineEdit_8->setReadOnly(true);
    ui->lineEdit_9->setReadOnly(true);

    ui->myBookList->setResizeMode(QListView::Adjust);
    ui->myBookList->setAutoScroll(true);


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
    //打开成功执行sql

    QSqlQuery query(db);

    //加载用户个人信息
    QString sql = "select * from users where id='"
            +QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }

    while(query.next())
    {

        qDebug()<<"加载用户信息"<<query.value(2).toString();
        ui->lineEdit->setText(query.value(0).toString());
        ui->lineEdit_2->setText(query.value(2).toString());
        ui->lineEdit_3->setText(query.value(1).toString());
        ui->lineEdit_4->setText(query.value(3).toString());
        ui->lineEdit_5->setText(query.value(4).toString());
        ui->lineEdit_6->setText(query.value(5).toString());
        ui->lineEdit_7->setText(query.value(6).toString());
        ui->lineEdit_8->setText(query.value(7).toString());
        qDebug()<<"加载用户信息完成";

    }


    //加载用户借阅信息
    addbookinfo();

}

userInfoDialog::~userInfoDialog()
{
    delete ui;
}

void userInfoDialog::addbookinfo()
{
    QSqlQuery query(db);
    QString sql="select * from waterflow where userid="+QString::number(userid)+";";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    qDebug()<<"加载借阅信息";
    while(query.next())
    {

        if(query.value(0).toInt()==1||(query.value(0).toInt()==2&&query.value(8).toInt()==2))  //加载借阅图书。
        {

            QWidget *c=new QWidget(ui->myBookList);
            QHBoxLayout *hl=new QHBoxLayout(c);

            QLabel *ID=new QLabel(query.value(1).toString());
            bookBut *name=new bookBut(2,userid,query.value(9).toString());
            QLabel *type=new QLabel("已借阅");
            QLabel *bDate=new QLabel(query.value(3).toString());
            QLabel *dDate=new QLabel(query.value(5).toString());

            if(query.value(8).toInt()==0){
                type->setText("借阅申请审核中");
                bDate->setText("--");
                dDate->setText("--");
            }
            name->setFixedWidth(120);
            name->setFixedHeight(40);
            ID->setFixedWidth(120);
            ID->setFixedHeight(40);
            type->setFixedWidth(120);
            type->setFixedHeight(40);
            bDate->setFixedWidth(120);
            bDate->setFixedHeight(40);
            dDate->setFixedWidth(120);
            dDate->setFixedHeight(40);

            ID->setAlignment(Qt::AlignHCenter);
            type->setAlignment(Qt::AlignHCenter);
            bDate->setAlignment(Qt::AlignHCenter);
            dDate->setAlignment(Qt::AlignHCenter);

            hl->addWidget(ID);
            hl->addStretch(1);
            hl->addWidget(name);
            hl->addStretch(1);
            hl->addWidget(type);
            hl->addStretch(1);
            hl->addWidget(bDate);
            hl->addStretch(1);
            hl->addWidget(dDate);

            hl->setContentsMargins(3,0,0,3);
            c->setLayout(hl);
            c->resize(350,100);
            c->show();

            QListWidgetItem *item=new QListWidgetItem(ui->myBookList);
            item->setSizeHint(QSize(600,40));  //设置行的大小
            ui->myBookList->setItemWidget(item,c);
        }
        else if(query.value(0).toInt()==2){

            QWidget *c=new QWidget(ui->myBookList);
            QHBoxLayout *hl=new QHBoxLayout(c);

            QLabel *ID=new QLabel(query.value(1).toString());
            bookBut *name=new bookBut(2,userid,query.value(9).toString());
            QLabel *type=new QLabel("预约中");
            QLabel *bDate=new QLabel("--");
            QLabel *dDate=new QLabel("--");
            if(query.value(8)==1){
                type->setText("成功预约");
            }
            ID->setAlignment(Qt::AlignHCenter);
            type->setAlignment(Qt::AlignHCenter);
            bDate->setAlignment(Qt::AlignHCenter);
            dDate->setAlignment(Qt::AlignHCenter);

            name->setFixedWidth(120);
            name->setFixedHeight(40);
            ID->setFixedWidth(120);
            ID->setFixedHeight(40);
            type->setFixedWidth(120);
            type->setFixedHeight(40);
            bDate->setFixedWidth(120);
            bDate->setFixedHeight(40);
            dDate->setFixedWidth(120);
            dDate->setFixedHeight(40);

            hl->addWidget(ID);
            hl->addStretch(1);
            hl->addWidget(name);
            hl->addStretch(1);
            hl->addWidget(type);
            hl->addStretch(1);
            hl->addWidget(bDate);
            hl->addStretch(1);
            hl->addWidget(dDate);

            hl->setContentsMargins(3,0,0,3);
            c->setLayout(hl);
            c->resize(350,100);
            c->show();

            QListWidgetItem *item=new QListWidgetItem(ui->myBookList);
            item->setSizeHint(QSize(600,40));  //设置行的大小
            ui->myBookList->setItemWidget(item,c);
        }

    }

    qDebug()<<"加载完成";

}



void userInfoDialog::on_pushButton_3_clicked()
{
    if(ui->lineEdit_3->isReadOnly()){
        ui->lineEdit_3->setReadOnly(false);
        ui->pushButton_3->setText("保存");
    }
    else
    {
        ui->lineEdit_3->setReadOnly(true);
        ui->pushButton_3->setText("编辑");
    }
}

void userInfoDialog::on_pushButton_4_clicked()
{
    if(ui->lineEdit_4->isReadOnly()){
        ui->lineEdit_4->setReadOnly(false);
        ui->pushButton_4->setText("保存");
    }
    else
    {
        ui->lineEdit_4->setReadOnly(true);
        ui->pushButton_4->setText("编辑");
    }
}

void userInfoDialog::on_pushButton_5_clicked()
{
    if(ui->lineEdit_5->isReadOnly()){
        ui->lineEdit_5->setReadOnly(false);
        ui->pushButton_5->setText("保存");
    }
    else
    {
        ui->lineEdit_5->setReadOnly(true);
        ui->pushButton_5->setText("编辑");
    }
}

void userInfoDialog::on_pushButton_6_clicked()
{
    if(ui->lineEdit_6->isReadOnly()){
        ui->lineEdit_6->setReadOnly(false);
        ui->pushButton_6->setText("保存");
    }
    else
    {
        ui->lineEdit_6->setReadOnly(true);
        ui->pushButton_6->setText("编辑");
    }
}

void userInfoDialog::on_cancleBut_clicked()
{
    this->hide();
}

void userInfoDialog::on_saveBut_clicked()
{
    QSqlQuery query(db);

    QString sql = "update users set name='"+ui->lineEdit_3->text()+"' where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    sql="update users set password='"+ui->lineEdit_4->text()+"' where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    sql="update users set address='"+ui->lineEdit_5->text()+"' where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    sql="update users set sex='"+ui->lineEdit_6->text()+"' where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    QMessageBox::information(this,"保存成功","成功修改个人信息",QMessageBox::Yes ,QMessageBox::Yes);
}
