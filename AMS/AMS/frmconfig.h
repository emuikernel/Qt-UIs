#ifndef FRMCONFIG_H
#define FRMCONFIG_H

#include <QDialog>

namespace Ui {
class frmConfig;
}

/* 说明:系统设置界面模块头文件
 * 功能:读取配置文件,设置配置文件,应用配置文件
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class frmConfig : public QDialog
{
    Q_OBJECT

public:
    explicit frmConfig(QWidget *parent = 0);
    ~frmConfig();

private slots:
    void on_btnjiben_clicked();         //基本设置按钮单击信号
    void on_btnzhuanfa_clicked();       //转发设置按钮单击信号
    void on_btnuser_clicked();          //操作员设置按钮单击信号
    void on_btnqita_clicked();          //其他设置按钮单击信号    
    void on_btnOk_clicked();            //应用配置文件
    void on_btnClose_clicked();         //关闭界面
    void on_btnSetTime_clicked();       //修改系统时间
    void on_btnBank_clicked();          //备份数据库
    void on_btnSetDefault_clicked();    //恢复出厂值    
    void on_btnAdd_clicked();           //添加操作员
    void on_btnDelete_clicked();        //删除操作员
    void on_btnUpdate_clicked();        //修改操作员
    void on_btnPwd_clicked();           //修改密码
    void on_listUser_currentRowChanged(int currentRow);//操作员切换

private:
    Ui::frmConfig *ui;

    void InitForm();            //初始化界面

    QStringList TempUserName;   //操作员姓名链表
    QStringList TempUserInfo;   //操作员信息链表
    void ReadUserInfo();        //加载操作员信息

    bool CheckAdmin(QString UserName);                      //校验当前是否是管理员
    bool IsExistUser(QString UserName);                     //校验操作员是否存在
    bool CheckUserInfo(QString UserName,QString UserPwd);   //校验输入框是否为空

};

#endif // FRMCONFIG_H
