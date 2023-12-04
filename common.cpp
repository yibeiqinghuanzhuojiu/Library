#include "common.h"

//书籍类型定义
QStringList categories = { "文学类", "艺术类", "自然科学类", "计算机类", "思政类", "经济类", "哲学类", "军体类",
                          "医学类", "工业技术", "环境科学类", "历史类", "地理类", "其他" };

QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[0-9]{6,8}"), NULL);


void refreshTables(QSqlQuery query/*数据库查询结果*/, int nAuth/*用户权限*/, QTableWidget* table)
{
    //清空表格
    table->clearContents();
    table->setRowCount(0);

    // 获取查询结果的列数和行数
    int columnCount = query.record().count();
    int rowCount = 0;

    if (AUTH_VISITOR == nAuth)
    {
        // 设置表格的列数
        table->setColumnCount(columnCount);

        // 增加表头
        QStringList headers;
        for (int i = 0; i < columnCount; ++i)
        {
            headers << query.record().fieldName(i);
        }
        table->setHorizontalHeaderLabels(headers);

        // 插入查询结果到表格中
        while (query.next())
        {
            table->insertRow(rowCount);

            // 插入查询结果到表格中的其他列
            for (int i = 0; i < columnCount; ++i)
            {
                if (7 == i || 4 == i)
                {
                    QTableWidgetItem* item = new NewTableWidgetItem(query.value(i).toString());
                    table->setItem(rowCount, i, item);
                }
                else
                {
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(i).toString());
                    table->setItem(rowCount, i, item);
                }
            }
            ++rowCount;
        }
    }
    else
    {
        //判断是书籍表格还是借阅详情表格，用于后面的设置表格排序--代码已经写好了，改起来有点麻烦就先这样笨蛋式的修改
        //思路就是重构了qt默认根据字符串排序的函数，重构为根据数字排序，在是数字的列就add自己重构后的NewTableWidgetItem

        // 设置表格的列数
        table->setColumnCount(columnCount + 1); // 选择列多加一列

        // 增加表头
        QStringList headers;
        headers << "选择"; // 添加选择列
        for (int i = 0; i < columnCount; ++i)
        {
            headers << query.record().fieldName(i);
        }
        table->setHorizontalHeaderLabels(headers);

        // 插入查询结果到表格中
        while (query.next())
        {
            table->insertRow(rowCount);
            // 添加选择列并设置CheckBox对齐方式为居中
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setCheckState(Qt::Unchecked);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            table->setItem(rowCount, 0, item);
            table->item(rowCount, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            // 插入查询结果到表格中的其他列
            for (int i = 0; i < columnCount; ++i)
            {
                QTableWidgetItem* item = new QTableWidgetItem(query.value(i).toString());
                table->setItem(rowCount, i + 1, item);
            }
            ++rowCount;
        }
    }
}

QString judgeAuthority(const int& nAuth)
{
    if (AUTH_MANAGER == nAuth) // 游客没有账号
    {
        return "管理员";
    }
    else if (AUTH_USER == nAuth)
    {
        return "普通用户";
    }
    else if (AUTH_VISITOR == nAuth)
    {
        return "游客";
    }
    else if (AUTH_USER_NOTBORROW == nAuth)
    {
        return "普通用户但是没有借书权限";
    }
}
