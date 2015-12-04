#ifndef FRMPWD_H
#define FRMPWD_H

#include <QDialog>

namespace Ui {
class frmPwd;
}

/* 说明:密码修改界面模块头文件
 * 功能:修改当前用户密码
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
class frmPwd : public QDialog
{
    Q_OBJECT

public:
    explicit frmPwd(QWidget *parent = 0);
    ~frmPwd();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_btnOk_clicked();    //确定修改密码
    void on_btnClose_clicked(); //关闭界面

private:
    Ui::frmPwd *ui;
};

#endif // FRMPWD_H
