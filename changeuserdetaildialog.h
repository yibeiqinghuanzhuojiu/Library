#ifndef CHANGEUSERDETAILDIALOG_H
#define CHANGEUSERDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class changeUserDetailDialog;
}

class changeUserDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit changeUserDetailDialog(QWidget *parent = nullptr);
    ~changeUserDetailDialog();

    void creatUI();
    void initInfor(const QString& id, const QString& name, const QString& password, const QString& auth);
    void setUI(const QString& title/*dialog标题*/, const QString& tip/*界面label文字*/);     //编辑用户信息和添加用户有不同的界面显示

private slots:
    void slot_changeUser();

private:
    Ui::changeUserDetailDialog *ui;
};

#endif // CHANGEUSERDETAILDIALOG_H
