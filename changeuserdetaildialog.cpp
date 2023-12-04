#include "changeuserdetaildialog.h"
#include "ui_changeuserdetaildialog.h"
#include "connectDatabase.h"
#include "common.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

changeUserDetailDialog::changeUserDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeUserDetailDialog)
{
    ui->setupUi(this);
    creatUI();
    QObject::connect(ui->Button_changeDetail, &QPushButton::clicked, this, &changeUserDetailDialog::slot_changeUser);
}

changeUserDetailDialog::~changeUserDetailDialog()
{
    delete ui;
}

void changeUserDetailDialog::creatUI()
{
    //权限combox添加元素
    QStringList qsAuthList = {"管理员", "普通用户"};
    ui->comboBox_authority->addItems(qsAuthList);
    //设置ID不可编辑,因为ID不能修改
    ui->lineEdit_ID->setReadOnly(true);

    ui->lineEdit_ID->setValidator(validator);
}

void changeUserDetailDialog::initInfor(const QString& id, const QString& name, const QString& password, const QString& auth)
{
    ui->lineEdit_name->setText(name);
    ui->lineEdit_ID->setText(id);
    ui->lineEdit_passWord->setText(password);
    ui->comboBox_authority->setCurrentText(auth);
}

void changeUserDetailDialog::setUI(const QString& title, const QString& tip)
{
    // 添加用户的时候ID是可以编辑的
    ui->lineEdit_ID->setReadOnly(false);
    this->setWindowTitle(title);
    ui->label_title->setText(tip);
}

void changeUserDetailDialog::slot_changeUser()
{
    if (ui->lineEdit_ID->text().isEmpty() || ui->lineEdit_name->text().isEmpty() || ui->lineEdit_passWord->text().isEmpty())
    {
        QMessageBox::information(NULL, "提示", "请填写完整的信息!");
        return;
    }

    if (g_database.open())
    {
        int nAuth = 1; //权限默认为普通用户
        if ("管理员" == ui->comboBox_authority->currentText())
        {
            nAuth = AUTH_MANAGER;
        }
        else
        {
            nAuth = AUTH_USER;
        }

        QSqlQuery query(g_database);
        if ("添加用户" == ui->label_title->text())
        {
            query.prepare("INSERT INTO user_detail VALUES(:id, :name, :password, :auth)");
            query.bindValue(":name", ui->lineEdit_name->text());
            query.bindValue(":password", ui->lineEdit_passWord->text());
            query.bindValue(":auth", nAuth);
            query.bindValue(":id", ui->lineEdit_ID->text());
            if (query.exec())
            {
                QMessageBox::information(NULL, "提示", "添加用户成功!");
            }
            else
            {
                QMessageBox::information(NULL, "提示", query.lastError().text());
            }
        }
        else
        {
            query.prepare("UPDATE user_detail SET 用户昵称 = :name, 用户密码 = :password, 权限 = :auth WHERE 用户ID = :id");
            query.bindValue(":name", ui->lineEdit_name->text());
            query.bindValue(":password", ui->lineEdit_passWord->text());
            query.bindValue(":auth", nAuth);
            query.bindValue(":id", ui->lineEdit_ID->text());
            if (query.exec())
            {
                QMessageBox::information(NULL, "提示", "更新用户信息成功!");
            }
            else
            {
                QMessageBox::information(NULL, "提示", query.lastError().text());
            }
        }
    }
}
