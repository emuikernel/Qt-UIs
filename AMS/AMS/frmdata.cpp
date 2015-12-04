#include "frmdata.h"
#include "ui_frmdata.h"
#include <QDesktopServices>
#include "api/myhelper.h"
#include "api/myapp.h"
#include "api/myapi.h"
#include "frmselect.h"
#include "api/myprintapi.h"
#include "api/myexcelapi.h"

/* 说明:警情查询界面模块实现文件
 * 功能:多条件查询警情,可导出和打印警情
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
frmData::frmData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmData)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    this->InitForm();
}

frmData::~frmData()
{
    delete ui;
}

void frmData::InitForm()
{
    //初始化表格列名和列宽
    columnNames[0]="时间";
    columnNames[1]="类型";
    columnNames[2]="内容";
    columnNames[3]="值班员";
    columnNames[4]="主机号";
    columnNames[5]="主机名";
    columnNames[6]="防区号";
    columnNames[7]="防区名";
    columnNames[8]="子系统";

    columnWidths[0]=190;
    columnWidths[1]=80;
    columnWidths[2]=135;
    columnWidths[3]=80;
    columnWidths[4]=70;
    columnWidths[5]=100;
    columnWidths[6]=65;
    columnWidths[7]=100;
    columnWidths[8]=65;

    ui->ckTriggerTime->setChecked(false);
    ui->ckTriggerContent->setChecked(false);
    ui->ckTriggerType->setChecked(false);
    ui->ckUserName->setChecked(false);
    ui->ckDefenceName->setChecked(false);

    ui->dateStart->setEnabled(false);
    ui->dateEnd->setEnabled(false);
    ui->cboxTriggerType->setEnabled(false);
    ui->cboxUserName->setEnabled(false);
    ui->cboxDefenceName->setEnabled(false);
    ui->cboxTriggerContent->setEnabled(false);

    ui->dateStart->setDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate());

    ui->cboxTriggerType->addItem("主机消息");
    ui->cboxTriggerType->addItem("用户操作");

    QStringList triggerContent;
    triggerContent<<"防区报警"<<"主机布防"<<"主机撤防"<<"主机故障"<<"主机上线"<<"主机下线"
                 <<"防区现场复位"<<"防区软件复位"<<"主机旁路"<<"防区旁路"<<"防区故障"<<"电源故障";
    ui->cboxTriggerContent->addItems(triggerContent);

    //绑定防区名称和用户到对应下拉框
    Sqlite->BindData("DefenceName","DefenceInfo",ui->cboxDefenceName);
    Sqlite->BindData("UserName","UserInfo",ui->cboxUserName);

    Sqlite=new mySqliteAPI(this);//实例化数据库操作对象,以备调用
    //设置需要显示数据的表格和翻页的按钮
    Sqlite->SetControl(ui->tableEvent,ui->labInfo,ui->btnFirst,ui->btnPre,ui->btnNext,ui->btnLast);
    //加载初始数据
    Sqlite->BindData("EventInfo","TriggerTime","desc",15,columnNames,columnWidths);

    if (myApp::CurrentUserType!="管理员"){
        ui->btnDelete->setEnabled(false);
    }
}

void frmData::on_ckTriggerTime_toggled(bool checked)
{
    ui->dateEnd->setEnabled(checked);
    ui->dateStart->setEnabled(checked);
}

void frmData::on_ckTriggerContent_toggled(bool checked)
{
    ui->cboxTriggerContent->setEnabled(checked);
}

void frmData::on_ckTriggerType_toggled(bool checked)
{
    ui->cboxTriggerType->setEnabled(checked);
}

void frmData::on_ckDefenceName_toggled(bool checked)
{
    ui->cboxDefenceName->setEnabled(checked);
}

void frmData::on_ckUserName_toggled(bool checked)
{
    ui->cboxUserName->setEnabled(checked);
}

void frmData::on_btnSelect_clicked()
{
    //构建SQL语句
    QString sql="where 1=1";

    if (ui->ckUserName->isChecked()){
        sql+=" and [TriggerUser]='"+ui->cboxUserName->currentText()+"'";
    }

    if (ui->ckTriggerType->isChecked()){
        sql+=" and [TriggerType]='"+ui->cboxTriggerType->currentText()+"'";
    }

    if (ui->ckDefenceName->isChecked()){
        sql+=" and [DefenceName]='"+ui->cboxDefenceName->currentText()+"'";
    }

    if (ui->ckTriggerContent->isChecked()){
        //这里改为like通配符,用户查询防区报警时,会自动将周界防区报警/紧急防区报警等报警类型全部查出
        sql+=" and [TriggerContent] like '%"+ui->cboxTriggerContent->currentText()+"'";
    }

    if (ui->ckTriggerTime->isChecked()){
        sql+=" and date([TriggerTime])>='"+ui->dateStart->date().toString("yyyy-MM-dd")+"'";
        sql+=" and date([TriggerTime])<='"+ui->dateEnd->date().toString("yyyy-MM-dd")+"'";
    }

    //绑定数据到表格
    Sqlite->BindDataSelect("EventInfo","TriggerTime","desc",sql,15,columnNames,columnWidths);
}

void frmData::on_btnDelete_clicked()
{
    if (myApp::CurrentUserType!="管理员"){
        myHelper::ShowMessageBoxError("没有权限删除记录!");
        return;
    }

    frmSelect *frm=new frmSelect;//弹出日期选择界面
    if (frm->exec()==1){
        QString timeStart=frm->SelectStartDate();
        QString timeEnd=frm->SelectEndDate();
        //询问是否要删除数据
        if (myHelper::ShowMessageBoxQuesion("确定要删除 "+timeStart+" 到 "+timeEnd+" 的数据吗？")==0) {
            //执行sql语句,删除数据
            QString sql="delete from [EventInfo] where date([TriggerTime])>='";
            sql+=timeStart;
            sql += "' and date([TriggerTime])<='";
            sql+=timeEnd+"'";
            Sqlite->ExecuteSql(sql);

            QString TempMsg="删除数据成功";
            myAPI::AddEventInfoUser(TempMsg);
            myHelper::ShowMessageBoxInfo(QString("%1!").arg(TempMsg));

            //加载初始数据
            Sqlite->BindData("EventInfo","TriggerTime","desc",15,columnNames,columnWidths);
        }
    }
}

void frmData::on_btnPrint_clicked()
{
    frmSelect *frm=new frmSelect;//弹出日期选择界面
    if (frm->exec()==1){
        QString timeStart=frm->SelectStartDate();
        QString timeEnd=frm->SelectEndDate();

        //询问是否要打印
        if (myHelper::ShowMessageBoxQuesion("确定要打印 "+timeStart+" 到 "+timeEnd+" 的数据吗？")==0) {
            //根据用户选择的日期,查询出需要打印的数据
            QStringList content;
            QSqlQuery query;

            QString sql="select [TriggerTime],[TriggerType],[TriggerContent],";
            sql+="[TriggerUser],[DefenceName] from [EventInfo] where 1=1";
            sql+=" and date([TriggerTime])>='"+timeStart+"'";
            sql+=" and date([TriggerTime])<='"+timeEnd+"'";

            QString type="所有事件记录";
            bool checkType=ui->ckTriggerType->isChecked();
            if (checkType) {
                type=ui->cboxTriggerType->currentText()+"记录";
                sql += " and [TriggerType]='" + ui->cboxTriggerType->currentText() + "'";
            }

            sql += " order by [TriggerTime] desc";//降序
            query.exec(sql);

            //循环遍历数据,存储到QStringList中
            while(query.next()) {
                QString temp="";
                temp+=query.value(0).toString()+";";
                temp+=query.value(1).toString()+";";
                temp+=query.value(2).toString()+";";
                temp+=query.value(3).toString()+";";
                temp+=query.value(4).toString()+";";

                content.append(temp.mid(0,temp.length()-1));
            }

            QString columnName[5];
            columnName[0]="时间";
            columnName[1]="类型";
            columnName[2]="内容";
            columnName[3]="值班员";
            columnName[4]="防区名";

            int columnWidth[5];
            columnWidth[0]=160;
            columnWidth[1]=90;
            columnWidth[2]=140;
            columnWidth[3]=100;
            columnWidth[4]=100;
            //调用打印数据函数
            myPrintAPI::Print(type+"【"+timeStart+" 到 "+timeEnd+"】",columnName,columnWidth,5,content);

            QString TempMsg=QString("打印%1").arg(type);
            myAPI::AddEventInfoUser(TempMsg);
        }
    }
}

void frmData::on_btnExcel_clicked()
{
    frmSelect *frm=new frmSelect;//弹出日期选择界面
    if (frm->exec()==1){
        QString timeStart=frm->SelectStartDate();
        QString timeEnd=frm->SelectEndDate();
        //询问是否要导出数据
        if (myHelper::ShowMessageBoxQuesion("确定要导出 "+timeStart+" 到 "+timeEnd+" 的数据吗？")==0) {
            //根据选择的日期,查询出需要导出的数据
            QString type="所有事件记录";
            bool checkType=ui->ckTriggerType->isChecked();
            if (checkType){
                type=ui->cboxTriggerType->currentText()+"记录";
            }

            QStringList content;
            QSqlQuery query;

            QString sql="select * from [EventInfo] where 1=1";
            sql+=" and date([TriggerTime])>='"+timeStart+"'";
            sql+=" and date([TriggerTime])<='"+timeEnd+"'";

            if (checkType){
                sql += " and [TriggerType]='" + ui->cboxTriggerType->currentText() + "'";
            }

            sql += " order by [TriggerTime] desc";//降序
            query.exec(sql);

            int columnCount=query.record().count();//获取字段数量

            //循环遍历数据,存储到QStringList中
            while(query.next()){
                QString temp="";
                for(int i=0;i<columnCount;i++){
                    temp+=query.value(i).toString()+";";
                }
                content<<temp.mid(0,temp.length()-1);//去掉末尾的分号
            }
            //调用导出数据函数
            QString fileName=type+"【"+timeStart+"到"+timeEnd+"】";
            myExcelAPI::ToExcel(fileName+".xls",fileName,fileName,columnNames,columnWidths,columnCount,content);

            if(myHelper::ShowMessageBoxQuesion("导出数据到Excel成功,现在就打开吗？")==0) {
                QDesktopServices::openUrl(QUrl(fileName+".xls"));
            }

            QString TempMsg=QString("导出%1").arg(type);
            myAPI::AddEventInfoUser(TempMsg);
        }
    }
}
