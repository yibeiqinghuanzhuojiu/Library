#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include "common.h"
#include "connectDatabase.h"
#include <QMessageBox>

addBookDialog::addBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBookDialog)
{
    ui->setupUi(this);
    creatUI();
    allConnects();
}

addBookDialog::~addBookDialog()
{
    delete ui;
}

void addBookDialog::creatUI()
{
    ui->comboBox_type->insertItems(0, categories);
    ui->comboBox_type->setCurrentText("");
    ui->lineEdit_publishDate->setPlaceholderText("时间格式为yyyy/MM//dd");
}

void addBookDialog::allConnects()
{
    QObject::connect(ui->Button_affirm, &QPushButton::clicked, this, &addBookDialog::slot_affirm);
    QObject::connect(ui->Button_clear, &QPushButton::clicked, this, &addBookDialog::slot_clear);
    QObject::connect(ui->lineEdit_bookID, &QLineEdit::editingFinished, this, &addBookDialog::slot_lineEditFinish);
}

void addBookDialog::slot_affirm()
{
    if (g_database.isOpen())
    {
        QString qsBookID = ui->lineEdit_bookID->text();
        QString qsBookName = ui->lineEdit_bookName->text();
        QString qsAuth = ui->lineEdit_anthor->text();
        QString qsPublisher = ui->lineEdit_publisher->text();
        QString qsPublishDate = ui->lineEdit_publishDate->text();
        QString qsPrice = ui->lineEdit_price->text();
        QString qsNum = ui->lineEdit_num->text();
        QString qsType = ui->comboBox_type->currentText();
        QString qsOther = ui->lineEdit_other->text();

        QSqlQuery query(g_database);
        QString qsQuery = QString("SELECT 1 FROM book_detail WHERE 书籍编号 = %1").arg(qsBookID);
        if (query.exec(qsQuery) && query.next())
        {
            if (!qsBookName.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 书名 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsBookName);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsAuth.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 作者 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsAuth);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsPublisher.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 出版社 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsPublisher);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsPublishDate.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 出版时间 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsPublishDate);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsPrice.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 价格 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsPrice);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsNum.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 图书余量 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsNum);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsType.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 书籍类型 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsType);
                query.bindValue(":id", qsBookID);
                query.exec();
            }
            if (!qsOther.isEmpty())
            {
                query.prepare("UPDATE book_detail SET 备注 = :name WHERE 书籍编号 = :id");
                query.bindValue(":name", qsOther);
                query.bindValue(":id", qsBookID);
                query.exec();
            }

            QMessageBox::information(NULL, "提示", "更新书籍信息成功!");
        }
        else
        {
            query.prepare("INSERT INTO book_detail VALUES(:id, :name, :auth, :type, :price, :publisher, :publishdate, :num, :other)");
            query.bindValue(":id", qsBookID);
            query.bindValue(":name", qsBookName);
            query.bindValue(":auth", qsAuth);
            query.bindValue(":type", qsType);
            query.bindValue(":price", qsPrice);
            query.bindValue(":publisher", qsPublisher);
            query.bindValue(":publishdate", qsPublishDate);
            query.bindValue(":num", qsNum);
            query.bindValue(":other", qsOther);
            if (query.exec())
            {
                QMessageBox::information(NULL, "提示", "添加书籍成功!");
            }
            else
            {
                QMessageBox::information(NULL, "提示", query.lastError().text());
            }
        }
    }
}

void addBookDialog::slot_clear()
{
    ui->lineEdit_bookID->clear();
    ui->lineEdit_bookName->clear();
    ui->lineEdit_anthor->clear();
    ui->lineEdit_price->clear();
    ui->lineEdit_publishDate->clear();
    ui->lineEdit_publisher->clear();
    ui->lineEdit_num->clear();
    ui->lineEdit_other->clear();
}

void addBookDialog::slot_lineEditFinish()
{
    if (g_database.isOpen())
    {
        QString qsBookID = ui->lineEdit_bookID->text();
        QSqlQuery query(g_database);
        QString qsQuery = QString("SELECT 1 FROM book_detail WHERE 书籍编号 = %1").arg(qsBookID);
        if (query.exec(qsQuery))
        {
            if (query.next())
            {
                ui->Button_affirm->setText("修改书籍信息");
            }
            else
            {
                ui->Button_affirm->setText("确认添加");
            }
        }
        else
        {
            QMessageBox::information(NULL, "提示", query.lastError().text());
        }
    }
}
