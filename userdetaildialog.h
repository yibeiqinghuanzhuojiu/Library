#ifndef USERDETAILDIALOG_H
#define USERDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class userDetailDialog;
}

class userDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userDetailDialog(QWidget *parent = nullptr);
    ~userDetailDialog();

    void allConnects();
    void initUI(const QString& id);

private slots:
    void slot_changeUserDetail();
    void slot_deletUser();

private:
    Ui::userDetailDialog *ui;
    QString     c_userID;
    QString     c_qsAuth;
    QString     c_qsUserName;
    QString     c_qsPassWord;
};

#endif // USERDETAILDIALOG_H
