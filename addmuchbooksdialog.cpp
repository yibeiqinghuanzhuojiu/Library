#include "addmuchbooksdialog.h"
#include "ui_addmuchbooksdialog.h"
#include "connectDatabase.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QSqlQuery>

addMuchBooksDialog::addMuchBooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addMuchBooksDialog)
{
    ui->setupUi(this);
    creatUI();
    allConnects();
}

addMuchBooksDialog::~addMuchBooksDialog()
{
    delete ui;
}

void addMuchBooksDialog::creatUI()
{
    ui->lineEdit_showFilePath->setPlaceholderText("请选择正确的文件!");
    ui->lineEdit_showFilePath->setEnabled(false);
}

void addMuchBooksDialog::allConnects()
{

    QObject::connect(ui->Button_selectFile, &QPushButton::clicked, this, &addMuchBooksDialog::slot_selectFile);
    QObject::connect(ui->Button_affirmAdd, &QPushButton::clicked, this, &addMuchBooksDialog::slot_affirmAdd);
}

void addMuchBooksDialog::slot_selectFile()
{
    QString qsFilePath;
    qsFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"));
    QFile fp(qsFilePath);
    if (!fp.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(NULL, "提示", "文件打开失败!");
        return;
    }
    else
    {
        ui->tableWidget_addMuchBooks->clear();
        ui->tableWidget_addMuchBooks->setColumnCount(0);
        ui->tableWidget_addMuchBooks->setRowCount(0);
        ui->lineEdit_showFilePath->setText(qsFilePath);
    }

    int nRowCount = 0;
    int nColunmn = 9; //固定的列数
    // 设置表格的列数
    ui->tableWidget_addMuchBooks->setColumnCount(nColunmn);
    // 增加表头
    QStringList headers = {"书籍编号", "书名", "作者", "书籍类型", "价格", "出版社", "出版时间", "图书余量", "备注"};
    ui->tableWidget_addMuchBooks->setHorizontalHeaderLabels(headers);

    QTextStream stream(&fp);
    while (!stream.atEnd())
    {
        QString qsLine = stream.readLine();
        QStringList qsFieldsList = qsLine.split(',');
        ui->tableWidget_addMuchBooks->insertRow(nRowCount);
        for (int i = 0; i < qsFieldsList.size(); ++i) {
            QTableWidgetItem* item = new QTableWidgetItem(qsFieldsList.at(i));
            ui->tableWidget_addMuchBooks->setItem(nRowCount, i, item);
        }
        c_setBooks.insert(qsFieldsList);
        ++nRowCount;
    }
}

void addMuchBooksDialog::slot_affirmAdd()
{
    if (g_database.open())
    {
        int nAddSuccess = 0;
        int nAddFailed = 0;
        QString qsAddFiled;
        for (const QStringList& bookList : c_setBooks)
        {
            QSqlQuery query(g_database);
            query.prepare("SELECT 1 FROM book_detail WHERE 书籍编号 = :id");
            query.bindValue(":id", bookList.at(0));
            if (query.exec() && query.next())
            {
                nAddFailed += 1;
                qsAddFiled += bookList.at(0) + ",";
                c_setBooks.remove(bookList);
                continue;
            }
            query.prepare("INSERT INTO book_detail VALUES(:id, :name, :auth, :type, :price, :publisher, :publishdate, :num, :other)");
            query.bindValue(":id", bookList.at(0));
            query.bindValue(":name", bookList.at(1));
            query.bindValue(":auth", bookList.at(2));
            query.bindValue(":type", bookList.at(3));
            query.bindValue(":price", bookList.at(4));
            query.bindValue(":publisher", bookList.at(5));
            query.bindValue(":publishdate", bookList.at(6));
            query.bindValue(":num", bookList.at(7));
            query.bindValue(":other", bookList.at(8));
            if (query.exec())
            {
                nAddSuccess += 1;
            }
            else
            {
                QMessageBox::information(NULL, "提示", "添加失败!");
            }
        }
        c_setBooks.clear();
        if (nAddFailed > 0)
        {
            QMessageBox::information(NULL, "提示", QString("%1本书籍添加成功!\n%2本书籍已存在,分别是%3请单独添加!").arg(nAddSuccess).arg(nAddFailed).arg(qsAddFiled));
        }
        else
        {
            QMessageBox::information(NULL, "提示", QString("%1本书籍添加成功!").arg(nAddSuccess));
        }

    }
}
