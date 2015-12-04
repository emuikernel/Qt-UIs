#include "frmconfig.h"
#include "ui_frmconfig.h"
#include "api/myhelper.h"
#include "api/myapi.h"
#include "api/myapp.h"
#include "frmpwd.h"

/* 说明:系统设置界面模块实现文件
 * 功能:读取配置文件,设置配置文件,应用配置文件
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
frmConfig::frmConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmConfig)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    this->InitForm();
    this->ReadUserInfo();
    this->on_btnjiben_clicked();
}

frmConfig::~frmConfig()
{
    delete ui;
}

void frmConfig::InitForm()
{    
    //加载配置文件对应界面展示信息
    ui->txtSoftTitle->setText(myApp::SoftTitle);
    ui->txtSoftVersion->setText(myApp::SoftVersion);
    ui->txtCompanyName->setText(myApp::CompanyName);
    ui->txtContactName->setText(myApp::ContactName);
    ui->txtContactTel->setText(myApp::ContactTel);

    ui->btnUseServer->SetCheck(myApp::UseServer);
    ui->txtServerPort->setText(QString::number(myApp::ServerPort));
    ui->btnUseSound->SetCheck(myApp::UseSound);    
    ui->btnAwalysPlay->SetCheck(myApp::AwalysPlay);

    ui->btnAutoRun->SetCheck(myApp::AutoRun);
    ui->btnAutoLogin->SetCheck(myApp::AutoLogin);
    ui->btnAutoBank->SetCheck(myApp::AutoBank);
    bool BankYear=myApp::TimeBank=="year"?true:false;
    ui->rbtnYear->setChecked(BankYear);
    ui->rbtnMonth->setChecked(!BankYear);

    for (int i=1;i<20;i++){
        ui->cboxCOMMap->addItem(QString("COM%1").arg(i));
        ui->cboxCOMMsg->addItem(QString("COM%1").arg(i));
    }
    ui->btnLinkMap->SetCheck(myApp::UseLinkMap);
    ui->cboxCOMMap->setCurrentIndex(ui->cboxCOMMap->findText(myApp::LinkMapCOM));
    ui->btnLinkMsg->SetCheck(myApp::UseLinkMap);
    ui->cboxCOMMsg->setCurrentIndex(ui->cboxCOMMsg->findText(myApp::LinkMsgCOM));
    ui->txtMsgTel->setText(myApp::MsgTel);
    ui->txtMsgTemplet->setText(myApp::MsgTemplet);

    ui->btnLinkNETSend->SetCheck(myApp::UseLinkNETSend);
    QStringList addr1=myApp::LinkNETSendAddr1.split("|");
    ui->ckNETAddr1->setChecked(addr1[0]=="true"?true:false);
    ui->txtNETAddr1->setText(addr1[1]);
    ui->txtNETPort1->setText(addr1[2]);
    QStringList addr2=myApp::LinkNETSendAddr2.split("|");
    ui->ckNETAddr2->setChecked(addr2[0]=="true"?true:false);
    ui->txtNETAddr2->setText(addr2[1]);
    ui->txtNETPort2->setText(addr2[2]);
    QStringList addr3=myApp::LinkNETSendAddr3.split("|");
    ui->ckNETAddr3->setChecked(addr3[0]=="true"?true:false);
    ui->txtNETAddr3->setText(addr3[1]);
    ui->txtNETPort3->setText(addr3[2]);
    ui->btnLinkNETRecive->SetCheck(myApp::UseLinkNETReceive);
    ui->txtLinkNETReceivePort->setText(QString::number(myApp::LinkNETReceivePort));

    ui->btnUsePopup->SetCheck(myApp::UsePopup);
    ui->btnUseEmail->SetCheck(myApp::UseEmail);
    ui->txtEmailAddr->setText(myApp::EmailAddr);
    ui->txtEmailPwd->setText(myApp::EmailPwd);
    ui->txtEmailSend->setText(myApp::EmailSend);

    //加载年月日时分秒下拉框
    for (int i=2012;i<=2030;i++){
        ui->cboxYear->addItem(QString::number(i));
    }

    for (int i=1;i<=12;i++){
        ui->cboxMonth->addItem(QString::number(i));
    }

    for (int i=1;i<=31;i++){
        ui->cboxDay->addItem(QString::number(i));
    }

    for (int i=0;i<24;i++){
        ui->cboxHour->addItem(QString::number(i));
    }

    for (int i=0;i<60;i++){
        ui->cboxMin->addItem(QString::number(i));
        ui->cboxSec->addItem(QString::number(i));
    }

    //加载当前时间
    QStringList Now=QDateTime::currentDateTime().toString("yyyy-M-d-h-m-s").split("-");

    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(Now[0]));
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(Now[1]));
    ui->cboxDay->setCurrentIndex(ui->cboxDay->findText(Now[2]));
    ui->cboxHour->setCurrentIndex(ui->cboxHour->findText(Now[3]));
    ui->cboxMin->setCurrentIndex(ui->cboxMin->findText(Now[4]));
    ui->cboxSec->setCurrentIndex(ui->cboxSec->findText(Now[5]));

    //如果当前不是管理员,则限制很多操作
    if (myApp::CurrentUserType!="管理员"){
        ui->btnSetDefault->setEnabled(false);
        ui->btnAdd->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnOk->setEnabled(false);
    }
}

void frmConfig::on_btnjiben_clicked()
{
    ui->btnjiben->setText("【基本设置】→");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("操作员设置");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(true);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnzhuanfa_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("【转发设置】→");
    ui->btnuser->setText("操作员设置");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(true);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnuser_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("【操作员设置】→");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(true);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnqita_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("操作员设置");
    ui->btnqita->setText("【其他设置】→");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(true);
}

void frmConfig::on_btnOk_clicked()
{
    //保存配置文件信息
    myApp::SoftTitle=ui->txtSoftTitle->text();
    myApp::SoftVersion=ui->txtSoftVersion->text();
    myApp::CompanyName=ui->txtCompanyName->text();
    myApp::ContactName=ui->txtContactName->text();
    myApp::ContactTel=ui->txtContactTel->text();
    myApp::LastLoginer=myApp::CurrentUserName;

    myApp::UseServer=ui->btnUseServer->GetCheck();
    myApp::ServerPort=ui->txtServerPort->text().toInt();
    myApp::UseSound=ui->btnUseSound->GetCheck();    
    myApp::AwalysPlay=ui->btnAwalysPlay->GetCheck();

    myApp::AutoRun=ui->btnAutoRun->GetCheck();
    myApp::AutoLogin=ui->btnAutoLogin->GetCheck();
    myApp::AutoBank=ui->btnAutoBank->GetCheck();
    myApp::TimeBank=(ui->rbtnYear->isChecked()?"year":"month");

    myApp::UseLinkMap=ui->btnLinkMap->GetCheck();
    myApp::LinkMapCOM=ui->cboxCOMMap->currentText();
    myApp::UseLinkMsg=ui->btnLinkMsg->GetCheck();
    myApp::LinkMsgCOM=ui->cboxCOMMsg->currentText();
    myApp::MsgTel=ui->txtMsgTel->text();
    myApp::MsgTemplet=ui->txtMsgTemplet->text();

    myApp::UseLinkNETSend=ui->btnLinkNETSend->GetCheck();

    QString IP1=ui->txtNETAddr1->text();
    if (!myHelper::IsIP(IP1)){
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入!");
        ui->txtNETAddr1->setFocus();
        return;
    }

    QString addr1=QString("%1|%2|%3")
            .arg(ui->ckNETAddr1->isChecked()?"true":"false")
            .arg(IP1)
            .arg(ui->txtNETPort1->text());
    myApp::LinkNETSendAddr1=addr1;

    QString IP2=ui->txtNETAddr2->text();
    if (!myHelper::IsIP(IP2)){
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入!");
        ui->txtNETAddr2->setFocus();
        return;
    }

    QString addr2=QString("%1|%2|%3")
            .arg(ui->ckNETAddr2->isChecked()?"true":"false")
            .arg(IP2)
            .arg(ui->txtNETPort2->text());
    myApp::LinkNETSendAddr2=addr2;

    QString IP3=ui->txtNETAddr3->text();
    if (!myHelper::IsIP(IP3)){
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入!");
        ui->txtNETAddr3->setFocus();
        return;
    }

    QString addr3=QString("%1|%2|%3")
            .arg(ui->ckNETAddr3->isChecked()?"true":"false")
            .arg(IP3)
            .arg(ui->txtNETPort3->text());
    myApp::LinkNETSendAddr3=addr3;

    myApp::UseLinkNETReceive=ui->btnLinkNETRecive->GetCheck();
    myApp::LinkNETReceivePort=ui->txtLinkNETReceivePort->text().toInt();

    myApp::UsePopup=ui->btnUsePopup->GetCheck();
    myApp::UseEmail=ui->btnUseEmail->GetCheck();
    myApp::EmailAddr=ui->txtEmailAddr->text();
    myApp::EmailPwd=ui->txtEmailPwd->text();
    myApp::EmailSend=ui->txtEmailSend->text();

    //调用保存配置文件函数
    myApp::WriteConfig();
    done(1);//关闭界面返回
    this->close();
}

void frmConfig::on_btnClose_clicked()
{
    done(0);//关闭界面返回
    this->close();
}

void frmConfig::on_btnSetTime_clicked()
{
    if (myHelper::ShowMessageBoxQuesion("确定要修改系统时间吗?")==0){
        int year=ui->cboxYear->currentText().toInt();
        int month=ui->cboxMonth->currentText().toInt();
        int day=ui->cboxDay->currentText().toInt();
        int hour=ui->cboxHour->currentText().toInt();
        int min=ui->cboxMin->currentText().toInt();
        int sec=ui->cboxSec->currentText().toInt();
        myHelper::SetSystemDateTime(year,month,day,hour,min,sec);

        QString TempMsg="修改系统时间成功";
        myAPI::AddEventInfoUser(TempMsg);
        myHelper::ShowMessageBoxInfo(QString("%1!").arg(TempMsg));
    }
}

void frmConfig::on_btnBank_clicked()
{
    QString dbPath=myApp::AppPath+QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss%1").arg(".db");
    bool ok=myHelper::CopyFile(myApp::AppPath+"AMS.db",dbPath);
    if (ok){
        QString TempMsg="备份数据库成功";
        myAPI::AddEventInfoUser(TempMsg);
        myHelper::ShowMessageBoxInfo(QString("%1,位置:%2!").arg(TempMsg).arg(dbPath));
    }else{
        QString TempMsg="备份数据库失败";
        myAPI::AddEventInfoUser(TempMsg);
        myHelper::ShowMessageBoxError(QString("%1!").arg(TempMsg));
    }
}

//恢复出厂值、清空防区数据、主机数据、用户数据、压缩数据库
void frmConfig::on_btnSetDefault_clicked()
{
    if (myHelper::ShowMessageBoxQuesion("确定要将数据库恢复出厂值吗?")==0) {
        QSqlQuery query;

        QString sql="delete from DefenceInfo;";//清空防区表
        query.exec(sql);

        sql="delete from EventInfo;";//清空事件记录表
        query.exec(sql);

        sql="delete from HostInfo;";//清空主机表
        query.exec(sql);

        sql="delete from UserInfo where UserName!='admin';";//清空用户表
        query.exec(sql);

        sql="VACUUM;";//压缩数据库
        query.exec(sql);

        QString TempMsg="恢复出厂值成功";
        myAPI::AddEventInfoUser(TempMsg);
        myHelper::ShowMessageBoxInfo(QString("%1!").arg(TempMsg));
    }
}

/******操作员设置处理******/
void frmConfig::ReadUserInfo()
{
    ui->listUser->clear();
    TempUserName.clear();
    TempUserInfo.clear();
    QSqlQuery query;
    query.exec("select [UserName],[UserPwd],[UserType] from [UserInfo]");
    while(query.next()){
        //添加用户姓名到用户列表
        TempUserName<<query.value(0).toString();
        TempUserInfo<<QString("%1|%2|%3")
                      .arg(query.value(0).toString())
                      .arg(query.value(1).toString())
                      .arg(query.value(2).toString());
    }

    ui->listUser->addItems(TempUserName);
    if (TempUserName.count()>0) {
        ui->listUser->setCurrentRow(0);
    }
}

void frmConfig::on_listUser_currentRowChanged(int currentRow)
{
    if (currentRow<0){return;}
    QStringList user=TempUserInfo[currentRow].split("|");
    ui->txtUserName->setText(user[0]);
    ui->txtUserPwd->setText(user[1]);
    ui->txtUserType->setCurrentIndex(ui->txtUserType->findText(user[2]));
}

bool frmConfig::IsExistUser(QString UserName)
{
    foreach (QString name, TempUserName){
        if (name==UserName){
            myHelper::ShowMessageBoxError("该操作员已经存在,请重新填写!");
            ui->txtUserName->setFocus();
            return true;
        }
    }
    return false;
}

bool frmConfig::CheckUserInfo(QString UserName,QString UserPwd)
{
    if (UserName==""){
        myHelper::ShowMessageBoxError("操作员姓名不能为空,请重新填写!");
        ui->txtUserName->setFocus();
        return false;
    }

    if (UserPwd==""){
        myHelper::ShowMessageBoxError("操作员密码不能为空,请重新填写!");
        ui->txtUserPwd->setFocus();
        return false;
    }

    return true;
}

bool frmConfig::CheckAdmin(QString UserName)
{
    if (UserName==""){
        myHelper::ShowMessageBoxError("请选择要删除或者修改的操作员!");
        return false;
    }

    if (UserName=="admin"){
        myHelper::ShowMessageBoxError("超级管理员不能删除和修改!");
        return false;
    }

    return true;
}

void frmConfig::on_btnAdd_clicked()
{
    QString UserName=ui->txtUserName->text();
    QString UserPwd=ui->txtUserPwd->text();
    QString UserType=ui->txtUserType->currentText();

    if (!this->CheckUserInfo(UserName,UserPwd)){return;}
    if (this->IsExistUser(UserName)){return;}

    QSqlQuery query;
    QString sql="insert into [UserInfo]([UserName],[UserPwd],[UserType])";
    sql+="values('"+UserName+"','";
    sql+=UserPwd+"','";
    sql+=UserType+"')";
    query.exec(sql);
    this->ReadUserInfo();//重新加载用户列表
}

void frmConfig::on_btnDelete_clicked()
{
    QString TempUserName=ui->listUser->currentIndex().data().toString();
    if (!this->CheckAdmin(TempUserName)){return;}

    if (myHelper::ShowMessageBoxQuesion("确定要删除该操作员吗?")==0){
        QSqlQuery query;
        QString sql="delete from [UserInfo] where [UserName]='"+TempUserName+"'";
        query.exec(sql);
        this->ReadUserInfo();//重新加载用户列表
    }
}

void frmConfig::on_btnUpdate_clicked()
{
    QString TempUserName=ui->listUser->currentIndex().data().toString();
    if (!this->CheckAdmin(TempUserName)){return;}

    QString UserName=ui->txtUserName->text();
    QString UserPwd=ui->txtUserPwd->text();
    QString UserType=ui->txtUserType->currentText();

    if (!this->CheckUserInfo(UserName,UserPwd)){return;}

    QSqlQuery query;
    QString sql="update [UserInfo] set";
    sql+=" [UserName]='"+UserName;
    sql+="',[UserPwd]='"+UserPwd;
    sql+="',[UserType]='"+UserType;
    sql+="' where [UserName]='"+TempUserName+"'";
    query.exec(sql);
    this->ReadUserInfo();
}

void frmConfig::on_btnPwd_clicked()
{
    frmPwd *pwd=new frmPwd;
    pwd->exec();
}
