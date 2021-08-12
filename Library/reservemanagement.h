#ifndef RESERVEMANAGEMENT_H
#define RESERVEMANAGEMENT_H

#include <QWidget>
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QPushButton>
#include<QLabel>
#include<QMap>
#include<QDate>
#include<QSqlTableModel>
namespace Ui {
class reserveManagement;
}

class reserveManagement : public QWidget
{
    Q_OBJECT

public:
    explicit reserveManagement(QWidget *parent = nullptr);
    void updateBorrow(int ope);
    bool Cmp(int a,int b);
    ~reserveManagement();

private slots:
    void on_pushButtonBorrow_clicked();

    void on_pushButtonLastPage_clicked();

    void on_pushButtonNextPage_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButtonReturn_clicked();

    void on_pushButtonRinnovare_clicked();

    void on_pushButtonUserRecomend_clicked();

    void on_pushButtonReserve_clicked();

    void on_pushButtonUndeal_clicked();

    void on_pushButtonDealt_clicked();

private:
    Ui::reserveManagement *ui;
    const int buttSizeWidth=200;//按钮的宽度
    const int buttSizeHeight=40;//按钮的高度
    const int buffSize=80;//每个项目所占的高度
    static const int pageSize=8;//每页能拥有的最大项目数
    static const int labNum=20;//按钮数组的大小
    bool signOfPageChange;
    QSqlQuery query;
    QPushButton buttSure[pageSize];
    QPushButton buttReject[pageSize];
    QDate currentTime;
    QLabel lab[pageSize];

    QLabel labUserId[labNum];//显示申请用户的id
    QLabel labBookId[labNum];//显示操作书籍的id
    QLabel labBorrowDate[labNum];//显示用户的借书日期
    QLabel labReturnDate[labNum];//显示用户的实际还书日期
    QLabel labDeadlineDate[labNum];//显示用户的还书的截至日期
    QLabel labBookLeft[labNum];
    QLabel labDays[labNum];

    QSqlTableModel model;
    QMap<QString,QString> mapBookLeft;
    int currentButton;//当前点击的按钮
    int currentOpe;
    int currentPage;//当前页
    int currentMaxItem;//当前页的最大的项
    int MaxItem;//全部项
    int totalPage;
};

#endif // RESERVEMANAGEMENT_H
