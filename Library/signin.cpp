#include "signin.h"
#include "ui_signin.h"
#include<mainwindow.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include<QMessageBox>
signin::signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    this->setWindowTitle("注册");
     connect(ui->lineEdit_4, SIGNAL(returnPressed()), ui->signin2, SIGNAL(clicked()), Qt::UniqueConnection);



}

signin::~signin()
{
    delete ui;
}

void signin::on_back_clicked()//返回主界面
{
    MainWindow *temp=new MainWindow;
    temp->show();
    this->close();

}

void signin::on_signin2_clicked()//执行注册按键
{


    bool flag=1;
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
    QString sql =
                "create table if not exists users("
                "id integer primary key,  name varchar(20),"
                "username varchar(30),password varchar(30),address varchar(50),sex varchar(10),"
                "credit int, users BOOL,fine double"
                ");";
        QSqlQuery query(db);//访问数据库
        if (!query.exec(sql))//创建表
        {
            qDebug() << query.lastError().text();

        }
        sql = "select * from users;";  //获取数据库所有用户
           if (!query.exec(sql))
           {
               qDebug() << query.lastError().text();

           }
        while(query.next()) //遍历看是否有相同的用户名
        {
            if(query.value(2).toString().compare(ui->lineEdit_2->text())==0)
            {
                flag=0;//标记是否有相同用户名
                QMessageBox::about(this, "失败", "该用户名已被使用");

            }
        }

        if(flag)
        {
            if(ui->lineEdit_3->text().compare(ui->lineEdit_4->text())==0)
            {

                query.prepare("INSERT INTO users (id, name,username,password,users,credit,fine) "
                              "VALUES (:id, :name, :username,:password,:users,:credit,:fine)");
                query.bindValue(":name",ui->lineEdit->text());
                query.bindValue(":username",ui->lineEdit_2->text());
                query.bindValue(":password",ui->lineEdit_3->text());
                query.bindValue(":users",true);
                query.bindValue(":credit",100);
                query.bindValue(":fine",0);
                query.exec();//入库
                QMessageBox::about(this, "成功", " 注册成功");
                ui->back->clicked();

            }
            else
            {
                 QMessageBox::about(this, "失败", "两次密码输入不一致");
            }

        }

}
