#ifndef RECBOOKDIALOG_H
#define RECBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class recBookDialog;
}

class recBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit recBookDialog(int id=1,QWidget *parent = 0);
    ~recBookDialog();

    int userid;

private slots:
    void on_okBut_clicked();

    void on_cancle_clicked();

private:
    Ui::recBookDialog *ui;
};

#endif // RECBOOKDIALOG_H
