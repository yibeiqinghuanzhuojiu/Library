#ifndef COMMON_H
#define COMMON_H

#include <bookDef.h>
#include <QString>
#include <QtSql>
#include <QTableWidget>
#include <QRegularExpressionValidator>

/*
枚举类型的定义
权限定义：Authority

*/
enum Authority
{
    AUTH_MANAGER             =0,
    AUTH_USER,
    AUTH_VISITOR,
    AUTH_USER_NOTBORROW      =-1//备用权限，是注册用户但是不可以借阅书籍

};

//书籍类型定义
extern QStringList categories;
//账号输入掩码：只允许输入6-8位的数字
extern QRegularExpressionValidator *validator;

/*
一些通用函数的定义类
包括数据库内容匹配或选择
*/
//重构Qt自带的排序
class NewTableWidgetItem : public QTableWidgetItem
{
public:
    NewTableWidgetItem(const QString& item) : QTableWidgetItem(item)
    {

    }
    bool operator< (const QTableWidgetItem &other) const
    {
        return (this->text().toInt() < other.text().toInt());
    }
};

//程序中各种表格的数据查询、表格刷新、数据显示、控件添加
extern void refreshTables(QSqlQuery query/*数据库查询结果*/, int nAuth/*用户权限*/, QTableWidget* table);
//权限判断
extern QString judgeAuthority(const int& nAuth);


#endif // COMMON_H
