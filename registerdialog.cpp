#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "connectDatabase.h"
#include "common.h"
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>

using namespace std;

registerDialog::registerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    creatUI();
    allConnects();
}

registerDialog::~registerDialog()
{
    delete ui;
}

void registerDialog::creatUI()
{
    ui->lineEdit_ID->setPlaceholderText("请输入6-8位数字");
    ui->lineEdit_ID->setValidator(validator);
    ui->comboBox_authority->addItem("普通用户");
}

void registerDialog::allConnects()
{
    QObject::connect(ui->Button_affirm, &QPushButton::clicked, this, &registerDialog::slot_buttonclicked);
}

void registerDialog::slot_buttonclicked()
{
    if (g_database.open())
    {
        QString qsID = ui->lineEdit_ID->text();
        QString qsName = ui->lineEdit_name->text();
        QString qsPassWord = ui->lineEdit_passWord->text();
        QSqlQuery query(g_database);
        query.prepare("SELECT EXISTS (SELECT 1 FROM user_detail WHERE 用户ID = :id)");
        query.bindValue(":id", qsID);
        if (!qsID.isEmpty() && !qsName.isEmpty() && !qsPassWord.isEmpty())
        {
            if (query.exec() && query.next())
            {
                if (query.value(0).toBool())
                {
                    QMessageBox::information(NULL, "提示", "账号已存在!");
                }
                else
                {
                    query.prepare("INSERT INTO user_detail (用户ID, 用户昵称, 用户密码, 权限) VALUES (:id, :name, :password, :auth)");
                    query.bindValue(":id", qsID);
                    query.bindValue(":name", qsName);
                    query.bindValue(":password", qsPassWord);
                    query.bindValue(":auth", 1);
                    if (query.exec())
                    {
                        QMessageBox::information(NULL, "提示", "注册成功!");
                        QDialog::accept();
                    }
                    else
                    {
                        QMessageBox::information(NULL, "提示", query.lastError().text());
                    }
                }
            }
        }
        else
        {
            QMessageBox::information(NULL, "提示", "请输入完整信息再注册!");
        }
    }
}
