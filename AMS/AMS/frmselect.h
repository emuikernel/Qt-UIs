#ifndef FRMSELECT_H
#define FRMSELECT_H

#include <QDialog>

namespace Ui {
class frmSelect;
}

/* 说明:日期选择界面模块头文件
 * 功能:提供开始时间和结束时间供用户选择
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
class frmSelect : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmSelect(QWidget *parent = 0);
    ~frmSelect();

    //获取当前选择的开始时间和结束时间
    QString SelectStartDate() const {return selectStartDate;}
    QString SelectEndDate() const {return selectEndDate;}
    
private slots:
    void on_btnOk_clicked();        //按下确定按钮
    void on_btnClose_clicked();     //按下关闭按钮

private:
    Ui::frmSelect *ui;

    QString selectStartDate;        //开始时间
    QString selectEndDate;          //结束时间
};

#endif // FRMSELECT_H
