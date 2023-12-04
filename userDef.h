#ifndef USERDEF_H
#define USERDEF_H

#include<string>
#include<vector>
using namespace std;

/*
用户类型的定义文件

*/
class User {
public:
    string id;
    string name;
    string password;
    //bool priv;
    int booknum;        //借阅数量
    int nAuth;          //权限
};
class UserQuery {
public:
    string id;
    string name;
    string password;
    std::vector<User> Users;
};

/*class Manager {
public:
    string id;
    string name;
    string password;
};*/


#endif // USERDEF_H
