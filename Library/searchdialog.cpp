#include "searchdialog.h"
#include "ui_searchdialog.h"
#include<QDebug>

searchDialog::searchDialog(int id,QString book,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);
    this->bookname=book;
    userid=id;

    ui->bookList->setAutoScroll(true);
    ui->label->setText(ui->label->text().append("《"+bookname+"》"));

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

    n=0;

    QString sql="select * from books where name like '%"+bookname+"%';"; //查询数据库有多少个book对象满足搜索条件
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    while(query.next()){
        n++;
        addbookinfo(query.value(0).toString(),query.value(1).toString(),query.value(3).toDouble());
    }
    ui->label_2->setText("一共"+QString::number(n)+"条搜索结果");
    ui->label->setText("您搜索的关键字是："+bookname);
    if(n==0){
        QWidget *c=new QWidget(ui->bookList);
        QHBoxLayout *hl=new QHBoxLayout(c);

        QLabel *la=new QLabel("抱歉，没有找到相关图书。");
        la->setAlignment(Qt::AlignHCenter);
        hl->addWidget(la);
        la->setFixedWidth(200);
        la->setFixedHeight(40);

        hl->setContentsMargins(1,0,0,1);
        c->setLayout(hl);
        c->resize(350,100);
        c->show();

        QListWidgetItem *item=new QListWidgetItem(ui->bookList);
        item->setSizeHint(QSize(600,40));  //设置行的大小
        ui->bookList->setItemWidget(item,c);
    }

}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::addbookinfo(const QString i,const QString b,const double p)
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

void searchDialog::on_returnBut_clicked()
{
    this->hide();
}
