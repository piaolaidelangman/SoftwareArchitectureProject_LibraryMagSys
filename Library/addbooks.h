#ifndef ADDBOOKS_H
#define ADDBOOKS_H

#include <QWidget>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QSqlTableModel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
namespace Ui {
class addBooks;
}

class addBooks : public QWidget
{
    Q_OBJECT

public:
    explicit addBooks(QWidget *parent = nullptr);
    ~addBooks();

private slots:

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);


    void on_pushButton_clicked();

private:
    Ui::addBooks *ui;
    QString category;
    QSqlQuery query;
};

#endif // ADDBOOKS_H
