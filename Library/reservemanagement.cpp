#include "reservemanagement.h"
#include "ui_reservemanagement.h"
#include<QMessageBox>
#include<QDate>
#include<QDateTime>
#include<QMap>
#include<QDebug>
#include<QModelIndex>
#include<QString>
reserveManagement::reserveManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reserveManagement)
{

    ui->setupUi(this);
    ui->widget_2->hide();
    ui->widget->setVisible(false);
    ui->tableView->setVisible(false);
    ui->pushButtonNextPage->setEnabled(false);
    ui->pushButtonLastPage->setEnabled(false);
    ui->labelHeadRecom->setVisible(0);
    signOfPageChange=true;

    ui->labelHeadOpe->setVisible(0);
    ui->labelHeadUser->setVisible(0);
    ui->labelHeadBorrowDate->setVisible(0);
    ui->labelHeadDeadlineDate->setVisible(0);
    ui->labelHeadRi->setVisible(0);
    ui->labelHeadLeft->setVisible(0);
    ui->labelHeadRecom->setVisible(0);
    ui->labelHeadRi->setVisible(0);

    ui->labelHeadNum->setVisible(0);
    ui->labelHeadUser->setVisible(0);
    ui->labelHeadBook->setVisible(0);

    QSqlDatabase menudb;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
       menudb = QSqlDatabase::database("qt_sql_default_connection");
    else
       menudb = QSqlDatabase::addDatabase("QSQLITE");
    menudb.setDatabaseName("Library.db");

    if (!menudb.open())
        QMessageBox::warning(this, "error", menudb.lastError().text());
    query=QSqlQuery(menudb);
    query.exec("select * from books");
    while(query.next())
    {
        mapBookLeft[query.value(0).toString()]=query.value(4).toString();
    }
    query.exec("create table waterflow"
               "(operation int,bookid int,userid int,"
               "bDate date,rDate date,dDate date, day int,"
               "ifhandle bool, result int, recbook varchar(30)"");");
    for (int i=0;i<pageSize;i++) {
        lab[i].setParent(ui->widget);
        lab[i].setVisible(false);
        buttSure[i].setParent(ui->widget);
        buttReject[i].setParent(ui->widget);
        buttSure[i].setVisible(false);
        buttReject[i].setVisible(false);
        buttSure[i].setText("同意");
        buttReject[i].setText("拒绝");
    }
    for (int i=0;i<labNum;i++) {
        labBookId[i].setParent(ui->widget);
        labBorrowDate[i].setParent(ui->widget);
        labReturnDate[i].setParent(ui->widget);
        labDeadlineDate[i].setParent(ui->widget);
        labBookLeft[i].setParent(ui->widget);
        labUserId[i].setParent(ui->widget);
        labDays[i].setParent(ui->widget);
        labBookId[i].setVisible(false);
        labBorrowDate[i].setVisible(false);
        labReturnDate[i].setVisible(false);
        labDeadlineDate[i].setVisible(false);
        labUserId[i].setVisible(false);
        labBookLeft[i].setVisible(false);
        labDays[i].setVisible(false);
    }
    for (int i=0;i<pageSize;i++)
    {
        connect(&buttReject[i],&QPushButton::clicked, this,[=](){
            QString opeBookId=labBookId[currentPage*pageSize-pageSize+i].text();
            QString opeUserId=labUserId[currentPage*pageSize-pageSize+i].text();
            if(currentOpe==1)
            {
                QString opeBookId=labBookId[currentPage*pageSize-pageSize+i].text();
                QString opeUserId=labUserId[currentPage*pageSize-pageSize+i].text();
                query.prepare("update waterflow set ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(true);
                query.addBindValue(false);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(1);
            }else if(currentOpe==2)//预约
            {

                query.prepare("update waterflow set ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(true);
                query.addBindValue(false);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(2);
            }else if(currentOpe==3)//xuji续借
            {

                query.prepare("update waterflow set ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(true);
                query.addBindValue(false);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(3);
            }else if(currentOpe==4)//huan还书
            {
                currentTime=QDate::currentDate();
                query.prepare("update waterflow set rDate=?, ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(currentTime);
                query.addBindValue(true);
                query.addBindValue(true);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(4);

                query.prepare("update books set num=? where barcode=?");
                query.addBindValue(QString::number(mapBookLeft[opeBookId].toInt()+1));
                query.addBindValue(opeBookId);
                query.exec();
            }
        });
    }
    for (int i=0;i<pageSize;i++)
    {
        connect(&buttSure[i],&QPushButton::clicked, this,[=](){
            QString opeBookId=labBookId[currentPage*pageSize-pageSize+i].text();
            QString opeUserId=labUserId[currentPage*pageSize-pageSize+i].text();
            if(currentOpe==1)
            {
                QString opeBookId=labBookId[currentPage*pageSize-pageSize+i].text();
                QString opeUserId=labUserId[currentPage*pageSize-pageSize+i].text();
                query.prepare("update waterflow set ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(true);
                query.addBindValue(true);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);
                query.exec();

                query.prepare("update books set num=? where barcode=?");
                query.addBindValue(QString::number(mapBookLeft[opeBookId].toInt()-1));
                query.addBindValue(opeBookId);
                query.exec();
                updateBorrow(1);
                mapBookLeft[opeBookId]=QString::number(mapBookLeft[opeBookId].toInt()-1);
            }else if(currentOpe==2)//预约
            {

                query.prepare("update waterflow set ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(true);
                query.addBindValue(1);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(2);
            }else if(currentOpe==3)////////////////////////////////////////xujie、、、、续借
            {
                query.exec("select * from waterflow");
                while(query.next())
                {
                    if(query.value(0).toInt()==currentOpe&&query.value(7).toBool()==false
                            &&query.value(1).toString()==opeBookId&&query.value(2).toString()==opeUserId)//6是中图法分类
                    {
                      currentTime=query.value(4).toDate();
                      break;
                    }
                }
                currentTime=currentTime.addDays(labDays[currentPage*pageSize-pageSize+i].text().toInt());
                query.prepare("update waterflow set dDate=?,ifhandle=?,result=? where userid=? and bookid=?");
                //query.addBindValue(currentTime.toString("yyyy-MM-dd"));
                query.addBindValue(currentTime.toString("yyyy-MM-dd"));
                query.addBindValue(true);
                query.addBindValue(true);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(3);
            }else if(currentOpe==4)//////////////////////////////////////////
            {
                query.exec("select * from waterflow");//cha查看这本书是否被预约过
                int j=0;
                while(query.next())
                {
                    if(query.value(0).toInt()==2&&query.value(7).toBool()==1&&query.value(8).toInt()==1&&query.value(1).toString()==opeBookId)//6是中图法分类
                    {
                       currentTime=QDate::currentDate();
                       int id=query.value(2).toInt();
                       int days=query.value(6).toInt();
                       query.prepare("update waterflow set bDate=?,dDate=?,result=? where userid=? and bookid=?");

                       query.addBindValue(currentTime);
                       currentTime=currentTime.currentDate();
                       currentTime=currentTime.addDays(days);
                       query.addBindValue(currentTime);
                       query.addBindValue(2);
                       query.addBindValue(id);
                       query.addBindValue(opeBookId);
                       query.exec();
                       j++;
                       break;
                    }
                }

                currentTime=QDate::currentDate();
                query.prepare("update waterflow set rDate=?, ifhandle=?,result=? where userid=? and bookid=?");
                query.addBindValue(currentTime);
                query.addBindValue(true);
                query.addBindValue(true);
                query.addBindValue(opeUserId);
                query.addBindValue(opeBookId);

                query.exec();
                updateBorrow(4);

                if(j==0)
                {
                    query.prepare("update books set num=? where barcode=?");
                    query.addBindValue(QString::number(mapBookLeft[opeBookId].toInt()+1));
                    query.addBindValue(opeBookId);
                    query.exec();
                }
            }
        });
    }
}

reserveManagement::~reserveManagement()
{
    delete ui;
}

void reserveManagement::on_pushButtonBorrow_clicked()
{
    updateBorrow(1);
}

void reserveManagement::on_pushButtonLastPage_clicked()
{
    int i,k=0;
    for(i=0;i<pageSize;i++) {
        lab[i].setVisible(false);
        buttSure[i].setVisible(false);
        buttReject[i].setVisible(false);
    }

    for(i=currentPage*pageSize-pageSize;i<currentMaxItem;i++)
    {
        labBookId[i].setVisible(false);
        labBorrowDate[i].setVisible(false);
        labReturnDate[i].setVisible(false);
        labDeadlineDate[i].setVisible(false);
        labUserId[i].setVisible(false);
        labBookLeft[i].setVisible(false);
        labDays[i].setVisible(false);
    }
    currentPage--;

    i=currentPage*pageSize-pageSize;

    k=0;
    for (;i<currentPage*pageSize;i++,k++) {
        lab[i%pageSize].move(0,k*buffSize+50);
        lab[i%pageSize].setText(QString::number(i+1)+".");
        lab[i%pageSize].setVisible(true);
        labBookId[i].move(100,k*buffSize+50);
        labBorrowDate[i].move(200,k*buffSize+50);
        labDeadlineDate[i].move(300,k*buffSize+50);
        labUserId[i].move(20,k*buffSize+50);
        labBookLeft[i].move(400,k*buffSize+50);


        buttSure[i%pageSize].setVisible(true);
        buttReject[i%pageSize].setVisible(true);
        labBookId[i].setVisible(true);
        labBorrowDate[i].setVisible(true);
        labDeadlineDate[i].setVisible(true);
        labUserId[i].setVisible(true);
        labBookLeft[i].setVisible(true);
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

void reserveManagement::on_pushButtonNextPage_clicked()
{
    int i,j=0,k=0;
    for (;j<pageSize;j++) {
        lab[j].setVisible(false);
        buttSure[j].setVisible(false);
        buttReject[j].setVisible(false);
    }
    for(i=currentPage*pageSize-pageSize;i<currentPage*pageSize;i++)
    {
        labBookId[i].setVisible(false);
        labBorrowDate[i].setVisible(false);
        labReturnDate[i].setVisible(false);
        labDeadlineDate[i].setVisible(false);
        labUserId[i].setVisible(false);
        labBookLeft[i].setVisible(false);
    }
    currentPage++;
    k=0;
    for (i=currentPage*pageSize-pageSize;i<MaxItem&&i<currentPage*pageSize;i++,k++) {
        lab[i%pageSize].move(0,k*buffSize+50);
        lab[i%pageSize].setText(QString::number(i+1)+".");
        lab[i%pageSize].setVisible(true);
        labBookId[i].move(100,k*buffSize+50);
        labBorrowDate[i].move(200,k*buffSize+50);
        labDeadlineDate[i].move(300,k*buffSize+50);
        labUserId[i].move(20,k*buffSize+50);
        labBookLeft[i].move(400,k*buffSize+50);

        buttSure[i%pageSize].setVisible(true);
        buttReject[i%pageSize].setVisible(true);
        labBookId[i].setVisible(true);
        labBorrowDate[i].setVisible(true);
        labDeadlineDate[i].setVisible(true);
        labUserId[i].setVisible(true);
        labBookLeft[i].setVisible(true);
    }

    currentMaxItem=i;
    ui->pushButtonLastPage->setEnabled(true);
    if(i>=MaxItem)
        ui->pushButtonNextPage->setEnabled(false);
    else
        ui->pushButtonNextPage->setEnabled(true);
    ui->labelCurrentPage->setText(QString::number(currentPage));
    signOfPageChange=true;
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

void reserveManagement::on_comboBox_currentIndexChanged(int index)
{

    int tar=ui->comboBox->currentText().toInt();
    if(signOfPageChange==false)
    {
        int i,k=0;
        for(i=currentPage*pageSize-pageSize;i<currentMaxItem&&i<currentPage*pageSize;i++) {
            lab[i%pageSize].setVisible(false);
            buttSure[i%pageSize].setVisible(false);
            buttReject[i%pageSize].setVisible(false);
        }

        for(i=currentPage*pageSize-pageSize;i<currentMaxItem&&i<currentPage*pageSize;i++)
        {
            labBookId[i].setVisible(false);
            labBorrowDate[i].setVisible(false);
            labReturnDate[i].setVisible(false);
            labDeadlineDate[i].setVisible(false);
            labUserId[i].setVisible(false);
            labBookLeft[i].setVisible(false);
        }
    currentPage=tar;
    i=currentPage*pageSize-pageSize;
    for (;i<MaxItem&&i<currentPage*pageSize;i++,k++) {
        lab[i%pageSize].move(0,k*buffSize+50);
        lab[i%pageSize].setText(QString::number(i+1)+".");
        lab[i%pageSize].setVisible(true);
        labBookId[i].move(100,k*buffSize+50);
        labBorrowDate[i].move(200,k*buffSize+50);
        labDeadlineDate[i].move(300,k*buffSize+50);
        labUserId[i].move(20,k*buffSize+50);
        labBookLeft[i].move(400,k*buffSize+50);

        buttSure[i].setVisible(true);
        buttReject[i].setVisible(true);
        labBookId[i].setVisible(true);
        labBorrowDate[i].setVisible(true);
        labDeadlineDate[i].setVisible(true);
        labUserId[i].setVisible(true);
        labBookLeft[i].setVisible(true);
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


void reserveManagement::updateBorrow(int ope)
{
    ui->labelHeadOpe->setVisible(true);
    ui->labelHeadUser->setVisible(true);
    ui->labelHeadBorrowDate->setVisible(1);
    ui->labelHeadDeadlineDate->setVisible(1);
    ui->labelHeadRi->setVisible(1);
    ui->labelHeadLeft->setVisible(1);
    ui->labelHeadRecom->setVisible(0);
    ui->labelHeadNum->setVisible(1);
    ui->labelHeadUser->setVisible(1);
    ui->labelHeadBook->setVisible(1);
    ui->labelHeadRi->setText("续借时长");
    if(ope==1)
    {
        ui->labelHeadLeft->setVisible(true);
        ui->labelHeadRi->setVisible(false);
    }
    else if(ope==4)
     {
        ui->labelHeadLeft->setVisible(false);
        ui->labelHeadRi->setVisible(false);
     }else if(ope==3)
    {

    }else if(ope==2)
    {
        //ui->labelHeadRi->setVisible(0);
        ui->labelHeadRi->setText("预约时长");
        ui->labelHeadBorrowDate->setVisible(0);
        ui->labelHeadDeadlineDate->setVisible(0);
    }
    for (int i=0;i<pageSize;i++) {
        lab[i].setParent(ui->widget);
        lab[i].setVisible(false);
        buttSure[i].setParent(ui->widget);
        buttReject[i].setParent(ui->widget);
        buttSure[i].setVisible(false);
        buttReject[i].setVisible(false);
        buttSure[i].setText("同意");
        buttReject[i].setText("拒绝");
    }
    for (int i=0;i<labNum;i++) {
        labBookId[i].setVisible(false);
        labBorrowDate[i].setVisible(false);
        labReturnDate[i].setVisible(false);
        labDeadlineDate[i].setVisible(false);
        labUserId[i].setVisible(false);
        labBookLeft[i].setVisible(false);
        labDays[i].setVisible(false);
    }
    currentOpe=ope;//jie借阅
    query.exec("select * from waterflow");
    int j=0,i;
    while(query.next())
    {
        if(query.value(0).toInt()==ope&&query.value(7).toBool()==false)//6是中图法分类
        {
           labUserId[j].setText(query.value(2).toString());
           labBookId[j].setText(query.value(1).toString());
           labBorrowDate[j].setText(query.value(3).toString());
           labDeadlineDate[j].setText(query.value(5).toString());
           labReturnDate[j].setText(query.value(4).toString());
           labDays[j].setText(query.value(6).toString());
           labBookLeft[j].setText(mapBookLeft[query.value(1).toString()]);
           j++;
        }
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
        buttSure[i].move(520,k*buffSize+50);
        buttReject[i].move(620,k*buffSize+50);
        lab[i].move(0,k*buffSize+50);
        lab[i].setText(QString::number(i+1)+".");
        lab[i].setVisible(true);

        labBookId[i].move(100,k*buffSize+50);
        labBorrowDate[i].move(200,k*buffSize+50);
        labDeadlineDate[i].move(300,k*buffSize+50);
        labUserId[i].move(20,k*buffSize+50);
        if(ope==1)
            labBookLeft[i].move(400,k*buffSize+50);
        else if(ope==3||ope==2)
            labDays[i].move(400,k*buffSize+50);

        buttSure[i].setVisible(true);
        buttReject[i].setVisible(true);
        labBookId[i].setVisible(true);
        labBorrowDate[i].setVisible(true);
        labDeadlineDate[i].setVisible(true);
        labUserId[i].setVisible(true);

        if(currentOpe==1)
        {
            labBookLeft[i].setVisible(true);
            labDays[i].setVisible(false);
        }else if(currentOpe==3)
        {
            labBookLeft[i].setVisible(false);
            labDays[i].setVisible(true);
        }else if(currentOpe==2)
        {
            labDays[i].setVisible(true);

        }
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
    }
}

void reserveManagement::on_pushButtonReturn_clicked()
{
    updateBorrow(4);
}

void reserveManagement::on_pushButtonRinnovare_clicked()
{
    updateBorrow(3);
}

void reserveManagement::on_pushButtonUserRecomend_clicked()
{
    ui->labelHeadOpe->setVisible(0);
    ui->labelHeadUser->setVisible(0);
    ui->labelHeadBorrowDate->setVisible(0);
    ui->labelHeadDeadlineDate->setVisible(0);
    ui->labelHeadRi->setVisible(0);
    ui->labelHeadLeft->setVisible(0);
    ui->labelHeadRecom->setVisible(1);
    ui->labelHeadNum->setVisible(1);
    ui->labelHeadBook->setVisible(1);
    for (int i=0;i<pageSize;i++) {
        lab[i].setVisible(false);
    }
    for (int i=0;i<labNum;i++) {
        labBookId[i].setVisible(false);
        labBorrowDate[i].setVisible(false);
        labReturnDate[i].setVisible(false);
        labDeadlineDate[i].setVisible(false);
        labUserId[i].setVisible(false);
        labBookLeft[i].setVisible(false);
        labDays[i].setVisible(false);
    }
    currentOpe=5;//jie借阅
    QMap<QString,int> recom;
    query.exec("select * from waterflow");
    int i=0;
    while(query.next())
    {
        if(query.value(0).toInt()==currentOpe)//6是中图法分类
        {
           recom[query.value(9).toString()]++;
        }
    }
    QMap<QString,int>::iterator it=recom.begin();
    for (;i<pageSize&&it!=recom.end();it++,i++) {
        labBookId[i].setText(it.key());
        lab[i].setText(QString::number(i+1));
        labUserId[i].setText(QString::number(it.value()));
        lab[i].move(0,i*buffSize+50);
        labBookId[i].move(100,i*buffSize+50);
        labUserId[i].move(250,i*buffSize+50);
        lab[i].setVisible(true);
        labBookId[i].setVisible(true);
        labUserId[i].setVisible(true);
    }
}
bool reserveManagement::Cmp(int a, int b)
{
    return a<b;
}

void reserveManagement::on_pushButtonReserve_clicked()//预约
{
    updateBorrow(2);
}

void reserveManagement::on_pushButtonUndeal_clicked()
{
    ui->widget->show();
    ui->widget_2->show();
    ui->tableView->hide();
}

void reserveManagement::on_pushButtonDealt_clicked()
{
    mapBookLeft.clear();
    query.exec("select * from books");
    while(query.next())
    {
        mapBookLeft[query.value(0).toString()]=query.value(1).toString();
    }
    ui->widget->hide();
    model.setTable("waterflow");
    model.setHeaderData(0, Qt::Horizontal, "    操作     ");
    model.setHeaderData(1, Qt::Horizontal, "       书籍              ");
    model.setHeaderData(2, Qt::Horizontal, "    用户       ");
    model.setHeaderData(3, Qt::Horizontal, "    借阅日期          ");
    model.setHeaderData(4, Qt::Horizontal, "    归还日期          ");
    model.setHeaderData(5, Qt::Horizontal, "    截至归还日期     ");
    model.setHeaderData(6, Qt::Horizontal, "续借/预约时长");
    model.setHeaderData(7, Qt::Horizontal, "    是否处理   ");
    model.setHeaderData(8, Qt::Horizontal, "    处理结果     ");
    model.setHeaderData(9, Qt::Horizontal, "    推荐书籍     ");
    ui->tableView->setModel(&model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->resizeColumnsToContents();
    model.setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString fil="ifhandle like 1 ";
    model.setFilter(fil);
    model.select();
    int rowC=model.rowCount();
    for (int i=0;i<rowC;i++) {
        QModelIndex index=model.index(i,0,QModelIndex());//rowNum,columnNum为行列号
        QString str= index.data().toString();//
        if(str=="1")
            str="借阅";
        else if(str=="2")
            str="预约";
        else if(str=="3")
            str="续借";
        else if(str=="4")
            str="还书";
        else if(str=="5")
            str="推荐图书";

        model.setData(model.index(i,0),str);

        index=model.index(i,8,QModelIndex());//rowNum,columnNum为行列号
        str= index.data().toString();//
        if(str=="1")
             str="同意";
        else if(str=="0")
             str="拒绝";
        else str="等待图书到来";

        model.setData(model.index(i,8),str);
        str="已处理";
        model.setData(model.index(i,7),str);
        index=model.index(i,1,QModelIndex());//rowNum,columnNum为行列号
        str= index.data().toString();//
        str=mapBookLeft[str];
        if(str!="")
            model.setData(model.index(i,1),str);
    }


    ui->tableView->show();
}



