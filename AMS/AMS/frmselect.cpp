#include "frmselect.h"
#include "ui_frmselect.h"
#include "api/myhelper.h"
#include "api/myapp.h"

/* 说明:日期选择界面模块实现文件
 * 功能:提供开始时间和结束时间供用户选择
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
frmSelect::frmSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSelect)
{
    ui->setupUi(this);

    myHelper::FormInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    myHelper::FormNotResize(this);    

    //重置日期框为当前日期
    ui->dateStart->setDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate());
}

frmSelect::~frmSelect()
{
    delete ui;
}

void frmSelect::on_btnOk_clicked()
{
    if (ui->dateStart->date()>ui->dateEnd->date()) {
        myHelper::ShowMessageBoxError("开始时间不能大于结束时间!");
        return;
    }

    this->selectStartDate=ui->dateStart->date().toString("yyyy-MM-dd");
    this->selectEndDate=ui->dateEnd->date().toString("yyyy-MM-dd");
    done(1);//关闭界面返回
    this->close();
}

void frmSelect::on_btnClose_clicked()
{
    done(0);
    this->close();
}
