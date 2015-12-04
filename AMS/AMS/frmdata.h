#ifndef FRMDATA_H
#define FRMDATA_H

#include <QDialog>
#include "api/mysqliteapi.h"

namespace Ui {
class frmData;
}

/* 说明:警情查询界面模块头文件
 * 功能:多条件查询警情,可导出和打印警情
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class frmData : public QDialog
{
    Q_OBJECT

public:
    explicit frmData(QWidget *parent = 0);
    ~frmData();

private slots:
    void on_ckTriggerTime_toggled(bool checked);    //时间切换
    void on_ckTriggerContent_toggled(bool checked); //内容切换
    void on_ckTriggerType_toggled(bool checked);    //类型切换
    void on_ckDefenceName_toggled(bool checked);    //防区名切换
    void on_ckUserName_toggled(bool checked);       //值班员切换
    void on_btnSelect_clicked();                    //警情查询
    void on_btnDelete_clicked();                    //删除警情
    void on_btnPrint_clicked();                     //打印警情
    void on_btnExcel_clicked();                     //导出警情到excel

private:
    Ui::frmData *ui;
    mySqliteAPI *Sqlite;        //数据库操作对象

    void InitForm();            //初始化界面

    QString columnNames[9];     //列名数组
    int columnWidths[9];        //列宽数组
};

#endif // FRMDATA_H
