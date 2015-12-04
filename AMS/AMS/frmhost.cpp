#include "frmhost.h"
#include "ui_frmhost.h"
#include "api/myhelper.h"
#include "api/myapp.h"
#include "api/myapi.h"

/* 说明:主机管理界面模块实现文件
 * 功能:添加删除修改主机等,多种主机切换
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
frmHost::frmHost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmHost)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    this->InitForm();
    this->ReadHostInfo();
}

frmHost::~frmHost()
{
    delete ui;
}

void frmHost::closeEvent(QCloseEvent *)
{
    if (myApp::CurrentUserType=="管理员"){
        myHelper::ShowMessageBoxInfo("如果更改过信息,请重新启动主程序!");
    }
}

void frmHost::on_txtHostType_currentIndexChanged(int index)
{
    QString tempHostType=ui->txtHostType->currentText();
    if (tempHostType=="LYMC110_NET"){
        ui->labHostConnectValue1->setText("主机地址:");
        ui->labHostConnectValue2->setText("键盘编号:");
        ui->labHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue1->setText("");
        ui->txtHostConnectValue2->setText("");
    }else if (tempHostType=="HT110B_NET"){
        ui->labHostConnectValue1->setText("布防命令:");
        ui->labHostConnectValue2->setText("撤防命令:");
        ui->labHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue2->setVisible(true);       
        ui->txtHostConnectValue1->setText("");
        ui->txtHostConnectValue2->setText("");
    }else if (tempHostType=="BXHOST_NET"){
        ui->labHostID->setText("站点编号:");
        ui->labHostName->setText("站点名称:");
        ui->labHostConnectValue1->setText("站点地址:");
        ui->labHostConnectValue2->setText("站点备注:");
        ui->labHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue1->setText("");
        ui->txtHostConnectValue2->setText("");
    }else if (tempHostType=="HT110B_COM" || tempHostType=="DS7400_COM" || tempHostType=="VISTA_COM"){
        ui->labHostConnectValue1->setText("通讯串口:");
        ui->labHostConnectValue2->setVisible(false);
        ui->txtHostConnectValue2->setVisible(false);        
        ui->txtHostConnectValue1->setText("COM1");
    }else if (tempHostType=="VISTAHOST_NET"){
        ui->labHostConnectValue1->setText("IP地址:");
        ui->labHostConnectValue2->setText("MAC地址:");
        ui->labHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue2->setVisible(true);
        ui->txtHostConnectValue1->setText("192.168.0.3");
        ui->txtHostConnectValue2->setText("00-E0-4C-E6-AB-5C");
    }
}

void frmHost::InitForm()
{
    QStringList hostType;

    if (myApp::NETHostType=="HT110B_NET"){
        hostType<<"HT110B_NET"<<"HT110B_COM"<<"DS7400_COM";
    }else if (myApp::NETHostType=="LYMC110_NET"){
        hostType<<"LYMC110_NET"<<"DS7400_COM";
    }else if(myApp::NETHostType=="BXHOST_NET"){
        hostType<<"BXHOST_NET"<<"DS7400_COM";
        this->setWindowTitle("站点管理");
    }else if (myApp::NETHostType=="VISTAHOST_NET"){
        hostType<<"VISTAHOST_NET"<<"DS7400_COM";
    }

    ui->txtHostType->addItems(hostType);

    if (myApp::CurrentUserType!="管理员"){
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
    }
}

void frmHost::ReadHostInfo()
{
    ui->listHost->clear();
    TempHostInfo.clear();

    QSqlQuery query;
    query.exec("select [HostType],[HostID],[HostName],[HostConnectValue1],[HostConnectValue2] from [HostInfo]");
    while(query.next()){
        QString hostInfo=QString("%1|%2|%3|%4|%5")
                .arg(query.value(0).toString())
                .arg(query.value(1).toString())
                .arg(query.value(2).toString())
                .arg(query.value(3).toString())
                .arg(query.value(4).toString());
        TempHostInfo<<hostInfo;
        ui->listHost->addItem(query.value(2).toString()+"["+query.value(1).toString()+"]");
    }
}

void frmHost::on_listHost_currentRowChanged(int currentRow)
{
    if (currentRow<0){return;}
    QStringList hostInfo=TempHostInfo[currentRow].split("|");
    ui->txtHostType->setCurrentIndex(ui->txtHostType->findText(hostInfo[0]));
    ui->txtHostID->setText(hostInfo[1]);
    ui->txtHostName->setText(hostInfo[2]);
    ui->txtHostConnectValue1->setText(hostInfo[3]);
    ui->txtHostConnectValue2->setText(hostInfo[4]);
}

bool frmHost::CheckHostInfo(QString HostType,QString HostID,QString HostName,QString HostConnectValue1,QString HostConnectValue2)
{
    if (HostID==""){
        myHelper::ShowMessageBoxError("编号不能为空,请重新填写!");
        ui->txtHostID->setFocus();
        return false;
    }

    if (HostName==""){
        myHelper::ShowMessageBoxError("名称不能为空,请重新填写!");
        ui->txtHostName->setFocus();
        return false;
    }

    if (HostType=="LYMC110_NET"){
        if (HostConnectValue1==""){
            myHelper::ShowMessageBoxError("主机地址不能为空,请重新填写!");
            ui->txtHostConnectValue1->setFocus();
            return false;
        }

        if (HostConnectValue2==""){
            myHelper::ShowMessageBoxError("键盘编号不能为空,请重新填写!");
            ui->txtHostConnectValue2->setFocus();
            return false;
        }
        return true;
    }else if (HostType=="HT110B_NET"){
        if (HostConnectValue1==""){
            myHelper::ShowMessageBoxError("布防代码不能为空,请重新填写!");
            ui->txtHostConnectValue1->setFocus();
            return false;
        }

        if (HostConnectValue2==""){
            myHelper::ShowMessageBoxError("撤防代码不能为空,请重新填写!");
            ui->txtHostConnectValue2->setFocus();
            return false;
        }
        return true;
    }else if(HostType=="HT110B_COM" || HostType=="DS7400_COM" || HostType=="VISTA_COM") {
        if (HostConnectValue1==""){
            myHelper::ShowMessageBoxError("通讯串口不能为空,请重新填写!");
            ui->txtHostConnectValue1->setFocus();
            return false;
        }
        return true;
    }else if (HostType=="VISTAHOST_NET"){
        if (HostConnectValue1==""){
            myHelper::ShowMessageBoxError("IP地址不能为空,默认为192.168.0.3,,请重新填写!");
            ui->txtHostConnectValue1->setFocus();
            return false;
        }

        if (HostConnectValue2==""){
            myHelper::ShowMessageBoxError("MAC地址不能为空,默认为00-E0-4C-E6-AB-5C,请重新填写!");
            ui->txtHostConnectValue2->setFocus();
            return false;
        }

        return true;
    }
    return true;
}

void frmHost::on_btnAdd_clicked()
{
    QString HostType=ui->txtHostType->currentText();
    QString HostID=ui->txtHostID->text();
    QString HostName=ui->txtHostName->text();
    QString HostConnectValue1=ui->txtHostConnectValue1->text();
    QString HostConnectValue2=ui->txtHostConnectValue2->text();

    if (!this->CheckHostInfo(HostType,HostID,HostName,HostConnectValue1,HostConnectValue2)){return;}
    if (myAPI::IsExistHost(HostID)){
        myHelper::ShowMessageBoxError("该主机编号已存在,请重新填写!");
        ui->txtHostID->setFocus();
        return;
    }

    QSqlQuery query;
    QString sql="insert into [HostInfo]([HostType],[HostID],[HostName],[HostConnectValue1],[HostConnectValue2])";
    sql+="values('"+HostType+"','";
    sql+=HostID+"','";
    sql+=HostName+"','";
    sql+=HostConnectValue1+"','";
    sql+=HostConnectValue2+"')";
    query.exec(sql);

    this->ReadHostInfo();
}

void frmHost::on_btnDelete_clicked()
{
    QString tempHostInfo=ui->listHost->currentIndex().data().toString();

    if (tempHostInfo==""){
        myHelper::ShowMessageBoxError("请选择要删除的主机!");
        return;
    }

    if (myHelper::ShowMessageBoxQuesion("确定要删除主机吗?对应防区会一起删除!")==0) {
        QString tempHostID=tempHostInfo.split("[")[1].split("]")[0];
        QSqlQuery query;
        QString sql="delete from [HostInfo] where [HostID]='"+tempHostID+"'";
        query.exec(sql);

        //同步删除防区表中该主机对应的防区
        sql="delete from [DefenceInfo] where [HostID]='"+tempHostID+"'";
        query.exec(sql);

        this->ReadHostInfo();
    }
}

void frmHost::on_btnUpdate_clicked()
{
    QString tempHostInfo=ui->listHost->currentIndex().data().toString();
    if (tempHostInfo==""){
        myHelper::ShowMessageBoxError("请选择要修改的主机!");
        return;
    }

    QString HostType=ui->txtHostType->currentText();
    QString HostID=ui->txtHostID->text();
    QString HostName=ui->txtHostName->text();
    QString HostConnectValue1=ui->txtHostConnectValue1->text();
    QString HostConnectValue2=ui->txtHostConnectValue2->text();

    if (!this->CheckHostInfo(HostType,HostID,HostName,HostConnectValue1,HostConnectValue2)){return;}

    QString tempHostID=tempHostInfo.split("[")[1].split("]")[0];
    QSqlQuery query;
    QString sql="update [HostInfo] set";
    sql+=" [HostType]='"+HostType;
    sql+="',[HostID]='"+HostID;
    sql+="',[HostName]='"+HostName;
    sql+="',[HostConnectValue1]='"+HostConnectValue1;
    sql+="',[HostConnectValue2]='"+HostConnectValue2;
    sql+="' where [HostID]='"+tempHostID+"'";
    query.exec(sql);

    //对应防区表中的防区对应主机名称和主机地址也要同步修改
    sql="update [DefenceInfo] set";
    sql+=" [HostID]='"+HostID;
    sql+="',[HostName]='"+HostName;
    sql+="' where [HostID]='"+tempHostID+"'";
    query.exec(sql);

    this->ReadHostInfo();
}
