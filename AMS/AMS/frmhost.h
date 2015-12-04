#ifndef FRMHOST_H
#define FRMHOST_H

#include <QDialog>

namespace Ui {
class frmHost;
}

/* 说明:主机管理界面模块头文件
 * 功能:添加删除修改主机等,多种主机切换
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class frmHost : public QDialog
{
    Q_OBJECT

public:
    explicit frmHost(QWidget *parent = 0);
    ~frmHost();

protected slots:
    void closeEvent(QCloseEvent *);                     //重写关闭事件

private slots:
    void on_txtHostType_currentIndexChanged(int index); //主机类型下拉选择
    void on_listHost_currentRowChanged(int currentRow); //主机移动
    void on_btnAdd_clicked();                           //添加主机
    void on_btnDelete_clicked();                        //删除主机
    void on_btnUpdate_clicked();                        //修改主机

private:
    Ui::frmHost *ui;

    void InitForm();            //初始化界面
    QStringList TempHostInfo;   //主机信息链表
    void ReadHostInfo();        //加载主机信息

    //校验主机信息
    bool CheckHostInfo(QString HostType,QString HostID,QString HostName,
                       QString HostConnectValue1,QString HostConnectValue2);
};

#endif // FRMHOST_H
