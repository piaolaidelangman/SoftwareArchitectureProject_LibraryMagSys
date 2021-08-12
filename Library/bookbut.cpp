#include "bookbut.h"

bookBut::bookBut(int t,int id,QString text,QWidget *parent) : QPushButton(parent)
{
    this->userid=id;
    this->setText(text);
    connect(this,SIGNAL(clicked()),this,SLOT(slotclicked()));
    type=t;
}

void bookBut::slotclicked()
{
    BookDialog *dlg=new BookDialog(type,userid,this->text(),this);
    dlg->setWindowTitle(this->text());
    dlg->show();
}


