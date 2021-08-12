#include "addbooks.h"
#include "ui_addbooks.h"
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QMessageBox>
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
addBooks::addBooks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addBooks)
{

    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Library");//加载数据库
     qDebug() << db.connectionName();
    db.setDatabaseName("Library.db");
     qDebug() << db.databaseName();

     if (!db.open())//打开数据库
    {
        qDebug() << "open fail";
        qDebug() << db.lastError().text();

    }
      QSqlQuery query(db);//访问数据库
      QString sql="create table if not exists books(barcode QString primary key, name varchar(255),"
                  "writer varchar(255), price double,num int,press varchar(255),category varchar(255));";

      if (!query.exec(sql))//创建表
      {
          qDebug() << query.lastError().text();

      }

}

addBooks::~addBooks()
{
    delete ui;
}


void addBooks::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)//确定选择的分类，用QString的category保存
{
     category=ui->treeWidget->currentItem()->text(0);
}

void addBooks::on_pushButton_clicked()//响应点击确定按钮，对数据库进行操作，并且关闭当前窗口
{

    QString str = QString("insert into books(barcode, name,"
                          "writer, price,num,press,category) values('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
            .arg(ui->lineEditCode->text()).arg(ui->lineEditName->text()).arg(ui->lineEditWriter->text())
            .arg(ui->lineEditPrice->text().toDouble()).arg(ui->lineEditNum->text().toInt())
            .arg(ui->lineEditPress->text()).arg(category);
    ui->lineEditNum->clear();
    ui->lineEditCode->clear();
    ui->lineEditName->clear();
    ui->lineEditPress->clear();
    ui->lineEditPrice->clear();
    ui->lineEditWriter->clear();
    query.exec(str);
    this->close();

}
