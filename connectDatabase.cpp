#include "connectDatabase.h"
#include <QDebug>
#include <QSqlError>
#include <qmessagebox.h>


QSqlDatabase g_database = QSqlDatabase::addDatabase("QMYSQL");

void databaseConnect()
{
    g_database.setHostName("127.0.0.1");
    g_database.setDatabaseName("mysql");
    g_database.setUserName("root");
    g_database.setPassword("1998811awen");

    // 尝试连接数据库
    if (g_database.open()) {
        QMessageBox::information(NULL, "infor", "link success");
    } else {
        QMessageBox::information(NULL, "infor", "link failed");
        qDebug() << g_database.lastError().text();
    }
}
