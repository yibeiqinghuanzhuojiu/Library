#include "library.h"
#include "ui_library.h"
#include "connectDatabase.h"
#include "loginDialog.h"
#include "addBookDialog.h"
#include "addmuchbooksdialog.h"
#include "userdetaildialog.h"
#include "changeuserdetaildialog.h"
#include "common.h"
#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QSettings>

using namespace std;


Library::Library(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Library)
    , c_nAuthority(2)
    , c_qsUserID("")
    , c_qsUserPassWord("")
    , c_typeTree(NULL)
    , c_typeRoot(NULL)
    , c_button_login_exit(NULL)
    , c_label_displayInfor(NULL)
{
    ui->setupUi(this);
    initRecord();
    creatUI();
    allConnect();
}

Library::~Library()
{
    delete ui;
}

void Library::allConnect()
{
    //登录
    if (c_button_login_exit)
    {
        QObject::connect(c_button_login_exit, &QPushButton::clicked, this, &Library::slot_login);
    }
    //tabWidget切换事件
    QObject::connect(ui->tab_main, &QTabWidget::tabBarClicked, this, &Library::initBookTable);
    //图书检索界面-检索按钮
    QObject::connect(ui->pushButton_search, &QPushButton::clicked, this, &Library::slot_button_search_clicked);
    //图书检索界面-借阅按钮
    QObject::connect(ui->pushButton_borrow, &QPushButton::clicked, this, &Library::slot_button_borrow_clicked);
    //图书检索界面-只看有余量的书籍
    QObject::connect(ui->checkBox, &QCheckBox::clicked, this, &Library::slot_check_all_clicked);
    //图书检索界面-表格勾选事件
    QObject::connect(ui->tableWidget_searchBooks, &QTableWidget::itemClicked, this, &Library::slot_saveSelectBooks);
    //图书检索界面-刷新
    QObject::connect(ui->Button_refreshSearchBooks, &QPushButton::clicked, this, &Library::initBookTable);
    //图书管理界面-添加按钮
    QObject::connect(ui->Button_addBook, &QPushButton::clicked, this, &Library::slot_addBook);
    //图书管理界面-刷新按钮
    QObject::connect(ui->Button_refreshBookTable, &QPushButton::clicked, this, &Library::slot_BookManageRefresh);
    //图书管理界面-批量导入按钮
    QObject::connect(ui->Button_addmuchBooks, &QPushButton::clicked, this, &Library::slot_addMuchBooks);
    //图书管理界面-删除按钮
    QObject::connect(ui->Button_deletBook, &QPushButton::clicked, this, &Library::slot_deletBooks);
    //图书管理界面-表格勾选事件
    QObject::connect(ui->tableWidget_allBookDetail, &QTableWidget::itemClicked, this, &Library::slot_saveManageBooks);
    //用户管理界面-查询
    QObject::connect(ui->Button_searchUser, &QPushButton::clicked, this, &Library::slot_searchUser);
    //用户管理界面-修改用户信息
    QObject::connect(ui->Button_changeUserdetail, &QPushButton::clicked, this, &Library::slot_changeUserDetail);
    //用户管理界面-删除用户
    QObject::connect(ui->Button_deletUser, &QPushButton::clicked, this, &Library::slot_deletUser);
    //用户管理界面-添加用户
    QObject::connect(ui->Button_addNewUser, &QPushButton::clicked, this, &Library::slot_addUser);
    //用户管理界面-表格勾选事件
    QObject::connect(ui->tableWidget_userDetail, &QTableWidget::itemClicked, this, &Library::slot_saveSelectUsers);
    //用户管理界面-刷新
    QObject::connect(ui->Button_refreshUserTable, &QPushButton::clicked, this, &Library::initBookTable);
    //借阅情况界面-表格勾选事件
    QObject::connect(ui->tableWidget_bookBorrow, &QTableWidget::itemClicked, this, &Library::slot_saveSelectRecords);
    //借阅情况界面-全选
    QObject::connect(ui->Button_selectAll, &QPushButton::clicked, this, &Library::slot_selectAllRecords);
    //借阅情况界面-归还书籍
    QObject::connect(ui->Button_bookReturn, &QPushButton::clicked, this, &Library::slot_returnBooks);
}

void Library::initRecord()
{
    if (g_database.open())
    {
        QVector<QPair<QString, QString>> setRecordIndex;
        QDate currentDate = QDate::currentDate();
        QSqlQuery query(g_database);
        query.exec("SELECT * FROM borrow");
        while (query.next())
        {
            setRecordIndex.append(QPair<QString, QString>(query.value("记录编号").toString(), query.value("归还时间").toString()));
        }
        for (const auto& index : setRecordIndex)
        {
            QString qsIndex = index.first;
            QString qsReturnDate = index.second;
            QDate returnDate = QDate::fromString(qsReturnDate, "yyyy-MM-dd");
            int nDays = currentDate.daysTo(returnDate);
            query.prepare("UPDATE borrow SET 剩余天数 = :day WHERE 记录编号 = :index");
            query.bindValue(":day", nDays);
            query.bindValue(":index", qsIndex);
            query.exec();
        }
    }
}

void Library::creatUI()
{
    creatTypeListUI();
    initBookTable();

    //设置出版时间输入提示
    ui->lineEdit_publishDate->setPlaceholderText("输入格式:yyyy/MM/DD");

    //设置title背景
    QPixmap pixmap(":/source/source/title_final.jpg");
    ui->title_background->setPixmap(pixmap.scaled(ui->title_background->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //设置登录/注销文字及时间显示
    c_button_login_exit = new QPushButton(this);
    c_button_login_exit->setFixedSize(80, 25);
    c_button_login_exit->setText("请登录!");
    c_button_login_exit->setStyleSheet("border:none");
    c_button_login_exit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    c_button_login_exit->move(this->size().width() - 80, ui->title_background->size().height() - 10);

    //显示用户和当前时间
    //QTimer timer;
    c_label_displayInfor = new QLabel(this);
    c_label_displayInfor->setFixedSize(140, 25);
    c_label_displayInfor->setText(QString("<a href=>%1您好!</a>").arg("游客"));
    c_label_displayInfor->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    c_label_displayInfor->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    /*QObject::connect(&timer, &QTimer::timeout, [=, &qsInfor]() {
        // 获取当前时间
        QDateTime currentTime = QDateTime::currentDateTime();
        // 格式化时间为字符串，例如 "yyyy-MM-dd hh:mm:ss"
        qsInfor = qsInfor + currentTime.toString("yyyy-MM-dd hh:mm:ss");
        c_label_displayInfor->setText(qsInfor);
        c_label_displayInfor->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
        });
    timer.start(1000);*/
    c_label_displayInfor->move(this->size().width() - 200, ui->title_background->size().height() - 10);

    ui->lineEdit_searchUserID->setValidator(validator);
    //启用表格排序
    ui->tableWidget_searchBooks->setSortingEnabled(true);
    ui->tableWidget_allBookDetail->setSortingEnabled(true);
    ui->tableWidget_userDetail->setSortingEnabled(true);
    ui->tableWidget_bookBorrow->setSortingEnabled(true);
    showTabByAuthority(c_nAuthority);
}

void Library::creatTypeListUI()
{
    //创建左侧书籍分类树状结构
    QVBoxLayout* layout_typeList = new QVBoxLayout(ui->widget_typeList);
    QTreeWidget* tree_typeList = new QTreeWidget();
    tree_typeList->setHeaderLabels({ "书籍类型" });

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(tree_typeList);
    rootItem->setText(0, "所有图书");
    foreach (const QString & category, categories)
    {
        QTreeWidgetItem* childItem = new QTreeWidgetItem(rootItem);
        childItem->setText(0, category);
    }

    layout_typeList->addWidget(tree_typeList);

    //点击类型后表格书籍筛选
    QObject::connect(tree_typeList, &QTreeWidget::itemClicked, [=](QTreeWidgetItem* item, int column) {
        refreshBookTableByType(item, column);
        });

}

void Library::showTabByAuthority(int auth)
{
    /*
    根据权限不同显示不同的界面,同时还有一些其他的权限控制
    tab页面控制如下:
    管理员：书籍检索(0)、书籍管理（1）、用户管理（2）、借阅情况（3）、其他（4）
    普通用户：书籍检索、借阅情况、其他
    游客：书籍检索、其他
    */
    if (AUTH_MANAGER == auth)
    {
        //全部页面都显示
        ui->tab_main->setTabVisible(0, true);
        ui->tab_main->setTabVisible(1, true);
        ui->tab_main->setTabVisible(2, true);
        ui->tab_main->setTabVisible(3, true);
        ui->tab_main->setTabVisible(4, true);

        ui->pushButton_borrow->setVisible(true);
    }
    else if (AUTH_USER == auth)
    {
        ui->tab_main->setTabVisible(0, true);
        ui->tab_main->setTabVisible(1, false);
        ui->tab_main->setTabVisible(2, false);
        ui->tab_main->setTabVisible(3, true);
        ui->tab_main->setTabVisible(4, true);

        ui->pushButton_borrow->setVisible(true);
    }
    else
    {
        ui->tab_main->setTabVisible(0, true);
        ui->tab_main->setTabVisible(1, false);
        ui->tab_main->setTabVisible(2, false);
        ui->tab_main->setTabVisible(3, false);
        ui->tab_main->setTabVisible(4, true);

        ui->pushButton_borrow->setVisible(false);
    }
}

void Library::initBookTable()
{
    //每次刷新表格的时候都要清楚之前勾选的记录
    c_setSelectSearchBooks.clear();
    c_setSelectManageBooks.clear();
    c_setSelectUsers.clear();
    c_setSelectRecords.clear();

    c_qsBookType = "所有图书";
    if(g_database.open())
    {
        // 执行数据库查询
        QSqlQuery query(g_database);
        if (AUTH_VISITOR == c_nAuthority)
        {
            query.exec("SELECT * FROM book_detail");
        }
        else if (AUTH_USER == c_nAuthority)
        {
            if (0 == ui->tab_main->currentIndex())
            {
                query.exec("SELECT * FROM book_detail");
            }
            else
            {
                query.prepare("SELECT * FROM borrow WHERE 用户ID = :id");
                query.bindValue(":id", c_qsUserID);
                query.exec();
            }
        }
        else if (AUTH_MANAGER == c_nAuthority)
        {
            if (0 == ui->tab_main->currentIndex())
            {
                query.exec("SELECT * FROM book_detail");
            }
            else if (1 == ui->tab_main->currentIndex())
            {
                query.exec("SELECT * FROM user_detail");
            }
            else if (2 == ui->tab_main->currentIndex())
            {
                query.exec("SELECT * FROM book_detail");
            }
            else if (3 == ui->tab_main->currentIndex())
            {
                query.exec("SELECT * FROM borrow");
            }
        }

        refreshBookTable(query);
    }
    else
    {
        QMessageBox::information(NULL, "提示", "databaes is not open!");
        qDebug() << g_database.lastError().text();
    }
}

void Library::refreshBookTable(QSqlQuery query)
{
    if (AUTH_VISITOR == c_nAuthority)
    {
        refreshTables(query, c_nAuthority, ui->tableWidget_searchBooks);
    }
    else if (AUTH_USER == c_nAuthority)
    {
        if (0 == ui->tab_main->currentIndex())
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_searchBooks);
        }
        else
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_bookBorrow);
        }
    }
    else if (AUTH_MANAGER == c_nAuthority)
    {
        if (0 == ui->tab_main->currentIndex())
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_searchBooks);
        }
        else if (1 == ui->tab_main->currentIndex())
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_userDetail);
        }
        else if (2 == ui->tab_main->currentIndex())
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_allBookDetail);
        }
        else if (3 == ui->tab_main->currentIndex())
        {
            refreshTables(query, c_nAuthority, ui->tableWidget_bookBorrow);
        }
    }
}

void Library::refreshBookTableByType(QTreeWidgetItem* item, int column)
{
    if (g_database.open())
    {
        QSqlQuery query(g_database);

        if (item->text(column) == "所有图书")
        {
            c_qsBookType = "所有图书";
            query.exec("SELECT * FROM book_detail");
            refreshBookTable(query);
        }
        else
        {
            c_qsBookType = item->text(column);
            query.prepare("SELECT * FROM book_detail WHERE 书籍类型 = :type");
            query.bindValue(":type", c_qsBookType);

            if (query.exec())
            {
                refreshBookTable(query);
            }
            else
            {
                QMessageBox::information(NULL, "提示", "query database faile!");
            }
        }
        slot_button_search_clicked();
    }
}

void Library::updateBorrowTable(const QString bookID, const QString bookName, const QString bookAuth, const QString userID)
{
    if (g_database.open())
    {

        QSqlQuery query(g_database);
        query.prepare("SELECT * FROM user_detail WHERE (用户ID = :id OR 用户昵称 = :name)");
        query.bindValue(":id", userID);
        query.bindValue(":name", userID);
        if (query.exec())
        {
            while (query.next())
            {
                //可能存在同名用户,因为登录时可以使用昵称登录,不一定使用ID,通过密码区分
                if (c_qsUserPassWord == query.value("用户密码").toString())
                {
                    QString qsUserName = query.value("用户昵称").toString();
                    QString qsuserID = query.value("用户ID").toString();
                    //获取借阅时间,默认借阅阅读时间是30天
                    QDate currentDate = QDate::currentDate();
                    QDate returnDate = currentDate.addDays(30);
                    QString qsCurrentDate = currentDate.toString("yyyy-MM-dd");
                    QString qsReturnDate = returnDate.toString("yyyy-MM-dd");
                    //int nDays = currentDate.daysTo(returnDate);

                    //判断是否是借阅的之前借阅过的书籍,如果是则加一,但是每一本书的借阅时间不一样，暂时先不实现，界面没设计
                    /*bool bHaveRecord = false;
                    int nBookNum = 0;
                    query.prepare("SELECT 1 FROM borrow WHERE 书籍编号 = :bookID AND 用户ID = :userID)");
                    query.bindValue(":bookID", bookID);
                    query.bindValue(":userID", qsuserID);
                    if (query.exec())
                    {
                        nBookNum = query.value("借阅数量").toInt();
                        bHaveRecord = query.value(0).toBool();
                    }*/

                    query.prepare("INSERT INTO borrow (书籍编号, 书名, 作者, 用户ID, 昵称, 借阅数量, 借阅时间, 归还时间, 剩余天数) "
                        "VALUES (:bookID, :bookName, :bookAuth, :userID, :userName, :borrowNum, :borrowDate, :returnDate, :days)");
                    query.bindValue(":bookID", bookID);
                    query.bindValue(":bookName", bookName);
                    query.bindValue(":bookAuth", bookAuth);
                    query.bindValue(":userID", qsuserID);
                    query.bindValue(":userName", qsUserName);
                    query.bindValue(":borrowNum", 1);
                    query.bindValue(":borrowDate", qsCurrentDate);
                    query.bindValue(":returnDate", qsReturnDate);
                    query.bindValue(":days", 30); //首次借阅默认天数就是30
                    if (query.exec())
                    {
                        qDebug() << "Insert borrow record successfully!";
                    }
                    else
                    {
                        qDebug() << "Insert borrow record failed!" << query.lastError().text();
                    }
                }
            }
        }
    }
}

void Library::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    slot_resizeEvent();
}

/*bool Library::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == c_button_login_exit) //指定事件接受者
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            slot_login();
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}*/

void Library::slot_button_search_clicked()
{
    QString qsBookID = ui->lineEdit_bookID->text();
    QString qsBookName = ui->lineEdit_bookName->text();
    QString qsAuthor = ui->lineEdit_anthor->text();
    QString qsPublishDate = ui->lineEdit_publishDate->text();
    QString qsPublisher = ui->lineEdit_publishFirm->text();
    QString price_low = ui->lineEdit_priceLow->text();
    QString price_high = ui->lineEdit_priceHigh->text();

    if (g_database.open())
    {
        //检索书籍-模糊匹配
        QString qsQuery = "SELECT * FROM book_detail WHERE";
        if (c_qsBookType != "所有图书")
        {
            qsQuery += " 书籍类型 LIKE '%" + c_qsBookType + "%' AND";
        }
        if (!qsBookID.isEmpty())
        {
            qsQuery += " 书籍编号 LIKE '%" + qsBookID + "%' AND";
        }
        if (!qsBookName.isEmpty())
        {
            qsQuery += " 书名 LIKE '%" + qsBookName + "%' AND";
        }
        if (!qsAuthor.isEmpty())
        {
            qsQuery += " 作者 LIKE '%" + qsAuthor + "%' AND";
        }
        if (!qsPublishDate.isEmpty())
        {
            qsQuery += " 出版时间 LIKE '%" + qsPublishDate + "%' AND";
        }
        if (!qsPublisher.isEmpty())
        {
            qsQuery += " 出版社 LIKE '%" + qsPublisher + "%' AND";
        }
        if (!price_low.isEmpty())
        {
            qsQuery += " 价格 >= " + price_low + " AND";
        }
        if (!price_high.isEmpty())
        {
            qsQuery += " 价格 <= " + price_high + " AND";
        }
        if (ui->checkBox->isChecked())
        {
            qsQuery += " 图书余量 > 0";
        }

        if (qsQuery.endsWith(" AND"))
        {
            qsQuery.chop(4); // 删除查询语句最后的AND
        }

        if (qsQuery.endsWith(" WHERE"))
        {
            qsQuery.chop(6);
        }

        QSqlQuery query(g_database);
        query.prepare(qsQuery);
        if (query.exec())
        {
            refreshBookTable(query);
        }
    }
    else
    {
        QMessageBox::information(NULL, "提示", "database is break!");
    }

}

void Library::slot_button_borrow_clicked()
{
    if (c_setSelectSearchBooks.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "未选择书籍!");
        return;
    }
    if (c_qsUserID.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "请登陆账号之后再借阅书籍!");
        return;
    }
    QStringList qsSelectBooks;

    for (const QString& selectBook : c_setSelectSearchBooks)
    {
        qsSelectBooks << selectBook;
    }

    if (g_database.open())
    {
        QStringList qsBorrowSuccess;
        QStringList qsBorrowFailed;
        for (const QString& bookID : qsSelectBooks)
        {
            QSqlQuery query(g_database);
            query.prepare("SELECT * FROM book_detail WHERE 书籍编号 = :id");
            query.bindValue(":id", bookID);
            if (query.exec() && query.next())
            {
                int bookNum = query.value("图书余量").toInt();
                QString qsBookName = query.value("书名").toString();
                QString qsBookAuth = query.value("作者").toString();
                if (0 == bookNum)
                {
                    qsBorrowFailed << qsBookName;
                }
                else
                {
                    bookNum -= 1;
                    query.prepare("UPDATE book_detail SET 图书余量 = :num WHERE 书籍编号 = :id");
                    query.bindValue(":num", bookNum);
                    query.bindValue(":id", bookID);
                    if (query.exec())
                    {
                        updateBorrowTable(bookID, qsBookName, qsBookAuth, c_qsUserID);
                        initBookTable();
                        qsBorrowSuccess << qsBookName;
                    }
                    else
                    {
                        QMessageBox::information(NULL, "提示", query.lastError().text());
                        return;
                    }
                }
            }
            else
            {
                QMessageBox::information(NULL, "提示", query.lastError().text());
                return;
            }
        }

        //显示借阅详情提示
        QString qsSuccess = "";
        QString qsFailed = "";
        if (!qsBorrowSuccess.isEmpty())
        {
            for (const QString& success : qsBorrowSuccess)
            {
                qsSuccess += success;
            }
            qsSuccess += "借阅成功!";
        }
        if (!qsBorrowFailed.isEmpty())
        {
            for (const QString& failed : qsBorrowFailed)
            {
                qsFailed += failed;
            }
            qsFailed += "图书余量不足，借阅失败!";
        }

        QMessageBox::information(NULL, "提示", QString("%1 %2").arg(qsSuccess, qsFailed));

        //借阅成功之后清空SET之中保存的信息,并且表格重置为全部未勾选
        c_setSelectSearchBooks.clear();
        for (int i = 0; i < ui->tableWidget_searchBooks->rowCount(); i++)
        {
            ui->tableWidget_searchBooks->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    }
}

void Library::slot_check_all_clicked()
{
    slot_button_search_clicked();
}

void Library::slot_login()
{
    if (AUTH_VISITOR != c_nAuthority)
    {
        c_nAuthority = AUTH_VISITOR;
        c_label_displayInfor->setText(QString("<a href=>%1您好!</a>").arg("游客"));
        c_button_login_exit->setText("请登录!");
        showTabByAuthority(c_nAuthority);
        initBookTable();
    }
    else
    {
        loginDialog dialog_login(this);
        QObject::connect(&dialog_login, &loginDialog::sigLoginSuccess, this, &Library::slot_login_success);
        dialog_login.exec();
    }
}

void Library::slot_login_success(const QString& qsUserID, const QString& qsUserName, const QString& qsPassWord, const QString& qsAuthority)
{
    //登陆后：登录按钮隐藏，注销按钮显示
    c_button_login_exit->setText("注销");
    //设置tab显示
    c_nAuthority = QString(qsAuthority).toInt(); //获取登录用户的权限
    c_qsUserID = qsUserID;
    c_qsUserPassWord = qsPassWord;
    c_label_displayInfor->setText(QString("<a href=>%1%2,您好!</a>").arg(judgeAuthority(c_nAuthority), qsUserName));
    showTabByAuthority(c_nAuthority);
    initBookTable();
}

void Library::slot_saveSelectBooks(QTableWidgetItem* item)
{
    if (item && item->column() == 0)
    {
        int row = item->row();
        if (item->checkState() == Qt::Checked)
        {
            //勾选就添加
            QString bookID = ui->tableWidget_searchBooks->item(row, 1)->text();
            c_setSelectSearchBooks.insert(bookID);
        }
        else
        {
            //取消勾选就移除
            QString bookID = ui->tableWidget_searchBooks->item(row, 1)->text();
            c_setSelectSearchBooks.remove(bookID);
        }
    }
}

void Library::slot_addBook()
{
    addBookDialog dialog_addbook(this);
    dialog_addbook.exec();
}

void Library::slot_BookManageRefresh()
{
    initBookTable();
}

void Library::slot_addMuchBooks()
{
    addMuchBooksDialog dialog_addMuchBooks(this);
    dialog_addMuchBooks.exec();
}

void Library::slot_deletBooks()
{
    if (c_setSelectManageBooks.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "未选择书籍!");
        return;
    }
    QMessageBox myBox;
    myBox.setText("警告!");
    myBox.setInformativeText("确认删除所有选中的图书吗？此行为不可撤销!");
    myBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    myBox.setDefaultButton(QMessageBox::Ok);
    int ret = myBox.exec();
    if (ret == QMessageBox::Ok)
    {
        if (g_database.open())
        {
            for (const QString& bookID : c_setSelectManageBooks)
            {
                QSqlQuery query(g_database);
                query.prepare("DELETE FROM book_detail WHERE 书籍编号 = :id");
                query.bindValue(":id", bookID);
                query.exec();
            }
            QMessageBox::information(NULL, "提示", "删除成功!");
            c_setSelectManageBooks.clear();
            initBookTable();
        }
    }
}

void Library::slot_saveManageBooks(QTableWidgetItem* item)
{
    if (item && item->column() == 0)
    {
        int row = item->row();
        if (item->checkState() == Qt::Checked)
        {
            //勾选就添加
            QString bookID = ui->tableWidget_allBookDetail->item(row, 1)->text();
            c_setSelectManageBooks.insert(bookID);
        }
        else
        {
            //取消勾选就移除
            QString bookID = ui->tableWidget_allBookDetail->item(row, 1)->text();
            c_setSelectManageBooks.remove(bookID);
        }
    }
}

void Library::slot_searchUser()
{
    QString qsUserID = ui->lineEdit_searchUserID->text();
    if (qsUserID.isEmpty())
    {
        return;
    }
    else
    {
        if (g_database.open())
        {
            QSqlQuery query(g_database);
            query.prepare("SELECT * FROM user_detail WHERE 用户ID = :id");
            query.bindValue(":id", qsUserID);
            if (query.exec() && query.next())
            {
                userDetailDialog dialog_userDetail(this);
                dialog_userDetail.initUI(qsUserID);
                dialog_userDetail.exec();
            }
            else
            {
                QMessageBox::information(NULL, "提示", "该用户不存在!");
            }
        }
    }
}

void Library::slot_changeUserDetail()
{
    if (0 == c_setSelectUsers.size())
    {
        QMessageBox::information(NULL, "提示", "请勾选需要修改的用户!");
    }
    else if (1 < c_setSelectUsers.size())
    {
        QMessageBox::information(NULL, "提示", "一次只能修改一个用户!");
    }
    else
    {
        if (g_database.open())
        {
            for (const QString& userID : c_setSelectUsers)
            {
                QSqlQuery query(g_database);
                query.prepare("SELECT * FROM user_detail WHERE 用户ID = :id");
                query.bindValue(":id", userID);
                if (query.exec() && query.next())
                {
                    QString qsName = query.value("用户昵称").toString();
                    QString qsPassword = query.value("用户密码").toString();
                    int nAuth = query.value("权限").toInt();
                    QString qsAuth = judgeAuthority(nAuth);
                    changeUserDetailDialog dialog_changeUser(this);
                    dialog_changeUser.initInfor(userID, qsName, qsPassword, qsAuth);
                    dialog_changeUser.exec();
                }
            }
        }
    }
}

void Library::slot_deletUser()
{
    if (c_setSelectUsers.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "未选择用户!");
        return;
    }
    QMessageBox myBox;
    myBox.setText("警告!");
    myBox.setInformativeText("确认删除所有选中的用户吗？此行为不可撤销!");
    myBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    myBox.setDefaultButton(QMessageBox::Ok);
    int ret = myBox.exec();
    if (ret == QMessageBox::Ok)
    {
        if (g_database.open())
        {
            for (const QString& userID : c_setSelectUsers)
            {
                QSqlQuery query(g_database);
                query.prepare("DELETE FROM user_detail WHERE 用户ID = :id");
                query.bindValue(":id", userID);
                query.exec();
            }
            QMessageBox::information(NULL, "提示", "删除成功!");
            c_setSelectUsers.clear();
            initBookTable();
        }
    }
}

void Library::slot_addUser()
{
    changeUserDetailDialog dialog_changeUser(this);
    dialog_changeUser.setUI("添加用户", "添加用户");
    dialog_changeUser.exec();
}

void Library::slot_saveSelectUsers(QTableWidgetItem* item)
{
    if (item && item->column() == 0)
    {
        int row = item->row();
        QString userID;
        if (item->checkState() == Qt::Checked)
        {
            //勾选就添加
            userID = ui->tableWidget_userDetail->item(row, 1)->text();
            c_setSelectUsers.insert(userID);
        }
        else
        {
            //取消勾选就移除
            userID = ui->tableWidget_userDetail->item(row, 1)->text();
            c_setSelectUsers.remove(userID);
        }
    }
}

void Library::slot_saveSelectRecords(QTableWidgetItem* item)
{
    //全选之后取消部分勾选按钮文字改变
    ui->Button_selectAll->setText("全选");
    if (item && item->column() == 0)
    {
        int row = item->row();
        QString index;
        if (item->checkState() == Qt::Checked)
        {
            //勾选就添加
            index = ui->tableWidget_bookBorrow->item(row, 1)->text();
            c_setSelectRecords.insert(index);
        }
        else
        {
            //取消勾选就移除
            index = ui->tableWidget_bookBorrow->item(row, 1)->text();
            c_setSelectRecords.remove(index);
        }
    }
}

void Library::slot_selectAllRecords()
{
    if ("全选" == ui->Button_selectAll->text())
    {
        c_setSelectRecords.clear();
        ui->Button_selectAll->setText("取消全选");
        for (int i = 0; i < ui->tableWidget_bookBorrow->rowCount(); i++)
        {
            ui->tableWidget_bookBorrow->item(i, 0)->setCheckState(Qt::Checked);
        }

        if (g_database.open())
        {
            QSqlQuery query(g_database);
            if (AUTH_MANAGER == c_nAuthority)
            {
                query.exec("SELECT * FROM borrow");
                while (query.next()) {
                    c_setSelectRecords.insert(query.value(0).toString());
                }
            }
            else
            {
                query.prepare("SELECT * FROM borrow WHERE 用户ID = :id");
                query.bindValue(":id", c_qsUserID);
                query.exec();
                while (query.next()) {
                    c_setSelectRecords.insert(query.value(0).toString());
                }
            }
        }
    }
    else
    {
        c_setSelectRecords.clear();
        ui->Button_selectAll->setText("全选");
        for (int i = 0; i < ui->tableWidget_bookBorrow->rowCount(); i++)
        {
            ui->tableWidget_bookBorrow->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    }
}

void Library::slot_returnBooks()
{
    if (g_database.open())
    {
        bool bReturn = false;
        if (c_setSelectRecords.isEmpty())
        {
            QMessageBox::information(NULL, "提示", "未选择要归还的记录!");
            return;
        }
        else
        {
            QSqlQuery query(g_database);
            for (const QString& index : c_setSelectRecords)
            {
                query.prepare("SELECT * FROM borrow WHERE 记录编号 = :index");
                query.bindValue(":index", index);
                if (query.exec() && query.next())
                {
                    QString qsBookID = query.value(1).toString();
                    query.prepare("SELECT * FROM book_detail WHERE 书籍编号 = :bookId");
                    query.bindValue(":bookId", qsBookID);
                    if (query.exec() && query.next())
                    {
                        int nBookNum = query.value("图书余量").toInt();
                        query.prepare("UPDATE book_detail SET 图书余量 = :num WHERE 书籍编号 = :bookID");
                        query.bindValue(":num", nBookNum + 1);
                        query.bindValue(":bookID", qsBookID);
                        query.exec();
                    }
                    else
                    {
                        QMessageBox::information(NULL, "提示", query.lastError().text());
                    }
                    query.prepare("DELETE FROM borrow WHERE 记录编号 = :index");
                    query.bindValue(":index", index);
                    if (query.exec())
                    {
                        bReturn = true;
                    }
                    else
                    {
                        QMessageBox::information(NULL, "提示", query.lastError().text());
                    }
                }
                else
                {
                    QMessageBox::information(NULL, "提示", query.lastError().text());
                }
            }
        }
        if (bReturn)
        {
            QMessageBox::information(NULL, "提示", "归还成功!");
            //全选且归还之后按钮文字改变
            ui->Button_selectAll->setText("全选");
            initBookTable();
        }
    }
}

void Library::slot_resizeEvent()
{
    //刷新title背景大小
    //可能有性能问题-后续在解决
    QPixmap pixmap(":/source/source/title_final.jpg");
    ui->title_background->setPixmap(pixmap.scaled(ui->title_background->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //刷新登录/注销按钮位置
    c_button_login_exit->move(this->size().width() - 80, ui->title_background->size().height() - 10);
    c_label_displayInfor->move(this->size().width() - 200, ui->title_background->size().height() - 10);

}
