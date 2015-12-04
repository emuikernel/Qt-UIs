#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>

namespace Ui {
class frmLogin;
}

/* 说明:操作员登录界面模块头文件
 * 功能:当前操作员登录处理,将当前操作员写入配置信息
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
class frmLogin : public QDialog
{
    Q_OBJECT

public:
    explicit frmLogin(QWidget *parent = 0);
    ~frmLogin();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void buttonClick();
    void on_btnLogin_clicked(); //用户按下登录按钮
    void on_btnClose_clicked(); //关闭界面

    void on_btnKey_clicked();

private:
    Ui::frmLogin *ui;

    QStringList TempUserPwd;    //密码链表
    QStringList TempUserType;   //类型链表

    void InitForm();            //初始化界面
};

#endif // FRMLOGIN_H
