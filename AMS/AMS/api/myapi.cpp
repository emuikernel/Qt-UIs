#include "myapi.h"
#include "myhelper.h"
#include "myapp.h"
#include "../frmhostinfo.h"

/* 说明:核心处理模块实现文件
 * 功能:加载所有防区主机信息,实现布防撤防等操作
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
myAPI::myAPI(QObject *parent) :
    QObject(parent)
{
}


QGroupBox *boxMain;             //显示地图和防区
QTableWidget *tableWidget;      //显示临时消息
QTreeWidget *treeWidget;        //显示主机防区树状图
QListWidget *listWidget;        //显示防区缩略图
QScrollArea *scrollArea;        //显示主机状态
QGroupBox *boxPanel;
int messageCount;               //已添加消息条数

void myAPI::AddMessage(QString message)
{
    //超过25条数据则自动清空,重新计数
    int count=myApp::DeskHeigth/33;
    if (messageCount>=count){
        tableWidget->clearContents();
        messageCount=0;
    }

    QTableWidgetItem *itemTime = new QTableWidgetItem(QTime::currentTime().toString("HH:mm:ss"));
    QTableWidgetItem *itemMessage = new QTableWidgetItem(message);

    tableWidget->setItem(messageCount,0,itemTime);
    tableWidget->setItem(messageCount,1,itemMessage);

    messageCount++;
}

void myAPI::AddEventInfo(QString TriggerType, QString TriggerContent,
                         QString DefenceID, QString DefenceName,
                         QString HostID,QString HostName,int SubSystem)
{
    QString sql = "insert into [EventInfo]([TriggerTime],[TriggerType],";
    sql+="[TriggerContent],[TriggerUser],[DefenceID],[DefenceName],[HostID],";
    sql+="[HostName],[SubSystem])values('";
    sql += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "','";
    sql += TriggerType + "','";
    sql += TriggerContent + "','";
    sql += myApp::CurrentUserName + "','";
    sql += DefenceID + "','";
    sql += DefenceName + "','";
    sql += HostID + "','";
    sql += HostName + "','";
    sql += QString::number(SubSystem) + "')";

    QSqlQuery query;
    query.exec(sql);
}

void myAPI::AddEventInfoUser(QString TriggerContent)
{
    AddEventInfo("用户操作", TriggerContent,"000", "000","0","0",0);
}

void myAPI::AddEventInfoHost(QString TriggerContent,QString HostID,QString HostName)
{
    AddEventInfo("主机消息", TriggerContent,"000", "000",HostID,HostName,0);
}

void myAPI::SetControl(QGroupBox *BoxMain, QTableWidget *TableWidget, QTreeWidget *TreeWidget,
                       QListWidget *ListWidget, QScrollArea *ScrollArea, QGroupBox *BoxPanel)
{
    boxMain=BoxMain;
    tableWidget=TableWidget;
    treeWidget=TreeWidget;
    listWidget=ListWidget;
    scrollArea=ScrollArea;
    boxPanel=BoxPanel;

    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    tableWidget->verticalHeader()->setVisible(false);//隐藏行表头


    //设置列数和列宽
    tableWidget->setColumnCount(2);
    tableWidget->setColumnWidth(0,75);
    tableWidget->setColumnWidth(1,155);

    //设置行高
    int count=myApp::DeskHeigth/33;
    tableWidget->setRowCount(count);
    for(int i=0;i<count;i++){
        tableWidget->setRowHeight(i,24);
    }

    QStringList headText;
    headText<<"时间"<<"事件"<<"主机";
    tableWidget->setHorizontalHeaderLabels(headText);//设置表头
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许双击编辑
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式为单行选中

    treeWidget->setColumnCount(1); //设置列数
    treeWidget->setColumnWidth(0,250);
    treeWidget->header()->setVisible(false);//隐藏列标题
    treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编辑
}


void myAPI::InitDefenceAll(QGroupBox *boxMain)
{
    qDeleteAll(boxMain->findChildren<myDefenceButton *>());//清空原有防区

    QSqlQuery query;
    QString sql=QString("select * from [DefenceInfo]");

    query.exec(sql);
    while (query.next()){
        QString tempDefenceID=query.value(0).toString();
        QString tempDefenceName=query.value(1).toString();
        QString tempDefenceType=query.value(2).toString();
        int tempSubsystem=query.value(3).toInt();
        QString tempHostID=query.value(4).toString();
        QString tempHostName=query.value(5).toString();
        int tempDefenceIndex=query.value(6).toInt();
        QString tempDefenceSound=query.value(7).toString();
        QString tempDefenceImage=query.value(8).toString();
        QString tempDefenceUser=query.value(9).toString();
        int tempDefenceX=query.value(10).toInt();
        int tempDefenceY=query.value(11).toInt();

        myDefenceButton *btn=new myDefenceButton
                (boxMain,tempDefenceID,tempDefenceName,GetDefenceType(tempDefenceType),
                 tempSubsystem,tempHostID,tempHostName,tempDefenceIndex,
                 tempDefenceSound,tempDefenceImage,tempDefenceUser,
                 tempDefenceX,tempDefenceY,DefenceStatus_GuZhang);

        btn->setGeometry(tempDefenceX,tempDefenceY,35,35);
    }
}

void myAPI::InitDefenceAll()
{
    qDeleteAll(boxMain->findChildren<myDefenceButton *>());//清空原有防区

    QSqlQuery query;
    QString sql=QString("select * from [DefenceInfo]");

    query.exec(sql);
    while (query.next()){
        QString tempDefenceID=query.value(0).toString();
        QString tempDefenceName=query.value(1).toString();
        QString tempDefenceType=query.value(2).toString();
        int tempSubsystem=query.value(3).toInt();
        QString tempHostID=query.value(4).toString();
        QString tempHostName=query.value(5).toString();
        int tempDefenceIndex=query.value(6).toInt();
        QString tempDefenceSound=query.value(7).toString();
        QString tempDefenceImage=query.value(8).toString();
        QString tempDefenceUser=query.value(9).toString();
        int tempDefenceX=query.value(10).toInt();
        int tempDefenceY=query.value(11).toInt();

        myDefenceButton *btn=new myDefenceButton
                (boxMain,tempDefenceID,tempDefenceName,GetDefenceType(tempDefenceType),
                 tempSubsystem,tempHostID,tempHostName,tempDefenceIndex,
                 tempDefenceSound,tempDefenceImage,tempDefenceUser,
                 tempDefenceX,tempDefenceY,DefenceStatus_GuZhang);

        btn->setGeometry(tempDefenceX,tempDefenceY,35,35);
    }
}

void myAPI::InitDefenceCurrent(QGroupBox *boxMain)
{
    //设置防区地图为背景图
    boxMain->setStyleSheet(QString("border-image:url(%1MAP/%2);")
                           .arg(myApp::AppPath)
                           .arg(myApp::CurrentImage));

    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();

    foreach (myDefenceButton *b, btn){
        if (b->GetDefenceImage()==myApp::CurrentImage){
            b->setVisible(true);
        }else{
            b->setVisible(false);
        }
    }
}

void myAPI::InitDefenceCurrent()
{
    //设置防区地图为背景图
    boxMain->setStyleSheet(QString("border-image:url(%1MAP/%2);")
                           .arg(myApp::AppPath)
                           .arg(myApp::CurrentImage));

    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();

    foreach (myDefenceButton *b, btn){
        if (b->GetDefenceImage()==myApp::CurrentImage){
            b->setVisible(true);
        }else{
            b->setVisible(false);
        }
    }
}

void myAPI::InitTreeWidget()
{
    treeWidget->clear();
    //逐个添加主机
    QSqlQuery queryHost;
    QString sqlHost="select [HostID],[HostName] from [HostInfo]";
    queryHost.exec(sqlHost);

    while(queryHost.next()){
        QString tempHostID=queryHost.value(0).toString();
        QString tempHostName=queryHost.value(1).toString();

        QTreeWidgetItem *itemHost = new QTreeWidgetItem
                (treeWidget,QStringList(tempHostName+"["+tempHostID+"]"));
        itemHost->setIcon(0,QIcon(":/image/tree1.png"));

        QSqlQuery queryDefence;
        QString sqlDefence=QString("select [DefenceID],[DefenceName] from [DefenceInfo] where [HostID]='%1'").arg(tempHostID);
        queryDefence.exec(sqlDefence);

        while(queryDefence.next()){
            QString tempDefenceIDX=queryDefence.value(0).toString();
            QString tempDefenceNameX=queryDefence.value(1).toString();
            QTreeWidgetItem *itemDefence = new QTreeWidgetItem(itemHost,QStringList(QString(tempDefenceNameX+"["+tempDefenceIDX+"]")));
            itemDefence->setIcon(0,QIcon(":/image/tree2.png"));
            itemHost->addChild(itemDefence);
        }
    }
    treeWidget->expandAll(); //结点全部展开
}

void myAPI::InitListWidget()
{
    listWidget->clear();
    listWidget->setViewMode(QListView::IconMode);//设置为图标显示模式
    listWidget->setResizeMode(QListView::Adjust);//设置为自适应
    listWidget->setIconSize(QSize(200,180));//设置图标大小
    listWidget->setMovement(QListView::Static);//设置为不可拖动
    QDir imagePath(myApp::AppPath+"MAP");
    QStringList filter;
    filter<<"*.jpg"<<"*.bmp"<<"*.png";//过滤图片文件拓展名
    QStringList maps=imagePath.entryList(filter);

    int count=maps.count();
    for (int i=0;i<count;i++) {
        QString tempMap=QString("%1MAP/%2").arg(myApp::AppPath).arg(maps[i]);
        QListWidgetItem *listItem = new QListWidgetItem(QIcon(tempMap),maps[i],listWidget,i);
        listItem->setTextAlignment(Qt::AlignCenter);
    }
}

void myAPI::InitHostInfo()
{
    int x=1;
    int y=1;
    int count=0;
    int tempHeight=140;

    //只有龙阅脉冲主机才需要140的高度布局转高压转低压
    if (myApp::NETHostType!="LYMC110_NET"){
        tempHeight=108;
    }

    boxPanel->setGeometry(0,0,1180,2500);

    QSqlQuery query;
    QString sql="select * from [HostInfo]";
    query.exec(sql);
    while(query.next()){
        QString tempHostType=query.value(0).toString();
        QString tempHostID=query.value(1).toString();
        QString tempHostName=query.value(2).toString();
        QString tempConnectValue1=query.value(3).toString();
        QString tempConnectValue2=query.value(4).toString();

        frmHostInfo *info=new frmHostInfo(boxPanel);

        info->setGeometry(x,y,235,tempHeight);
        info->SetHostInfo(tempHostType,tempHostID,tempHostName,tempConnectValue1,tempConnectValue2);

        if (tempHostType=="HT110B_COM"){
            info->SetOnLine(true);//HT110B_COM主机没有心跳来判断上下线
        }else{
            info->SetOnLine(false);
        }

        info->SetStatus("撤防");


        x+=235;
        count++;
        //超过五个主机时换行排列
        if (count>=5){
            x=1;
            y+=tempHeight;
            count=0;
        }
    }
    //将存放主机面板的容器放入滚动条区域
    scrollArea->setWidget(boxPanel);
}

//用于龙阅脉冲主机查询主机对应的主机地址和键盘编号
void myAPI::GetHostConnectValue(QString hostID, QString &hostConnectValue1, QString &hostConnectValue2)
{
    QSqlQuery query;
    QString sql=QString("select [HostConnectValue1],[HostConnectValue2] from [HostInfo] where [HostID]='%1'")
            .arg(hostID);

    query.exec(sql);
    query.next();

    hostConnectValue1=query.value(0).toString();
    hostConnectValue2=query.value(1).toString();
}

//用于龙阅脉冲主机根据主机地址和键盘编号查询主机编号和主机名称
void myAPI::GetHostID_Name(QString hostConnectValue1, QString hostConnectValue2, QString &hostID, QString &hostName)
{
    QSqlQuery query;
    QString sql=QString("select [HostID],[HostName] from [HostInfo] where [HostConnectValue1]='%1' and [HostConnectValue2]='%2'")
            .arg(hostConnectValue1).arg(hostConnectValue2);

    query.exec(sql);
    query.next();

    hostID=query.value(0).toString();
    hostName=query.value(1).toString();
}

//用于宏泰主机根据主机编号000001查询出对应主机名称
QString myAPI::GetHostName(QString hostID)
{
    QSqlQuery query;
    QString sql=QString("select [HostName] from [HostInfo] where [HostID]='%1'")
            .arg(hostID);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

//用于宏泰主机和DS7400主机查询出布防代码或者对应串口号
QString myAPI::GetHostConnectValue1(QString hostID)
{
    QSqlQuery query;
    QString sql=QString("select [HostConnectValue1] from [HostInfo] where [HostID]='%1'")
            .arg(hostID);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

//用于宏泰主机查询出撤防代码
QString myAPI::GetHostConnectValue2(QString hostID)
{
    QSqlQuery query;
    QString sql=QString("select [HostConnectValue2] from [HostInfo] where [HostID]='%1'")
            .arg(hostID);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

QString myAPI::GetDefenceID(int tempID)
{
    QString defenceID;
    if (tempID<10){
        defenceID=QString("00%1").arg(tempID);
    }else if (tempID<100){
        defenceID=QString("0%1").arg(tempID);
    }
    return defenceID;
}

QString myAPI::GetDefenceID(QString hostID, QString defenceIndex)
{
    QSqlQuery query;
    QString sql=QString("select [DefenceID] from [DefenceInfo] where [HostID]='%1' and [DefenceIndex]='%2'")
            .arg(hostID).arg(defenceIndex);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

QString myAPI::GetDefenceName(QString hostID, QString defenceID)
{
    QSqlQuery query;
    QString sql=QString("select [DefenceName] from [DefenceInfo] where [HostID]='%1' and [DefenceID]='%2'")
            .arg(hostID).arg(defenceID);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

bool myAPI::IsExistHost(QString hostID)
{
    QSqlQuery query;
    QString sql=QString("select [HostName] from [HostInfo] where [HostID]='%1'")
            .arg(hostID);

    query.exec(sql);
    return query.next();
}

//2014-3-20增加查询防区对应报警声音文件,用于报警时播放对应报警声音
QString myAPI::GetDefenceSound(QString hostID, QString defenceID)
{
    QSqlQuery query;
    QString sql=QString("select [DefenceSound] from [DefenceInfo] where [HostID]='%1' and [DefenceID]='%2'")
            .arg(hostID).arg(defenceID);

    query.exec(sql);
    query.next();
    QString temp=query.value(0).toString();
    return temp;
}

//用于宏泰主机区分报警防区类型
QString myAPI::GetDefenceType(DefenceType type)
{
    switch (type){
    case DefenceType_zhoujie:return "周界";break;
    case DefenceType_ruqin:return "入侵";break;
    case DefenceType_jinji:return "紧急";break;
    case DefenceType_xiaofang:return "消防";break;
    case DefenceType_wuxiao:return "无效";break;
    default:return "周界";break;
    }
}

DefenceType myAPI::GetDefenceType(QString type)
{
    DefenceType tempDefenceType=DefenceType_zhoujie;
    if (type=="周界防区"){
        tempDefenceType=DefenceType_zhoujie;
    }else if(type=="入侵防区"){
        tempDefenceType=DefenceType_ruqin;
    }else if(type=="紧急防区"){
        tempDefenceType=DefenceType_jinji;
    }else if(type=="消防防区"){
        tempDefenceType=DefenceType_xiaofang;
    }else if(type=="无效防区"){
        tempDefenceType=DefenceType_wuxiao;
    }
    return tempDefenceType;
}

void myAPI::HostConnect(QString hostID, QString hostName)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID){
            b->SetDefenceStatus(DefenceStatus_CheFang);            
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetOnLine(true);
            break;
        }
    }

    AddMessage(hostName+"上线");
    AddEventInfoHost("主机上线",hostID,hostName);
}

void myAPI::HostDisConnect(QString hostID, QString hostName)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID){
            b->SetDefenceStatus(DefenceStatus_GuZhang);
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetOnLine(false);
            break;
        }
    }

    AddMessage(hostName+"下线");
    AddEventInfoHost("主机下线",hostID,hostName);
}

void myAPI::HostError(QString hostID, QString hostName, QString error)
{
    //对应主界面添加快捷信息
    AddMessage(hostName+error);
    //将操作记录添加到数据库
    AddEventInfoHost(error,hostID,hostName);
}

//用于龙阅脉冲主机高压处理,对应主机面板处理
void myAPI::HostHigh(QString hostID)
{
    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("高压");
            break;
        }
    }
}

//用于龙阅脉冲主机低压处理,对应主机面板处理
void myAPI::HostLow(QString hostID)
{
    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("低压");
            break;
        }
    }
}

//用户对整台主机进行布防操作
void myAPI::BuFang(QString hostID,QString hostName)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetDefenceStatus()!=DefenceStatus_PangLu){
            if (b->GetHostID()==hostID){
                b->SetDefenceStatus(DefenceStatus_BuFang);
            }
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("布防");
            break;
        }
    }

    AddMessage(hostName+"布防");
    AddEventInfo("用户操作","主机布防","000","000",hostID,hostName,0);
}

//用户对主机子系统进行布防
void myAPI::BuFang(QString hostID,QString hostName,int subSystem)
{    
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn) {
        if (b->GetDefenceStatus()!=DefenceStatus_PangLu){
            if (b->GetHostID()==hostID && b->GetSubSystem()==subSystem){
                b->SetDefenceStatus(DefenceStatus_BuFang);
            }
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("布防");
            break;
        }
    }

    AddMessage(hostName+"布防");
    AddEventInfo("用户操作","主机布防","000","000",hostID,hostName,subSystem);
}

//用户对主机单个防区进行布防操作
void myAPI::BuFang(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetDefenceStatus()!=DefenceStatus_PangLu){
            if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
                b->SetDefenceStatus(DefenceStatus_BuFang);
                AddMessage(b->GetDefenceName()+"布防");
                AddEventInfo("用户操作","防区布防",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
                break;
            }
        }
    }
}

//用户对单个主机进行撤防操作
void myAPI::CheFang(QString hostID,QString hostName)
{    
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID){
            b->SetDefenceStatus(DefenceStatus_CheFang);
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("撤防");
            break;
        }
    }

    AddMessage(hostName+"撤防");
    AddEventInfo("用户操作","主机撤防","000","000",hostID,hostName,0);
}

//用户对单个主机对应子系统进行撤防操作,对应区分子系统的主机
void myAPI::CheFang(QString hostID,QString hostName,int subSystem)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetSubSystem()==subSystem){
            b->SetDefenceStatus(DefenceStatus_CheFang);
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("撤防");
            break;
        }
    }

    AddMessage(hostName+"撤防");
    AddEventInfo("用户操作","主机撤防","000","000",hostID,hostName,subSystem);
}

//用户对单个防区进行撤防操作
void myAPI::CheFang(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            b->SetDefenceStatus(DefenceStatus_CheFang);
            AddMessage(b->GetDefenceName()+"撤防");
            AddEventInfo("用户操作","防区撤防",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }
}

//主机故障旁路
void myAPI::PangLu(QString hostID,QString hostName)
{    
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID){
            b->SetDefenceStatus(DefenceStatus_PangLu);
        }
    }
    AddMessage(hostName+"旁路");
    AddEventInfoUser("主机旁路");
}

//主机防区旁路
void myAPI::PangLu(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            b->SetDefenceStatus(DefenceStatus_PangLu);
            AddMessage(b->GetDefenceName()+"旁路");
            AddEventInfo("用户操作","防区旁路",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }
}

//主机防区报警时触发
void myAPI::BaoJing(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            //切换到当前防区地图
            myApp::CurrentImage=b->GetDefenceImage();
            InitDefenceCurrent();
            b->SetDefenceStatus(DefenceStatus_BaoJing);
            AddMessage(b->GetDefenceName()+"报警");
            AddEventInfo("主机消息",GetDefenceType(b->GetDefenceType())+"防区报警",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }

    QList<frmHostInfo *> hostInfo=boxPanel->findChildren<frmHostInfo *>();
    foreach (frmHostInfo *host, hostInfo){
        if (host->GetHostID()==hostID){
            host->SetStatus("报警");
            break;
        }
    }
}

//主机防区断开和故障时触发
void myAPI::GuZhang(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            if (b->GetDefenceStatus()==DefenceStatus_BaoJing){return;}
            b->SetDefenceStatus(DefenceStatus_GuZhang);
            AddMessage(b->GetDefenceName()+"故障");
            AddEventInfo("主机消息","防区故障",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }
}

//主机现场防区开路或者短路恢复触发
void myAPI::HuiFu(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            b->SetDefenceStatus(DefenceStatus_CheFang);
            AddMessage(b->GetDefenceName()+"现场复位");
            AddEventInfo("主机消息","防区现场复位",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }
}

//用户按下右下角弹框中的软件复位按钮触发
void myAPI::HuiFuSoft(QString hostID,QString hostName,QString defenceID)
{
    QList<myDefenceButton *> btn=boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn){
        if (b->GetHostID()==hostID && b->GetDefenceID()==defenceID){
            b->SetDefenceStatus(DefenceStatus_CheFang);
            AddMessage(b->GetDefenceName()+"软件复位");
            AddEventInfo("用户操作","防区软件复位",defenceID,b->GetDefenceName(),hostID,hostName,b->GetSubSystem());
            break;
        }
    }

}
