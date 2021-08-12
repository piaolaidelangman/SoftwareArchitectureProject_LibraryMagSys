#include "usermainwindow.h"
#include "ui_usermainwindow.h"
#include"searchdialog.h"
#include<QDebug>
#include<mainwindow.h>

#include<userinfodialog.h>
#include<QMessageBox>
#include<recbookdialog.h>

UserMainWindow::UserMainWindow(int id,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserMainWindow)
{
    ui->setupUi(this);

    userid=id;

    ui->frame->setStyleSheet("#frame{border:2px solid #014F84}#background-color:#00d8ff");
    ui->searchLine->setPlaceholderText("请输入要搜索的书名(如：边城)");

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

    QString sql = "select * from users where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    while(query.next())
    {

        qDebug()<<"加载用户信息"<<query.value(2).toString();
        ui->username->setText(query.value(2).toString());
        ui->idLabel->setText(query.value(0).toString());
        qDebug()<<"加载用户信息完成";
    }

    sql="create table if not exists waterflow"
        "(operation int,bookid int,userid int,"
        "bDate date,rDate date,dDate date, day int,"
        "ifhandle bool, result int,recbook varchar(30)"
        ");";
    //bDate 借书日期 rDate 还书日期 dDate 截至日期 bookid 图书id  userid 用户id   ifhandle 是否处理  result 处理结果
    //operation(1：借阅(填写bDate dDate ) 2：预约(用户填写预约借阅天数day  管理员同意时填写bDate和dDate)
    //          3：续借(day为用户想要多借的天数 管理员同意时dDate+=day) 4：还书(填写还书日期rDate) 5：荐书申请（只提供书名）)
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }

    sql = "create table if not exists users("
          "id integer primary key,  name varchar(20),"
          "username varchar(30),password varchar(30),address varchar(50),sex varchar(10),"
          "credit int, users BOOL, fine double"
          ");";
    //创建用户表
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }




    sql="select * from books;"; //查询数据库的所有图书
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    while(query.next()){
        addbookinfo(query.value(0).toString(),query.value(1).toString(),query.value(3).toDouble());
    }
}

UserMainWindow::~UserMainWindow()
{
    delete ui;
}

void UserMainWindow::on_searchButton_clicked()
{
    QString bookname=ui->searchLine->text();
    searchDialog *s=new searchDialog(userid,bookname,this);
    s->setWindowTitle("搜索图书");
    s->show();

}

void UserMainWindow::on_pushButton_clicked()
{
    userInfoDialog *dlg=new userInfoDialog(userid,this);
    dlg->setWindowTitle("用户信息");
    dlg->show();
}

void UserMainWindow::on_pay_clicked()  //支付按钮
{

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
    QString sql="select fine from users where id='"+QString::number(userid)+"';";
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    if(query.next()){
        if(query.value(0).toDouble()>0){
            QMessageBox::StandardButton reply;
            reply=QMessageBox::question(this,"请支付罚款！","您剩余"+query.value(0).toString()+"元罚款待支付，请支付。",QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                QMessageBox::information(this,"支付成功","您已付清所有罚款。",QMessageBox::Yes);
                sql="update users set fine=0 where id='"+QString::number(userid)+"';";
                if (!query.exec(sql))
                {
                    qDebug() << query.lastError().text();

                }
            }
        }
        else
        {
            QMessageBox::information(this,"","您的罚款已全部支付。",QMessageBox::Yes);
        }
    }
}

void UserMainWindow::on_rec_clicked()  //荐书按钮
{
    recBookDialog *dlg=new recBookDialog(userid,this);
    dlg->setWindowTitle("用户荐书系统");
    dlg->show();
}

void UserMainWindow::addbookinfo(const QString i,const QString b,const double p)
{

    QWidget *c=new QWidget(ui->bookList);
    QHBoxLayout *hl=new QHBoxLayout(c);


    bookBut *name=new bookBut(1,userid,b);
    QLabel *ID=new QLabel(i);
    QLabel *price=new QLabel(QString::number(p));

    name->setFixedWidth(200);
    name->setFixedHeight(40);
    ID->setFixedWidth(200);
    ID->setFixedHeight(40);
    ID->setAlignment(Qt::AlignHCenter);
    price->setFixedWidth(200);
    price->setFixedHeight(40);
    price->setAlignment(Qt::AlignHCenter);

    hl->addWidget(name);
    hl->addStretch(1);
    hl->addWidget(ID);
    hl->addStretch(1);
    hl->addWidget(price);

    hl->setContentsMargins(3,0,0,3);
    c->setLayout(hl);
    c->resize(350,100);
    c->show();

    QListWidgetItem *item=new QListWidgetItem(ui->bookList);
    item->setSizeHint(QSize(600,40));  //设置行的大小
    ui->bookList->setItemWidget(item,c);


}

void UserMainWindow::on_pushButton_2_clicked()
{
    this->close();
    MainWindow *w=new MainWindow;
    w->show();

}
