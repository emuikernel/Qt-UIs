#include "frmpwd.h"
#include "ui_frmpwd.h"
#include "api/myhelper.h"
#include "api/myapp.h"
#include "api/myapi.h"

/* 说明:密码修改界面模块实现文件
 * 功能:修改当前用户密码
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
frmPwd::frmPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPwd)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
}

frmPwd::~frmPwd()
{
    delete ui;
}

bool frmPwd::eventFilter(QObject *obj, QEvent *event)
{
    //如果用户按下了回车键,则触发修改密码信号
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
        if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter) {
            this->on_btnOk_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmPwd::on_btnOk_clicked()
{
    QString PwdOld=ui->txtPwdOld->text();
    QString PwdNew=ui->txtPwdNew->text();
    QString PwdNewX=ui->txtPwdNewX->text();

    if (PwdOld!=myApp::CurrentUserPwd){
        myHelper::ShowMessageBoxError("原密码错误,请重新输入!");
        ui->txtPwdOld->setFocus();
        return;
    }

    if (PwdNew==""){
        myHelper::ShowMessageBoxError("新密码不能为空,请重新输入!");
        ui->txtPwdNew->setFocus();
        return;
    }

    if (PwdNewX==""){
        myHelper::ShowMessageBoxError("确认密码不能为空,请重新输入!");
        ui->txtPwdNewX->setFocus();
        return;
    }

    if (PwdNew!=PwdNewX){
        myHelper::ShowMessageBoxError("新密码和确认密码不一致,请重新输入!");
        ui->txtPwdNew->setFocus();
        return;
    }

    QSqlQuery query;
    QString sql=QString("update [UserInfo] set [UserPwd]='%1' where [UserName]='%2'")
            .arg(PwdNewX).arg(myApp::CurrentUserName);
    query.exec(sql);

    //重置当前用户新密码,用于退出判断
    myApp::CurrentUserPwd=PwdNewX;
    QString TempMsg="密码修改成功";   
    myAPI::AddEventInfoUser(TempMsg);
    myHelper::ShowMessageBoxInfo(QString("%1,请牢记新密码【%2】!").arg(TempMsg).arg(PwdNewX));
    this->close();
}

void frmPwd::on_btnClose_clicked()
{
    this->close();
}
