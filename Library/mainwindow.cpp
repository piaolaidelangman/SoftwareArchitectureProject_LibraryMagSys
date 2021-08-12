#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<login.h>
#include<signin.h>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    this->setWindowTitle("奇怪的图书馆");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_clicked()//点击进入登录界面
{
    login *temp=new login;
    temp->show();
    this->close();
}

void MainWindow::on_signin_clicked()//点击进入注册界面
{
    signin *temp=new signin;
    temp->show();
    this->close();

}
