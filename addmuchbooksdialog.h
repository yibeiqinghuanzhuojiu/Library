#ifndef ADDMUCHBOOKSDIALOG_H
#define ADDMUCHBOOKSDIALOG_H

#include <QDialog>
#include <bookDef.h>

namespace Ui {
class addMuchBooksDialog;
}

class addMuchBooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addMuchBooksDialog(QWidget *parent = nullptr);
    ~addMuchBooksDialog();

    void creatUI();
    void allConnects();

private slots:
    void slot_selectFile();
    void slot_affirmAdd();

private:
    Ui::addMuchBooksDialog *ui;
    QSet<QStringList>      c_setBooks; // 保存从文件读取的书籍信息
};

#endif // ADDMUCHBOOKSDIALOG_H
