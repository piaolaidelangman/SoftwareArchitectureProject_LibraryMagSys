#include "usermanagement.h"
#include "ui_usermanagement.h"
#include<QMessageBox>
#include<QDebug>
#include<QIntValidator>
userManagement::userManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userManagement)
{

    ui->setupUi(this);
    signOfPageChange=true;
    ui->lineEdit->hide();
    QIntValidator validator(0,9999,this);
    ui->lineEdit->setValidator(&validator);
    ui->labelUserId->setVisible(false);
    for (int i=0;i<pageSize;i++) {
        lab[i].setParent(ui->widget);
        lab[i].setVisible(false);
    }
    for (int i=0;i<buttNum;i++)
    {
        connect(&butt[i],&QPushButton::clicked, this,[=](){
            ui->labelUserName_2->setText(butt[i].property("username").toString());
            ui->labelCredit_2->setText(butt[i].property("credit").toString());
            ui->labelAddress_2->setText(butt[i].property("address").toString());
            ui->labelSex_2->setText(butt[i].property("sex").toString());
            ui->labelUsers_2->setText(butt[i].property("users").toString());
            ui->labelFine_2->setText(butt[i].property("fine").toString());
            ui->labelUserId->setText(butt[i].property("id").toString());
            currentButton=i;
        });
        butt[i].resize(buttSizeWidth,buttSizeHeight);
        butt[i].setParent(ui->widget);
        butt[i].setVisible(false);
    }
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

    query.exec("select * from users");
    int j=0,i;
    while(query.next())
    {
           butt[j].setText(query.value(0).toString()+"  "+query.value(1).toString());
           butt[j].setProperty("id",query.value(0).toString());
           butt[j].setProperty("username",query.value(2).toString());
           butt[j].setProperty("address",query.value(4).toString());
           butt[j].setProperty("sex",query.value(5).toString());
           butt[j].setProperty("credit",query.value(6).toString());
           butt[j].setProperty("users",query.value(7).toString());
           butt[j].setProperty("fine",QString::number(query.value(8).toDouble()));
           j++;
    }
    MaxItem=j;
    if(MaxItem!=0)
    {totalPage=MaxItem/pageSize;
    if(MaxItem%pageSize!=0)
        totalPage++;
    ui->labelTotalPage->setText("/"+QString::number(totalPage));
    if(totalPage!=0)
        ui->labelCurrentPage->setText("1");
    else
    {
        ui->labelCurrentPage->setText("0");
        ui->pushButtonNextPage->setEnabled(false);
        ui->pushButtonLastPage->setEnabled(false);
    }
    ui->comboBox->setMaxCount(totalPage);
    for (i=0;i<totalPage;i++) {
        ui->comboBox->addItem(QString::number(i+1));
    }
    int k=0;
    for (i=0;i<j&&i<pageSize;i++,k++) {
        butt[i].move(20,k*buffSize);
        lab[i].move(0,k*buffSize+5);
        lab[i].setText(QString::number(i+1)+".");
        lab[i].setVisible(true);
        butt[i].setVisible(true);
    }
    currentPage=1;
    if(currentPage==totalPage)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    if(currentPage>1)
        ui->pushButtonLastPage->setEnabled(true);
    else
        ui->pushButtonLastPage->setEnabled(false);
    currentMaxItem=i;
    ui->pushButtonLastPage->setEnabled(false);
    if(currentMaxItem<MaxItem)
        ui->pushButtonNextPage->setEnabled(true);
    }else
    {
         ui->pushButtonNextPage->setEnabled(false);
         ui->pushButtonLastPage->setEnabled(false);
         ui->labelCurrentPage->setText("0");
         ui->labelTotalPage->setText("/0");
         ui->comboBox->clear();
    }

}

userManagement::~userManagement()
{
    delete ui;
}
void userManagement::on_pushButtonNextPage_clicked()//响应下一页操作
{
    int i,j=0;
    for(i=currentPage*pageSize-pageSize;i<currentPage*pageSize;i++)
        butt[i].setVisible(false);
    currentPage++;
    for (;i<MaxItem&&i<currentPage*pageSize;i++,j++) {
        butt[i].setParent(ui->widget);
        butt[i].move(20,j*buffSize);
        lab[j].move(0,j*buffSize+5);
        lab[j].setText(QString::number(i+1)+".");
        lab[j].setVisible(true);
        butt[i].setVisible(true);
    }
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);

    }
    currentMaxItem=i;
    ui->pushButtonLastPage->setEnabled(true);
    if(i>=MaxItem)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    ui->labelCurrentPage->setText(QString::number(currentPage));
    ui->comboBox->setCurrentIndex(currentPage-1);
    if(currentPage==totalPage)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    if(currentPage>1)
        ui->pushButtonLastPage->setEnabled(true);
    else
        ui->pushButtonLastPage->setEnabled(false);
}




void userManagement::on_pushButtonLastPage_clicked()//响应上一页操作
{
    int i,j=0;
    for(i=currentPage*pageSize-pageSize;i<currentMaxItem;i++)
        butt[i].setVisible(false);
    currentPage--;
    i=currentPage*pageSize-pageSize;
    for (;i<currentPage*pageSize;i++,j++) {
        butt[i].setParent(ui->widget);
        butt[i].move(20,j*buffSize);
        lab[j].move(0,j*buffSize+5);
        lab[j].setText(QString::number(i+1)+".");
        lab[j].setVisible(true);
        butt[i].setVisible(true);
    }
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);
    }
    currentMaxItem=i;
    ui->pushButtonNextPage->setEnabled(true);
    if(currentMaxItem-pageSize>0)
        ui->pushButtonLastPage->setEnabled(true);
    else
        ui->pushButtonLastPage->setEnabled(false);
    ui->labelCurrentPage->setText(QString::number(currentPage));
    ui->comboBox->setCurrentIndex(currentPage-1);
    if(currentPage==totalPage)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    if(currentPage>1)
        ui->pushButtonLastPage->setEnabled(true);
    else
        ui->pushButtonLastPage->setEnabled(false);
}

void userManagement::on_comboBox_currentIndexChanged(int index)//通过选择页面进行跳转
{
    int tar=ui->comboBox->currentText().toInt();
    if(signOfPageChange==false)
    {int i,j=0;
    for(i=currentPage*pageSize-pageSize;i<currentPage*pageSize;i++)
        butt[i].setVisible(false);
    currentPage=tar;
    i=currentPage*pageSize-pageSize;
    for (;i<MaxItem&&i<currentPage*pageSize;i++,j++) {
        butt[i].setParent(ui->widget);
        butt[i].move(20,j*buffSize);
        lab[j].move(0,j*buffSize+5);
        lab[j].setText(QString::number(i+1)+".");
        lab[j].setVisible(true);
        butt[i].setVisible(true);
    }
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);
    }
    currentMaxItem=i;
    ui->pushButtonLastPage->setEnabled(true);
    if(i>=MaxItem)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    ui->labelCurrentPage->setText(QString::number(currentPage));
    }
    else
        signOfPageChange=false;
}


void userManagement::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="修改")
    {
        ui->lineEdit->setText(ui->labelFine_2->text());
        ui->lineEdit->show();
        ui->pushButton->setText("确认");
    }else
    {
        query.prepare("update user set fine=? where id=?");
        query.addBindValue(ui->lineEdit->text().toDouble());
        query.addBindValue(ui->labelUserId->text());
        //qDebug()<<ui->labelUserId->text();
        query.exec();
        ui->pushButton->setText("修改");
        ui->lineEdit->hide();
        ui->labelFine_2->setText(ui->lineEdit->text());
    }
}

