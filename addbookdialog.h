#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class addBookDialog;
}

class addBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addBookDialog(QWidget *parent = nullptr);
    ~addBookDialog();

    void creatUI();
    void allConnects();

private slots:
    void slot_affirm();         //确认按钮
    void slot_clear();          //清除按钮
    void slot_lineEditFinish(); //书籍编号输入结束后判断是否已经存在

private:
    Ui::addBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
