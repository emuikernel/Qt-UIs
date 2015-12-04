#include "frmhostinfo.h"
#include "ui_frmhostinfo.h"
#include "api/myapp.h"

/* 说明:主机微型操作界面模块实现文件
 * 功能:对单个主机进行布防撤防操作
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
frmHostInfo::frmHostInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmHostInfo)
{
    ui->setupUi(this);

    this->hostType="HT110B_NET";
    this->hostID="000000";
    this->hostName="";
    this->hostConnectValue1="";
    this->hostConnectValue2="";
    ui->hostStatus->SetDefenceStatus(DefenceStatus_CheFang);
    ui->hostOnLine->SetDefenceStatus(DefenceStatus_GuZhang);
    ui->hostAlarm->SetDefenceStatus(DefenceStatus_DuanKai);
    ui->hostHigh->SetHigh();
}

frmHostInfo::~frmHostInfo()
{
    delete ui;
}


void frmHostInfo::SetHostInfo(QString hostType,QString hostID,QString hostName,QString hostConnectValue1,QString hostConnectValue2)
{
    this->hostType=hostType;
    this->hostID=hostID;
    this->hostName=hostName;
    this->hostConnectValue1=hostConnectValue1;
    this->hostConnectValue2=hostConnectValue2;

    if (myApp::NETHostType=="BXHOST_NET"){
        ui->labHostName->setText(QString("站点:%1").arg(hostName));
    }else{
        ui->labHostName->setText(QString("主机:%1").arg(hostName));
    }

    if (myApp::NETHostType=="LYMC110_NET"){
        ui->btnHigh->setVisible(true);
        ui->btnLow->setVisible(true);
        ui->hostHigh->setVisible(true);
        ui->labHighLow->setVisible(true);        
    }else{
        ui->btnHigh->setVisible(false);
        ui->btnLow->setVisible(false);
        ui->hostHigh->setVisible(false);
        ui->labHighLow->setVisible(false);
        ui->btnBuFang->setGeometry(160,36,61,25);
        ui->btnCheFang->setGeometry(160,66,61,25);
    }
}

void frmHostInfo::SetOnLine(bool onLine)
{
    if (onLine){
        ui->labOnLine->setText("在线");
        ui->hostOnLine->SetDefenceStatus(DefenceStatus_BuFang);
        ui->hostAlarm->SetDefenceStatus(DefenceStatus_DuanKai);
        //默认上线设置为高压
        ui->hostHigh->SetHigh();
        ui->labHighLow->setText("高压(值:5.5kv)");
    }else{
        ui->labOnLine->setText("离线");
        ui->hostOnLine->SetDefenceStatus(DefenceStatus_GuZhang);
    }
    ui->btnBuFang->setEnabled(onLine);
    ui->btnCheFang->setEnabled(onLine);
    ui->btnHigh->setEnabled(onLine);
    ui->btnLow->setEnabled(onLine);
}

void frmHostInfo::SetStatus(QString status)
{
    if (status=="布防"){
        ui->hostStatus->SetDefenceStatus(DefenceStatus_BuFang);
        ui->labStatus->setText(status);
    }else if (status=="撤防"){
        ui->hostStatus->SetDefenceStatus(DefenceStatus_CheFang);
        ui->hostAlarm->SetDefenceStatus(DefenceStatus_DuanKai);
        ui->labStatus->setText(status);
    }else if (status=="报警"){
        ui->hostAlarm->SetDefenceStatus(DefenceStatus_BaoJing);
    }else if (status=="高压"){
        ui->hostHigh->SetHigh();
        ui->labHighLow->setText("高压(值:5.5kv)");
    }else if (status=="低压"){
        ui->hostHigh->SetLow();
        ui->labHighLow->setText("低压(值:0.8kv)");
    }
}

void frmHostInfo::on_btnBuFang_clicked()
{

}

void frmHostInfo::on_btnCheFang_clicked()
{

}

void frmHostInfo::on_btnHigh_clicked()
{

}

void frmHostInfo::on_btnLow_clicked()
{

}
