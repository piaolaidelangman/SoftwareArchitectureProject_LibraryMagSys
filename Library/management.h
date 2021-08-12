#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QMainWindow>
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<addbooks.h>
#include<reservemanagement.h>
#include<usermanagement.h>
#include<QPushButton>
#include<QLabel>
namespace Ui {
class management;
}

class management : public QMainWindow
{
    Q_OBJECT

public:
    explicit management(QWidget *parent = nullptr);
    ~management();

private slots:
    void on_pushButton_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButtonSearch_clicked();

    void on_pushButtonChange_clicked();

    void on_pushButtonSure_clicked();

    void on_pushButtonNextPage_clicked();

    void on_pushButtonLastPage_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButtonGoBookManagement_clicked();

    void on_pushButtonGoUserManagement_clicked();

    void on_pushButtonGoReserveManagement_clicked();

    void on_pushButton_2_clicked();

private:
    const int buttSizeWidth=200;//按钮的宽度
    const int buttSizeHeight=40;//按钮的高度
    const int buffSize=80;//每个项目所占的高度
    static const int pageSize=8;//每页能拥有的最大项目数
    static const int buttNum=20;//按钮数组的大小
    bool signOfPageChange;
    Ui::management *ui;
    addBooks addbook;
    reserveManagement resMan;
    userManagement userMan;
    QSqlQuery query;
    QPushButton butt[buttNum];
    QLabel lab[pageSize];
    QLabel labDetial[pageSize];
    int currentButton;//当前点击的按钮
    int currentPage;//当前页
    int currentMaxItem;//当前页的最大的项
    int MaxItem;//全部项
    int totalPage;

};

#endif // MANAGEMENT_H
