#ifndef BOOKBUT_H
#define BOOKBUT_H

#include <QObject>
#include<QPushButton>
#include<bookdialog.h>
class bookBut : public QPushButton
{
    Q_OBJECT
public:
    explicit bookBut(int t=1,int id=1,QString text="", QWidget *parent = 0);
    int userid;
    int type;
signals:

public slots:
    void slotclicked();
};

#endif // BOOKBUT_H
