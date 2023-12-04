#include "userdetaildialog.h"
#include "ui_userdetaildialog.h"
#include "changeuserdetaildialog.h"
#include "connectDatabase.h"
#include "common.h"
#include <QSqlQuery>
#include <QMessageBox>

userDetailDialog::userDetailDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::userDetailDialog)
    , c_userID("")
    , c_qsAuth("")
    , c_qsUserName("")
    , c_qsPassWord("")
{
    ui->setupUi(this);
    allConnects();
}

userDetailDialog::~userDetailDialog()
{
    delete ui;
}

void userDetailDialog::allConnects()
{
    QObject::connect(ui->Button_changeDetail, &QPushButton::clicked, this, &userDetailDialog::slot_changeUserDetail);
    QObject::connect(ui->Button_delet, &QPushButton::clicked, this, &userDetailDialog::slot_deletUser);
}

void userDetailDialog::initUI(const QString& id)
{
    /*设置界面lineEdit不可编辑*/
    ui->lineEdit_ID->setReadOnly(true);
    ui->lineEdit_authority->setReadOnly(true);
    ui->lineEdit_bookNum->setReadOnly(true);
    ui->lineEdit_name->setReadOnly(true);
    ui->lineEdit_passWord->setReadOnly(true);

    c_userID = id;
    if (g_database.open())
    {
        QSqlQuery query(g_database);
        query.prepare("SELECT * FROM user_detail WHERE 用户ID = :id");
        query.bindValue(":id", c_userID);
        if (query.exec() && query.next())
        {

            c_qsUserName = query.value("用户昵称").toString();
            c_qsPassWord = query.value("用户密码").toString();
            int nAuth = query.value("权限").toInt();
            c_qsAuth = judgeAuthority(nAuth);
            ui->lineEdit_ID->setText(c_userID);
            ui->lineEdit_name->setText(c_qsUserName);
            ui->lineEdit_passWord->setText(c_qsPassWord);
            ui->lineEdit_authority->setText(c_qsAuth);
        }
        //查询该用户借阅记录
        query.prepare("SELECT * FROM borrow WHERE 用户ID = :id");
        query.bindValue(":id", c_userID);
        if (query.exec())
        {
            refreshTables(query, QString(c_qsAuth).toInt(), ui->tableWidget_borrowDetail);
            ui->lineEdit_bookNum->setText(QString::number(query.size()));
        }

    }
}

void userDetailDialog::slot_changeUserDetail()
{
    changeUserDetailDialog dialog_changeUser(this);
    dialog_changeUser.initInfor(c_userID, c_qsUserName, c_qsPassWord, c_qsAuth);
    dialog_changeUser.exec();
}

void userDetailDialog::slot_deletUser()
{
    QMessageBox myBox;
    myBox.setText("警告!");
    myBox.setInformativeText("确认删除该用户吗？此行为不可撤销!");
    myBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    myBox.setDefaultButton(QMessageBox::Ok);
    int ret = myBox.exec();
    if (ret == QMessageBox::Ok)
    {
        if (g_database.open())
        {
            QSqlQuery query(g_database);
            query.prepare("DELETE FROM user_detail WHERE 用户ID = :id");
            query.bindValue(":id", c_userID);
            query.exec();
            QMessageBox::information(NULL, "提示", "删除成功!");
            QDialog::accept();
        }
    }
}
