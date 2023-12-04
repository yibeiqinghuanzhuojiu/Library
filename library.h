#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSqlQuery>
#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class Library; }
QT_END_NAMESPACE

class Library : public QMainWindow
{
    Q_OBJECT

public:
    Library(QWidget *parent = nullptr);
    ~Library();

    /*所有的connect*/
    void allConnect();

    /*界面生成*/
    void initRecord();                                                  //初始化记录：借阅剩余天数等
    void creatUI();
    void creatTypeListUI();
    void showTabByAuthority(int auth);                                  //根据不同的权限显示tab

    /*书籍检索界面函数*/
    void initBookTable();                                               //初始化数据库所有书籍到表格
    void refreshBookTable(QSqlQuery query);                             //条件改动就刷新表格
    void refreshBookTableByType(QTreeWidgetItem* item, int column);     //通过type分类书籍
    void updateBorrowTable(const QString bookID, const QString bookName, const QString bookAuth, const QString userID); //更新MySQL借阅表格

    /*用户管理界面函数*/
    void changeUserDetail(const QString& userID);                       //修改用户信息
    void deletUser(const QString& userID);                              //删除用户

    /*窗口大小检测*/
    void resizeEvent(QResizeEvent* event);

    /*鼠标点击事件，实现登录/注销文字点击*/
    //bool eventFilter(QObject* obj, QEvent* event);

signals:
    void sigSearchUser(const QString& userID);

private slots:
    /*检索界面*/
    void slot_button_search_clicked();         //检索按钮点击事件
    void slot_button_borrow_clicked();         //借阅按钮点击事件
    void slot_check_all_clicked();             //只看有存量的书勾选事件
    void slot_login();                         //登录
    void slot_login_success(const QString& qsUserID, const QString& qsUserName, const QString& qsPassWord, const QString& qsAuthority);        //登录成功
    void slot_saveSelectBooks(QTableWidgetItem* item);   //保存勾选书籍用于借阅
    /*图书管理界面*/
    void slot_addBook();                       //添加或修改单本图书
    void slot_BookManageRefresh();             //刷新按钮
    void slot_addMuchBooks();                  //批量导入书籍
    void slot_deletBooks();                    //删除书籍
    void slot_saveManageBooks(QTableWidgetItem* item); //保存图书管理界面勾选的图书
    /*用户管理界面*/
    void slot_searchUser();                    //查询用户
    void slot_changeUserDetail();              //修改用户信息
    void slot_deletUser();                     //删除用户
    void slot_addUser();                       //添加用户
    void slot_saveSelectUsers(QTableWidgetItem* item);   //保存勾选的用户
    /*借阅情况界面*/
    void slot_saveSelectRecords(QTableWidgetItem* item);   //保存勾选的借阅记录
    void slot_selectAllRecords();              //表格全选
    void slot_returnBooks();                   //书籍归还

    void slot_resizeEvent();

public:
    Ui::Library             *ui;
    QString                 c_qsBookType;               //在条件检索时保留左侧对书籍的类型检索
    //bool                    c_bCheckVol;              //是否勾选仅看有余量的书籍
    int                     c_nAuthority;               //当前使用用户的权限，默认游客权限：2
    QString                 c_qsUserID;                 //保存当前用户ID
    QString                 c_qsUserPassWord;           //保存当前用户密码
    QSet<QString>           c_setSelectSearchBooks;     //勾选检索的书籍用于借阅:保存书籍编号
    QSet<QString>           c_setSelectManageBooks;     //勾选图书管理界面用于删除等
    QSet<QString>           c_setSelectUsers;           //勾选用户用于删除,修改等
    QSet<QString>           c_setSelectRecords;         //勾选借阅记录

    QTreeWidget             *c_typeTree;
    QTreeWidgetItem         *c_typeRoot;
    QPushButton             *c_button_login_exit;         //登录/注销
    QLabel                  *c_label_displayInfor;  //用户信息显示
};
#endif // LIBRARY_H
