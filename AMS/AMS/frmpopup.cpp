#include "frmpopup.h"
#include "ui_frmpopup.h"
#include "api/myapp.h"
#include "api/myapi.h"

/* 说明:右下角弹出临时消息界面模块实现文件
 * 功能:主机上线下线防区报警等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
frmPopup::frmPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmPopup)
{
    ui->setupUi(this);      
    int frmWidth=420;
    int frmHeight=190;

    //设置窗体不能改变大小
    this->setFixedSize(frmWidth,frmHeight);
    //设置窗体最前端显示,且无边框,在任务栏不显示
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint
                         | Qt::Tool | Qt::X11BypassWindowManagerHint);
    //设置窗体背景透明,关闭时自动释放资源
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(myApp::DeskWidth-frmWidth,myApp::DeskHeigth-frmHeight+40,frmWidth,frmHeight);
}

frmPopup::~frmPopup()
{    
    delete ui;
}

void frmPopup::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    drawWindowShadow(p);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawRoundedRect(QRect(18,18,this->width() - 36,this->height() - 36),2.0f,2.0f);
}

void frmPopup::drawWindowShadow(QPainter &p)
{
    QList<QPixmap> pixmaps;
    pixmaps.append(QPixmap(":/image/shadow_left.png"));
    pixmaps.append(QPixmap(":/image/shadow_right.png"));
    pixmaps.append(QPixmap(":/image/shadow_top.png"));
    pixmaps.append(QPixmap(":/image/shadow_bottom.png"));
    pixmaps.append(QPixmap(":/image/shadow_corner1.png"));
    pixmaps.append(QPixmap(":/image/shadow_corner2.png"));
    pixmaps.append(QPixmap(":/image/shadow_corner3.png"));
    pixmaps.append(QPixmap(":/image/shadow_corner4.png"));

    p.drawPixmap(0,0,20,20,pixmaps[4]);
    p.drawPixmap(this->width() - 20,0,20,20,pixmaps[5]);
    p.drawPixmap(0,this->height() - 20,20,20,pixmaps[6]);
    p.drawPixmap(this->width() - 20,this->height() - 20,20,20,pixmaps[7]);

    p.drawPixmap(0,20,20,this->height() - 40,pixmaps[0].scaled(20,this->height() - 40));
    p.drawPixmap(this->width() - 20,20,20,this->height() - 40,pixmaps[1].scaled(20,this->height() - 40));
    p.drawPixmap(20,0,this->width() - 40,20,pixmaps[2].scaled(this->width() - 40,20));
    p.drawPixmap(20,this->height() - 20,this->width() - 40,20,pixmaps[3].scaled(this->width() - 40,20));
}

void frmPopup::mouseMoveEvent(QMouseEvent *e)
{
    newPos=e->globalPos();
    move(tempPos+newPos-lastPos);
}

void frmPopup::mousePressEvent(QMouseEvent *e)
{
    lastPos=e->globalPos();
    tempPos=lastPos-e->pos();
}

void frmPopup::SetMessage(QString HostID, QString HostName,QString DefenceID, QString TriggerContent)
{
    this->hostID=HostID;
    this->hostName=HostName;
    this->defenceID=DefenceID;
    this->triggerContent=TriggerContent;

    QString str=triggerContent;
    if (TriggerContent=="防区报警"){
        ui->btnReset->setText("软件复位");
        str=QString("%1%2").arg(myAPI::GetDefenceName(HostID,DefenceID)).arg("报警");
    }else{ui->btnReset->setText("关闭");}

    ui->labTriggerContent->setText(QString("事件:%1").arg(str));
    ui->labHostName->setText(QString("主机:%1").arg(HostName));
}

void frmPopup::on_btnReset_clicked()
{
    if (ui->btnReset->text()=="软件复位"){
        myAPI::HuiFuSoft(hostID,hostName,defenceID);
    }
    this->close();
}
