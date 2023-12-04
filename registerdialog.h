#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    registerDialog(QWidget* parent = nullptr);
    ~registerDialog();

    void creatUI();
    void allConnects();

private slots:
    void slot_buttonclicked();

private:
    Ui::registerDialog *ui;
};

#endif // REGISTERDIALOG_H
