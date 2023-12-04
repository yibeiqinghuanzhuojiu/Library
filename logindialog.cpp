#include "logindialog.h"
#include "ui_logindialog.h"
#include "connectDatabase.h"
#include "registerdialog.h"
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    creatUI();
    allConnects();
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::creatUI()
{
    //权限combox添加元素
    QStringList qsAuthList = {"管理员", "普通用户"};
    ui->comboBox_authority->addItems(qsAuthList);

    ui->lineEdit_ID->setPlaceholderText("请输入ID或者昵称");
}

void loginDialog::allConnects()
{
    QObject::connect(ui->Button_login, &QPushButton::clicked, this, &loginDialog::slot_onLoginClicked);
    QObject::connect(ui->Button_register, &QPushButton::clicked, this, &loginDialog::slot_onRegisterClicked);
}

void loginDialog::slot_onLoginClicked()
{
    if (g_database.open())
    {
        QString qsUserID = ui->lineEdit_ID->text();
        QString qsPassWord = ui->lineEdit_passWord->text();
        QString qsAuthority = ui->comboBox_authority->currentText();
        if (qsAuthority == "管理员")
        {
            qsAuthority = "0";
        }
        else if (qsAuthority == "普通用户")
        {
            qsAuthority = "1";
        }

        QSqlQuery query(g_database);

        query.prepare("SELECT * FROM user_detail WHERE "
                      "(用户ID = :userID OR 用户昵称 = :userName) "
                      "AND 用户密码 = :password "
                      "AND 权限 = :auth");
        query.bindValue(":userID", qsUserID);
        query.bindValue(":userName", qsUserID);
        query.bindValue(":password", qsPassWord);
        query.bindValue(":auth", qsAuthority.toInt());

        if (query.exec() && query.next())
        {
            //登陆成功
            QString qsUserName = query.value("用户昵称").toString();
            QString qsUseID = query.value("用户ID").toString();
            emit sigLoginSuccess(qsUseID, qsUserName, qsPassWord, qsAuthority);
            QDialog::accept();
        }
        else
        {
            //登陆失败
            QMessageBox::information(NULL, "登陆失败", "请仔细检查登录信息是否正确!");
        }
    }
    else
    {
        QMessageBox::information(NULL, "提示", "databaes is not open!");
        qDebug() << g_database.lastError().text();
    }
}

void loginDialog::slot_onRegisterClicked()
{
    QDialog::reject();
    registerDialog dialog_register(this);
    dialog_register.exec();
}
