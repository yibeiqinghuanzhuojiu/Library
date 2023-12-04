#include "library.h"
#include "connectDatabase.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //连接MySQL数据库
    databaseConnect();

    Library w;
    w.show();
    /*if (w.close())
    {
        g_database.close();
    }*/
    return a.exec();
}
