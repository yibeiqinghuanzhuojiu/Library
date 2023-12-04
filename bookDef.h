#ifndef BOOKDEF_H
#define BOOKDEF_H

#include <string>
#include <vector>
#include <QString>

/*
书籍类型的定义文件

*/
class Book
{
public:
    QString book_id;
    QString name;
    QString author;
    QString type;               //书籍类型
    double price;
    QString publisher;          //出版社
    QString publishdate;        //发布日期
    int num;                    //书的总量
    QString other;              //其他
};


class BookQuery
{
public:
    QString book_id;
    QString name;
    QString author;
    QString type;
    int begin_year;
    int end_year;
    QString publisher;
    double low_price;
    double high_price;
    bool have_stock;
    std::vector<Book> Catalog;
};

class Borrow
{
public:
    QString book_id;
    QString book_name;
    QString book_anthor;
    QString book_num;           //借阅数量
    QString user_name;
    QString user_id;
    QString borrow_date;        //借阅时间
    QString return_date;        //归还时间
    int days;
};


#endif // BOOKDEF_H
