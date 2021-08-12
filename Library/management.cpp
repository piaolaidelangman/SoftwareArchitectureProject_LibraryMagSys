#include "management.h"
#include "ui_management.h"
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QMessageBox>
#include<QString>
#include<QPushButton>
#include<QDebug>
#include<mainwindow.h>
management::management(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::management)
{


    ui->setupUi(this);
    this->setWindowTitle("尊敬的管理员您好");
    signOfPageChange=false;
    userMan.setParent(ui->stackedWidget->widget(1));
    ui->stackedWidget->addWidget(&resMan);
    ui->pushButtonNextPage->setEnabled(false);
    ui->pushButtonLastPage->setEnabled(false);
    QSqlDatabase menudb;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
          menudb = QSqlDatabase::database("qt_sql_default_connection");
        else
          menudb = QSqlDatabase::addDatabase("QSQLITE");
        menudb.setDatabaseName("Library.db");

        if (!menudb.open())
            QMessageBox::warning(this, "error", menudb.lastError().text());
        query=QSqlQuery(menudb);
        query.exec("create table books(barcode QString primary key, name varchar(255),"
                   "writer varchar(255), price double,int num,press varchar(255),category varchar(255));");
        for (int i=0;i<pageSize;i++) {
            //lab[i].setText(QString::number(i+1));
            lab[i].setParent(ui->widget);
            lab[i].setVisible(false);
            labDetial[i].setParent(ui->widget);
            labDetial[i].setVisible(false);
        }
        for (int i=0;i<buttNum;i++)
        {
            connect(&butt[i],&QPushButton::clicked, this,[=](){
                ui->labelNum->setText(butt[i].property("num").toString());
                ui->labelName->setText(butt[i].text());
                ui->labelWriter->setText(butt[i].property("writer").toString());
                ui->labelPress->setText(butt[i].property("press").toString());
                ui->labelPrice->setText(butt[i].property("price").toString());
                ui->labelCode->setText(butt[i].property("barcode").toString());
                ui->labelCategory->setText(butt[i].property("category").toString());

                ui->pushButtonChange->setEnabled(true);
                currentButton=i;
            });
            butt[i].resize(buttSizeWidth,buttSizeHeight);
            butt[i].setParent(ui->widget);
            butt[i].setVisible(false);
        }
        ui->lineEditNum->setVisible(false);
        ui->lineEditName->setVisible(false);
        ui->lineEditPress->setVisible(false);
        ui->lineEditPrice->setVisible(false);
        ui->lineEditWriter->setVisible(false);
}

management::~management()
{
    delete ui;
}

void management::on_pushButton_clicked()//新增图书，打开新增图书界面
{
    addbook.show();
}

void management::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)//选择了一个分类，显示属于该分类的所有的书籍
{
    for (int k=0;k<pageSize;k++) {
      lab[k].setVisible(false);
      labDetial[k].setVisible(false);
    }
    signOfPageChange=true;
    ui->comboBox->clear();
    QString ca=ui->treeWidget->currentItem()->text(0);
    int i=1;
    for (;i<ca.length();) {
        if(ca.at(i)>='0'&&ca.at(i)<='9')
            i++;
        else
            break;
    }
    ca=ca.mid(0,i);
    for (int k=0;k<buttNum;k++) {
        butt[k].setVisible(false);
    }
    int j=0;
    query.exec("select * from books");
    QString temp="";
    while(query.next())
    {
        if(query.value(6).toString().mid(0,i)==ca)//6是中图法分类
        {
           butt[j].setText(query.value(1).toString());

           butt[j].setProperty("barcode",query.value(0).toString());
           butt[j].setProperty("writer",query.value(2).toString());
           butt[j].setProperty("price",query.value(3).toString());
           butt[j].setProperty("num",query.value(4).toString());
           butt[j].setProperty("press",query.value(5).toString());
           butt[j].setProperty("category",query.value(6).toString());
           j++;
        }
    }
    MaxItem=j;
    if(MaxItem!=0)
    {
        totalPage=MaxItem/pageSize;
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
        labDetial[i].setText(butt[i].property("writer").toString()+"著 "+butt[i].property("press").toString());
        labDetial[i].move(30,k*buffSize+43);
        labDetial[i].setVisible(true);
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



void management::on_pushButtonSearch_clicked()//响应点击搜索按钮，从数据库中查找与搜索信息相关的全部书籍并显示出来
{
    for (int k=0;k<pageSize;k++) {
      lab[k].setVisible(false);
      labDetial[k].setVisible(false);
    }
    QString str=ui->lineEditSearch->text();
    if(str!="")
    {
        ui->lineEditSearch->setText("");
        for (int k=0;k<buttNum;k++) {
            butt[k].setVisible(false);
        }
        int j=0;
        query.exec("select * from books");
        while(query.next())
        {
            for (int i=0;i<7;i++)
            {
                if(query.value(i).toString().contains(str))//6是中图法分类
                {
                    butt[j].setText(query.value(1).toString());

                    butt[j].setProperty("barcode",query.value(0).toString());
                    butt[j].setProperty("writer",query.value(2).toString());
                    butt[j].setProperty("price",query.value(3).toString());
                    butt[j].setProperty("num",query.value(4).toString());
                    butt[j].setProperty("press",query.value(5).toString());
                    butt[j].setProperty("category",query.value(6).toString());
                    j++;
                    break;
                 }
            }
        }
        MaxItem=j;
        int i;
        signOfPageChange=true;
        int totalPage=MaxItem/pageSize;
        if(MaxItem%pageSize!=0)
            totalPage++;
        ui->labelTotalPage->setText("/"+QString::number(totalPage));
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
            labDetial[i%pageSize].setText(butt[i].property("writer").toString()+"著 "+butt[i].property("press").toString());
            labDetial[i%pageSize].move(30,k*buffSize+43);
            labDetial[i%pageSize].setVisible(true);
        }
        currentPage=1;
        currentMaxItem=i;
        ui->pushButtonLastPage->setEnabled(false);
        if(currentMaxItem<MaxItem)
            ui->pushButtonNextPage->setEnabled(true);
        if(currentPage==totalPage)
            ui->pushButtonNextPage->setEnabled(false);
        else
            ui->pushButtonNextPage->setEnabled(true);
        if(currentPage>1)
            ui->pushButtonLastPage->setEnabled(true);
        else
            ui->pushButtonLastPage->setEnabled(false);
    }
}



void management::on_pushButtonChange_clicked()//响应修改按钮，允许管理员修改书籍信息
{
  ui->pushButtonSure->setEnabled(true);
  ui->pushButtonChange->setEnabled(false);
  ui->labelNum->setVisible(false);
  ui->labelName->setVisible(false);
  ui->labelPress->setVisible(false);
  ui->labelPrice->setVisible(false);
  ui->labelWriter->setVisible(false);

  ui->lineEditNum->setText(ui->labelNum->text());
  ui->lineEditName->setText(ui->labelName->text());
  ui->lineEditPress->setText(ui->labelPress->text());
  ui->lineEditPrice->setText(ui->labelPrice->text());
  ui->lineEditWriter->setText(ui->labelWriter->text());

  ui->lineEditNum->setVisible(true);
  ui->lineEditName->setVisible(true);
  ui->lineEditPress->setVisible(true);
  ui->lineEditPrice->setVisible(true);
  ui->lineEditWriter->setVisible(true);
}


void management::on_pushButtonSure_clicked()//响应修改信息的确定按钮，对数据库进行操作
{
    ui->pushButtonSure->setEnabled(false);

    query.prepare("update books set name=?,writer=?,price=?,num=?,press=? where barcode=?");
    query.addBindValue(ui->lineEditName->text());
    query.addBindValue(ui->lineEditWriter->text());
    query.addBindValue(ui->lineEditPrice->text());
    query.addBindValue(ui->lineEditNum->text());
    query.addBindValue(ui->lineEditPress->text());
    query.addBindValue(ui->labelCode->text());
    query.exec();

    ui->labelNum->setText(ui->lineEditNum->text());
    ui->labelName->setText(ui->lineEditName->text());
    ui->labelWriter->setText(ui->lineEditWriter->text());
    ui->labelPress->setText(ui->lineEditPress->text());
    ui->labelPrice->setText(ui->lineEditPrice->text());

    ui->lineEditNum->setVisible(false);
    ui->lineEditName->setVisible(false);
    ui->lineEditPress->setVisible(false);
    ui->lineEditPrice->setVisible(false);
    ui->lineEditWriter->setVisible(false);

    ui->labelNum->setVisible(true);
    ui->labelName->setVisible(true);
    ui->labelPress->setVisible(true);
    ui->labelPrice->setVisible(true);
    ui->labelWriter->setVisible(true);

    butt[currentButton].setText(ui->lineEditName->text());
    ui->pushButtonChange->setEnabled(true);
}

void management::on_pushButtonNextPage_clicked()//响应下一页操作
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
        labDetial[i%pageSize].setText(butt[i].property("writer").toString()+"著 "+butt[i].property("press").toString());
        labDetial[i%pageSize].move(30,j*buffSize+43);
        labDetial[i%pageSize].setVisible(true);
    }
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);
        labDetial[j].setVisible(false);
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




void management::on_pushButtonLastPage_clicked()//响应上一页操作
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
        labDetial[i%pageSize].setText(butt[i].property("writer").toString()+"著 "+butt[i].property("press").toString());
        labDetial[i%pageSize].move(30,j*buffSize+43);
        labDetial[i%pageSize].setVisible(true);
    }
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);
        labDetial[j].setVisible(false);
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

void management::on_comboBox_currentIndexChanged(int index)//通过选择页面进行跳转
{
    int tar=ui->comboBox->currentText().toInt();
    if(signOfPageChange==false)
    {
        int i,j=0;
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
        labDetial[j].setVisible(0);
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

void management::on_pushButtonGoBookManagement_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void management::on_pushButtonGoUserManagement_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void management::on_pushButtonGoReserveManagement_clicked()
{
   ui->stackedWidget->setCurrentIndex(2);
}

void management::on_pushButton_2_clicked()
{
    this->close();
    MainWindow *w=new MainWindow;
    w->show();
}
