#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QWidget>
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<addbooks.h>
#include<QPushButton>
#include<QLabel>
namespace Ui {
class userManagement;
}

class userManagement : public QWidget
{
    Q_OBJECT

public:
    explicit userManagement(QWidget *parent = nullptr);
    ~userManagement();

private slots:
    void on_pushButtonLastPage_clicked();

    void on_pushButtonNextPage_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::userManagement *ui;
    const int buttSizeWidth=200;//按钮的宽度
    const int buttSizeHeight=40;//按钮的高度
    const int buffSize=80;//每个项目所占的高度
    static const int pageSize=8;//每页能拥有的最大项目数
    static const int buttNum=20;//按钮数组的大小
    bool signOfPageChange;
    QSqlQuery query;
    QPushButton butt[buttNum];
    QLabel lab[pageSize];
    int currentButton;//当前点击的按钮
    int currentPage;//当前页
    int currentMaxItem;//当前页的最大的项
    int MaxItem;//全部项
    int totalPage;
};

#endif // USERMANAGEMENT_H
