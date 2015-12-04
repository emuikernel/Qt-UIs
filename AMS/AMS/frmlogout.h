#ifndef FRMLOGOUT_H
#define FRMLOGOUT_H

#include <QDialog>

namespace Ui {
class frmLogout;
}

/* 说明:操作员退出界面模块头文件
 * 功能:当前操作员退出处理
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
class frmLogout : public QDialog
{
    Q_OBJECT

public:
    explicit frmLogout(QWidget *parent = 0);
    ~frmLogout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void buttonClick();
    void on_btnLogout_clicked();//用户按下退出按钮
    void on_btnClose_clicked();//关闭界面

    void on_btnKey_clicked();

private:
    Ui::frmLogout *ui;
};

#endif // FRMLOGOUT_H
