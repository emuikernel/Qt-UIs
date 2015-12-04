#include "frmmain.h"
#include "ui_frmmain.h"
#include "api/myhelper.h"
#include "api/myapi.h"
#include "frmconfig.h"
#include "frmhost.h"
#include "frmdefence.h"
#include "frmdata.h"
#include "frmlogout.h"
#include "frmhostinfo.h"

/* 说明:主界面模块实现文件
 * 功能:将防区主机数据加载到主界面,初始化工具栏状态栏等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    myHelper::FormInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);//窗体居中显示
    this->InitForm();               //初始化界面

    //设置防区容器/防区树状列表/防区缩略图等
    myAPI::SetControl(ui->boxMain,ui->tableWidget,ui->treeWidget,
                      ui->listWidget,ui->scrollArea,boxPanel);
    myAPI::InitDefenceAll();        //加载所有防区数据到容器
    myAPI::InitDefenceCurrent();    //加载当前防区
    myAPI::InitTreeWidget();        //加载主机防区树状图
    myAPI::InitListWidget();        //加载防区缩略图    

    myAPI::InitHostInfo();          //加载主机实体图

    //2014-3-2增加防区主界面双击最大化,再次双击还原.安装过滤器监听鼠标双击事件.
    ui->boxMain->installEventFilter(this);
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::closeEvent(QCloseEvent *event)
{
    frmLogout *logout=new frmLogout;
    logout->exec();
    event->ignore();
}

//bool isMax=false;
//bool frmMain::eventFilter(QObject *obj, QEvent *event)
//{
//    //鼠标在防区地图上双击时最大化显示防区地图,再次双击还原.
//    if (event->type()==QEvent::MouseButtonDblClick) {
//        ui->statusBar->setVisible(isMax);
//        ui->mainToolBar->setVisible(isMax);
//        ui->gboxBottom->setVisible(isMax);
//        ui->tabWidget->setVisible(isMax);
//        isMax=!isMax;

//        return true;
//    }
//    return QObject::eventFilter(obj,event);
//}

int oldWidth=800;
int oldHeight=600;
int newWidth=0;
int newHeight=0;
void frmMain::resizeEvent(QResizeEvent *)
{
    newWidth=ui->boxMain->geometry().width();
    newHeight=ui->boxMain->geometry().height();

    QList<myDefenceButton *> btn=ui->boxMain->findChildren<myDefenceButton *>();
    foreach (myDefenceButton *b, btn) {
        int tempLeft=(int)(double)(b->geometry().left()*newWidth/oldWidth);
        int tempTop=(int)(double)(b->geometry().top()*newHeight/oldHeight);
        b->setGeometry(tempLeft,tempTop,35,35);
    }
    oldWidth=newWidth;
    oldHeight=newHeight;
}

void frmMain::InitForm()
{
#ifdef Q_OS_WIN//如果是windows系统
    //设置开机启动（只有windows系统才是这样设置开机启动）
    QString strPath=QApplication::applicationFilePath();
    strPath=strPath.replace("/","\\");
    if (myApp::AutoRun){ myHelper::AutoRunWithSystem(true,"AMS",strPath);}
    else{myHelper::AutoRunWithSystem(false,"AMS",strPath);}
#endif

    this->setWindowTitle(myApp::SoftTitle);

    labWelcom=new QLabel(QString("欢迎使用%1%2").arg(myApp::SoftTitle).arg(myApp::SoftVersion));
    ui->statusBar->addWidget(labWelcom);

    labUser=new QLabel(QString("当前用户:%1【%2】").arg(myApp::CurrentUserName).arg(myApp::CurrentUserType));
    ui->statusBar->addWidget(labUser);

    labLive=new QLabel(QString("已运行:0天0时0分0秒"));
    ui->statusBar->addWidget(labLive);

    labTime=new QLabel(QDateTime::currentDateTime().toString("当前时间:yyyy年MM月dd日 dddd HH:mm:ss"));
    ui->statusBar->addWidget(labTime);

    labBaoXiu=new QLabel(QString("报修电话:%1【%2】").arg(myApp::ContactTel).arg(myApp::ContactName));
    ui->statusBar->addWidget(labBaoXiu,1);

    connect(ui->action_config,SIGNAL(triggered()),this,SLOT(ShowForm()));
    connect(ui->action_host,SIGNAL(triggered()),this,SLOT(ShowForm()));
    connect(ui->action_defence,SIGNAL(triggered()),this,SLOT(ShowForm()));
    connect(ui->action_data,SIGNAL(triggered()),this,SLOT(ShowForm()));
    connect(ui->action_exit,SIGNAL(triggered()),this,SLOT(ShowForm()));

    //启动定时器计算当前时间和已运行时间
    timerDate=new QTimer(this);
    timerDate->setInterval(1000);
    connect(timerDate,SIGNAL(timeout()),this,SLOT(ShowDateTime()));
    timerDate->start();

    //延时1000毫秒执行备份数据库操作,执行一次
    QTimer::singleShot(1000,this,SLOT(BackupDatabase()));

    //从全局配置中读取是否启用运行限制
    QStringList data=myApp::KeyData.split("|");
    UseRun=data[2].toInt();
    RunTime=data[3].toInt();

    boxPanel=new QGroupBox;

    //2014-3-25增加对宝学报警主机的支持
    if (myApp::NETHostType=="BXHOST_NET"){
        ui->action_host->setText("站点管理");
    }

}

int day=0;
int hour=0;
int minute=0;
int second=0;
void frmMain::ShowDateTime()
{
    second++;
    if (second==60){minute++;second=0;}
    if (minute==60){hour++;minute=0;}
    if (hour==24){day++;hour=0;}

    labLive->setText(QString("已运行:%1天%2时%3分%4秒").arg(day).arg(hour).arg(minute).arg(second));
    labTime->setText(QDateTime::currentDateTime().toString("当前时间:yyyy年MM月dd日 dddd HH:mm:ss"));

    if (UseRun==1){
        if (hour==RunTime){
            myHelper::ShowMessageBoxError("运行时间到期,程序将自动关闭!");
            qApp->quit();//强制退出程序
        }
    }
}

void frmMain::BackupDatabase()
{
    if (myApp::AutoBank){
        QDate date=QDate::currentDate();
        int strYear=date.year();
        int strMonth=date.month();
        QString strBak="";

        if (myApp::TimeBank=="year"){
            strBak=QString("bak%1.db").arg(strYear);
        }else{
            strBak=QString("bak%1%2.db").arg(strYear).arg(strMonth);
        }

        //判断是否已经备份过数据库
        if (!myHelper::FileIsExist(myApp::AppPath+strBak)){
            this->labTime->setText("正在执行备份数据库操作......");
            myHelper::CopyFile(myApp::AppPath+"AMS.db",myApp::AppPath+strBak);
            QSqlQuery query;
            QString sql="delete from EventInfo;";//清空事件记录表
            query.exec(sql);
            myHelper::Sleep(300);
            sql="VACUUM;";//压缩数据库
            query.exec(sql);
            myAPI::AddEventInfoUser("备份数据库成功");
        }
    }
}

void frmMain::ShowForm()
{
    QAction *action=(QAction *)sender();
    QString triggerName=action->objectName();

    if (triggerName=="action_config"){
        frmConfig *config=new frmConfig;
        //按下了确定按钮,重新加载配置文件信息到状态栏和标题
        if (config->exec()==1){
            this->setWindowTitle(myApp::SoftTitle);
            this->labWelcom->setText(QString("欢迎使用%1%2").arg(myApp::SoftTitle).arg(myApp::SoftVersion));
            this->labUser->setText(QString("当前用户:%1【%2】").arg(myApp::CurrentUserName).arg(myApp::CurrentUserType));
            this->labBaoXiu->setText(QString("报修电话:%1【%2】").arg(myApp::ContactTel).arg(myApp::ContactName));
        }
    }else if(triggerName=="action_host"){
        frmHost *host=new frmHost;
        host->exec();
    }else if(triggerName=="action_defence"){
        frmDefence *defence=new frmDefence;
        defence->exec();
    }else if(triggerName=="action_data"){
        frmData *data=new frmData;
        data->exec();
    }else if(triggerName=="action_exit"){
        frmLogout *logout=new frmLogout;
        logout->exec();
    }
}

void frmMain::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString tempImage=item->text();
    if (myApp::CurrentImage!=tempImage){
        myApp::CurrentImage=tempImage;
        myAPI::InitDefenceCurrent();
    }
}
