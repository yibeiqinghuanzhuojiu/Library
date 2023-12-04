#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    loginDialog(QWidget* parent = nullptr);
    ~loginDialog();

    void creatUI();
    void allConnects(); // 所有信号连接

signals:
    void sigLoginSuccess(const QString& qsUserID, const QString& qsUserName, const QString& qsPassWord, const QString& qsAuthority);

private slots:
    void slot_onLoginClicked();
    void slot_onRegisterClicked();

private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
