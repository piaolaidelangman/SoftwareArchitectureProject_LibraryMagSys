#include "login.h"
#include "ui_login.h"
#include<mainwindow.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include<QMessageBox>
#include<management.h>
#include<usermainwindow.h>
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
     connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->login2, SIGNAL(clicked()), Qt::UniqueConnection);
}

login::~login()
{
    delete ui;
}

void login::on_login2_clicked()//登录
{
    bool flag=0;
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
            "credit int, users BOOL, fine double"
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
    int myid;
    while(query.next())
    {//判断登录的用户名或者密码是否正确
        if(query.value(2).toString().compare(ui->lineEdit->text())==0 &&query.value(3).toString().compare(ui->lineEdit_2->text())==0)
        {
            flag=1;
            myid=query.value(0).toInt();
            break;

        }
    }
    bool judge=query.value(7).toBool();
    if(flag && !judge)
    {
        this->close();
        management *w=new management;
        w->show();
    }
    else if(flag)
    {
        this->close();
        qDebug()<<myid<<endl;
        UserMainWindow *w=new UserMainWindow(myid);
        w->show();

    }
    else
    {
        QMessageBox::about(this, "登录失败", " 用户名或者密码错误");
    }
}

void login::on_back_clicked()//返回主界面
{
    this->close();
    MainWindow *temp=new MainWindow;
    temp->show();

}
