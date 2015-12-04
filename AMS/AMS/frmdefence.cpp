#include "frmdefence.h"
#include "ui_frmdefence.h"
#include "api/myhelper.h"
#include "api/myapi.h"
#include "api/myapp.h"
#include <QtMultimedia/QSound>

/* 说明:防区管理界面模块实现文件
 * 功能:添加删除修改防区/调整防区位置等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
frmDefence::frmDefence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDefence)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);

    this->InitForm();                       //初始化界面
    this->GetImageFile();                   //加载所有地图文件
    this->GetSoundFile();                   //加载所有声音文件
    this->ReadHostInfo();                   //加载主机数据

    myAPI::InitDefenceAll(ui->gboxMain);    //加载防区数据到表格
    myAPI::InitDefenceCurrent(ui->gboxMain);//加载当前防区数据
    this->ReadDefenceInfo();                //加载防区数据

    this->IsAddOrUpdateDefence=false;       //当前是否处于添加和编辑防区模式,为真的话则防区可以移动
    ui->gboxMain->installEventFilter(this); //安装过滤器,监听鼠标按下
}

frmDefence::~frmDefence()
{
    delete ui;
}

bool frmDefence::eventFilter(QObject *obj, QEvent *event)
{
    if (obj==ui->gboxMain && this->IsAddOrUpdateDefence){
        if (event->type()==QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(event);
            if (mouseEvent->button()==Qt::LeftButton) {
                btnTemp->setVisible(true);
                btnTemp->move(mouseEvent->x(),mouseEvent->y());
                ui->txtDefenceX->setText(QString::number(mouseEvent->x()));
                ui->txtDefenceY->setText(QString::number(mouseEvent->y()));
                return true;
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmDefence::closeEvent(QCloseEvent *)
{
    myApp::IsMove=false;//防止在调整位置模式下关闭窗口回到主界面防区可以鼠标拖动
    if (myApp::CurrentUserType=="管理员"){
        myHelper::ShowMessageBoxInfo("如果更改过防区信息,请重新启动主程序!");
    }
}

void frmDefence::InitForm()
{
    //填充防区号
    for (int i=1;i<=500;i++){
        if (i<10){ui->txtDefenceID->addItem(QString("00%1").arg(i));continue;}
        if (i<100){ui->txtDefenceID->addItem(QString("0%1").arg(i));continue;}
        if (i<1000){ui->txtDefenceID->addItem(QString("%1").arg(i));continue;}
    }

    //填充子系统
    for (int i=1;i<=30;i++){
        ui->txtSubSystem->addItem(QString::number(i));
    }

    //填充防区类型
    QStringList DefenceType;
    DefenceType<<"周界防区"<<"入侵防区"<<"紧急防区"<<"消防防区"<<"无效防区";
    ui->txtDefenceType->addItems(DefenceType);
    ui->txtDefenceType->setCurrentIndex(1);

    if (myApp::CurrentUserType!="管理员"){
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
        ui->btnMap->setEnabled(false);
        ui->btnMove->setEnabled(false);
    }

    ui->treeDefence->setColumnCount(1); //设置列数
    ui->treeDefence->setColumnWidth(0,200);
    ui->treeDefence->header()->setVisible(false);//隐藏列标题
    ui->treeDefence->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编辑
}

void frmDefence::GetImageFile()
{
    ui->txtDefenceImage->clear();
    QDir imagePath(myApp::AppPath+"MAP");
    QStringList filter;
    filter<<"*.jpg"<<"*.bmp"<<"*.png";//过滤图片文件拓展名
    ui->txtDefenceImage->addItems(imagePath.entryList(filter));
}

void frmDefence::GetSoundFile()
{
    ui->txtDefenceSound->clear();
    QDir soundPath(myApp::AppPath+"SOUND");
    QStringList filter;
    filter<<"*.wav";//过滤声音文件拓展名
    ui->txtDefenceSound->addItems(soundPath.entryList(filter));
}

void frmDefence::ReadHostInfo()
{
    TempHostID.clear();
    TempHostName.clear();
    ui->txtHostID->clear();
    ui->txtHostName->clear();

    //填充主机编号
    QSqlQuery query;
    query.exec("select [HostID],[HostName] from [HostInfo]");
    while(query.next()){
        TempHostID<<query.value(0).toString();
        TempHostName<<query.value(1).toString();
    }

    ui->txtHostID->addItems(TempHostID);
}

void frmDefence::ReadDefenceInfo()
{    
    ui->treeDefence->setColumnWidth(0,250);
    TempDefenceIDHostID.clear();
    ui->treeDefence->clear();
    //逐个添加主机
    QSqlQuery queryHost;
    QString sqlHost="select [HostID],[HostName] from [HostInfo]";
    queryHost.exec(sqlHost);

    while(queryHost.next()) {
        QString tempHostID=queryHost.value(0).toString();
        QString tempHostName=queryHost.value(1).toString();

        QTreeWidgetItem *itemHost = new QTreeWidgetItem
                (ui->treeDefence,QStringList(tempHostName+"["+tempHostID+"]"));
        itemHost->setIcon(0,QIcon(":/image/tree1.png"));

        QSqlQuery queryDefence;
        QString sqlDefence=QString("select [DefenceID],[DefenceName] from [DefenceInfo] where [HostID]='%1'").arg(tempHostID);
        queryDefence.exec(sqlDefence);

        while(queryDefence.next()){
            QString tempDefenceIDX=queryDefence.value(0).toString();
            QString tempDefenceNameX=queryDefence.value(1).toString();
            //一个防区号和主机编号唯一性
            TempDefenceIDHostID<<QString("%1|%2").arg(tempDefenceIDX).arg(tempHostID);

            QTreeWidgetItem *itemDefence = new QTreeWidgetItem(itemHost,QStringList(QString(tempDefenceNameX+"["+tempDefenceIDX+"]")));
            itemDefence->setIcon(0,QIcon(":/image/tree2.png"));
            itemHost->addChild(itemDefence);
        }
    }
    ui->treeDefence->expandAll(); //结点全部展开
}

//判断该防区和主机是否已经存在
bool frmDefence::IsExistDefence(QString DefenceID,QString HostID)
{
    QString temp=QString("%1|%2").arg(DefenceID).arg(HostID);

    foreach (QString defence, TempDefenceIDHostID){
        if (defence==temp){
            myHelper::ShowMessageBoxError("同一主机同一防区号已经存在,请重新选择!");
            return true;
        }
    }
    return false;
}

//保存防区信息时,校验防区信息是否正确
bool frmDefence::CheckDefenceInfo(QString DefenceName,QString HostID,QString DefenceImage,QString DefenceX,QString DefenceY)
{
    if (DefenceName==""){
        myHelper::ShowMessageBoxError("防区名称不能为空,请重新填写!");
        ui->txtDefenceName->setFocus();
        return false;
    }

    if (HostID==""){
        myHelper::ShowMessageBoxError("主机编号不能为空,请在主机管理中添加好主机!");
        return false;
    }

    if (DefenceImage==""){
        myHelper::ShowMessageBoxError("防区地图不能为空,请先导入一张地图图片文件!");
        return false;
    }

    if (DefenceX=="" || DefenceY==""){
        myHelper::ShowMessageBoxError("防区坐标不能为空,请重新选择!");
        return false;
    }

    return true;
}

void frmDefence::on_txtHostID_currentIndexChanged(int index)
{
    ui->txtHostName->clear();
    ui->txtHostName->addItem(TempHostName[index]);
}

void frmDefence::on_txtDefenceImage_currentIndexChanged(const QString &arg1)
{
    myApp::CurrentImage=arg1;
    myAPI::InitDefenceCurrent(ui->gboxMain);
}

void frmDefence::on_btnAdd_clicked()
{
    if (ui->btnAdd->text()=="添加(&A)"){
        ui->btnDelete->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
        ui->btnMap->setEnabled(false);
        ui->btnMove->setEnabled(false);
        ui->treeDefence->setEnabled(false);

        ui->txtDefenceName->clear();
        ui->txtDefenceName->setFocus();
        ui->txtDefenceX->clear();
        ui->txtDefenceY->clear();
        ui->btnAdd->setText("保存(&S)");
        this->IsAddOrUpdateDefence=true;

        //增加临时防区按钮,鼠标按下时显示
        btnTemp=new myDefenceButton(ui->gboxMain);
        btnTemp->setGeometry(0,0,40,40);
        btnTemp->SetDefenceStatus(DefenceStatus_BuFang);
        btnTemp->setVisible(false);
    }else{
        QString defenceID=ui->txtDefenceID->currentText();
        QString defenceName=ui->txtDefenceName->text();
        QString defenceType=ui->txtDefenceType->currentText();
        QString subSystem=ui->txtSubSystem->currentText();
        QString hostID=ui->txtHostID->currentText();
        QString hostName=ui->txtHostName->currentText();
        QString defenceIndex=ui->txtDefenceIndex->currentText();
        QString defenceSound=ui->txtDefenceSound->currentText();
        QString defenceImage=ui->txtDefenceImage->currentText();
        QString defenceUser=ui->txtDefenceUser->currentText();
        QString defenceX=ui->txtDefenceX->text();
        QString defenceY=ui->txtDefenceY->text();

        if (!this->CheckDefenceInfo(defenceName,hostID,defenceImage,defenceX,defenceY)){return;}
        if (this->IsExistDefence(defenceID,hostID)){return;}

        QSqlQuery query;
        QString sql="insert into [DefenceInfo](";
        sql+= "[DefenceID],[DefenceName],[DefenceType],[SubSystem],";
        sql+="[HostID],[HostName],[DefenceIndex],[DefenceSound],";
        sql+="[DefenceImage],[DefenceUser],[DefenceX],[DefenceY])";
        sql+="values('"+defenceID+"','";
        sql+=defenceName+"','";
        sql+=defenceType+"','";
        sql+=subSystem+"','";
        sql+=hostID+"','";
        sql+=hostName+"','";
        sql+=defenceIndex+"','";
        sql+=defenceSound+"','";
        sql+=defenceImage+"','";
        sql+=defenceUser+"','";
        sql+=defenceX+"','";
        sql+=defenceY+"')";
        query.exec(sql);

        ui->btnDelete->setEnabled(true);
        ui->btnUpdate->setEnabled(true);
        ui->btnMap->setEnabled(true);
        ui->btnMove->setEnabled(true);
        ui->treeDefence->setEnabled(true);
        ui->btnAdd->setText("添加(&A)");
        this->IsAddOrUpdateDefence=false;

        //重新加载防区信息
        myAPI::InitDefenceAll(ui->gboxMain);
        myAPI::InitDefenceCurrent(ui->gboxMain);
        ui->txtDefenceID->setCurrentIndex(ui->txtDefenceID->currentIndex()+1);
        this->ReadDefenceInfo();
    }
}

void frmDefence::on_btnDelete_clicked()
{
    QString temp=ui->treeDefence->currentIndex().data().toString();
    if (temp==""){
        myHelper::ShowMessageBoxError("请选择要删除的防区!");
        return;
    }

    //存储防区号和主机编号，用来作为删除防区的where条件
    tempDefenceID=ui->txtDefenceID->currentText();
    tempHostID=ui->txtHostID->currentText();

    if (myHelper::ShowMessageBoxQuesion("确定要删除防区吗?")==0){
        QSqlQuery query;
        QString sql="delete from [DefenceInfo] where [DefenceID]='"+tempDefenceID
                +"' and [HostID]='"+tempHostID+"'";
        query.exec(sql);

        //重新加载防区信息
        myAPI::InitDefenceAll(ui->gboxMain);
        myAPI::InitDefenceCurrent(ui->gboxMain);
        this->ReadDefenceInfo();
    }
}


void frmDefence::on_btnUpdate_clicked()
{
    QString temp=ui->treeDefence->currentIndex().data().toString();
    if (temp==""){
        myHelper::ShowMessageBoxError("请选择要修改的防区!");
        return;
    }

    if (ui->btnUpdate->text()=="修改(&U)"){
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnMap->setEnabled(false);
        ui->btnMove->setEnabled(false);
        ui->treeDefence->setEnabled(false);
        ui->btnUpdate->setText("保存(&S)");
        this->IsAddOrUpdateDefence=true;

        //增加临时防区按钮,鼠标按下时显示
        btnTemp=new myDefenceButton(ui->gboxMain);
        btnTemp->setGeometry(0,0,40,40);
        btnTemp->SetDefenceStatus(DefenceStatus_BuFang);
        btnTemp->setVisible(false);

        //存储防区号和主机编号，用来作为修改防区的where条件
        tempDefenceID=ui->txtDefenceID->currentText();
        tempHostID=ui->txtHostID->currentText();
    }else{
        QString defenceID=ui->txtDefenceID->currentText();
        QString defenceName=ui->txtDefenceName->text();
        QString defenceType=ui->txtDefenceType->currentText();
        QString subSystem=ui->txtSubSystem->currentText();
        QString hostID=ui->txtHostID->currentText();
        QString hostName=ui->txtHostName->currentText();
        QString defenceIndex=ui->txtDefenceIndex->currentText();
        QString defenceSound=ui->txtDefenceSound->currentText();
        QString defenceImage=ui->txtDefenceImage->currentText();
        QString defenceUser=ui->txtDefenceUser->currentText();
        QString defenceX=ui->txtDefenceX->text();
        QString defenceY=ui->txtDefenceY->text();

        if (!this->CheckDefenceInfo(defenceName,hostID,defenceImage,defenceX,defenceY)){return;}

        QSqlQuery query;
        QString sql="update [DefenceInfo] set";
        sql+=" [DefenceID]='"+defenceID;
        sql+="',[DefenceName]='"+defenceName;
        sql+="',[DefenceType]='"+defenceType;
        sql+="',[SubSystem]='"+subSystem;
        sql+="',[HostID]='"+hostID;
        sql+="',[HostName]='"+hostName;
        sql+="',[DefenceIndex]='"+defenceIndex;
        sql+="',[DefenceSound]='"+defenceSound;
        sql+="',[DefenceImage]='"+defenceImage;
        sql+="',[DefenceUser]='"+defenceUser;
        sql+="',[DefenceX]='"+defenceX;
        sql+="',[DefenceY]='"+defenceY;
        sql+="' where [DefenceID]='"+tempDefenceID;
        sql+="' and [HostID]='"+tempHostID+"'";
        query.exec(sql);

        ui->btnAdd->setEnabled(true);
        ui->btnDelete->setEnabled(true);
        ui->btnMap->setEnabled(true);
        ui->btnMove->setEnabled(true);
        ui->treeDefence->setEnabled(true);
        ui->btnUpdate->setText("修改(&U)");
        this->IsAddOrUpdateDefence=false;

        //重新加载防区信息
        myAPI::InitDefenceAll(ui->gboxMain);
        myAPI::InitDefenceCurrent(ui->gboxMain);
        this->ReadDefenceInfo();
    }
}

void frmDefence::on_btnMap_clicked()
{
    QString fileName=myHelper::GetFileName("图片文件(*.jpg *.png *.bmp)");
    if (fileName==""){return;}

    bool ok= myHelper::CopyFile(fileName,myApp::AppPath+"MAP/"+myHelper::GetFileNameWithExtension(fileName));
    if (ok){
        this->GetImageFile();
        myHelper::ShowMessageBoxInfo("导入地图文件成功!");
    }else{
        myHelper::ShowMessageBoxError("导入地图文件失败!");
    }
}

void frmDefence::UpdateDefenceInfo(QString DefenceID, QString HostID, int DefenceX, int DefenceY)
{
    QSqlQuery query;
    query.exec("update [DefenceInfo] set [DefenceX]='"+QString::number(DefenceX)
               +"',[DefenceY]='"+QString::number(DefenceY)
               +"' where [DefenceID]='"+DefenceID
               +"' and [HostID]='"+HostID+"'");
}

void frmDefence::on_btnMove_clicked()
{
    if (ui->btnMove->text()=="调整防区位置(&M)"){
        myApp::IsMove=true;
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnMap->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
        ui->btnMove->setText("保存防区位置(&S)");
    }else{
        QList<myDefenceButton *> btn=ui->gboxMain->findChildren<myDefenceButton *>();
        foreach (myDefenceButton *b, btn){
            this->UpdateDefenceInfo(b->GetDefenceID(),b->GetHostID(),b->geometry().x(),b->geometry().y());
        }

        myApp::IsMove=false;
        ui->btnAdd->setEnabled(true);
        ui->btnDelete->setEnabled(true);
        ui->btnMap->setEnabled(true);
        ui->btnUpdate->setEnabled(true);
        ui->btnMove->setText("调整防区位置(&M)");

        //重新加载防区信息
        myAPI::InitDefenceAll(ui->gboxMain);
        myAPI::InitDefenceCurrent(ui->gboxMain);
        myHelper::ShowMessageBoxInfo("保存防区位置信息成功!");
    }
}

void frmDefence::on_treeDefence_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current==0 || current->parent()==0){return;}
    QString tempHostID=current->parent()->text(0).split("[")[1].split("]")[0];
    QString tempDefenceID=current->text(0).split("[")[1].split("]")[0];

    QSqlQuery query;
    QString sql=QString("select * from [DefenceInfo] where [DefenceID]='%1' and [HostID]='%2'")
            .arg(tempDefenceID).arg(tempHostID);
    query.exec(sql);
    while (query.next()){

        QString tempDefenceName=query.value(1).toString();
        QString tempDefenceType=query.value(2).toString();
        QString tempSubsystem=query.value(3).toString();

        QString tempHostName=query.value(5).toString();
        QString tempDefenceIndex=query.value(6).toString();
        QString tempDefenceSound=query.value(7).toString();
        QString tempDefenceImage=query.value(8).toString();
        QString tempDefenceUser=query.value(9).toString();
        QString tempDefenceX=query.value(10).toString();
        QString tempDefenceY=query.value(11).toString();

        ui->txtDefenceID->setCurrentIndex(ui->txtDefenceID->findText(tempDefenceID));
        ui->txtDefenceName->setText(tempDefenceName);
        ui->txtDefenceType->setCurrentIndex(ui->txtDefenceType->findText(tempDefenceType));
        ui->txtSubSystem->setCurrentIndex(ui->txtSubSystem->findText(tempSubsystem));
        ui->txtHostID->setCurrentIndex(ui->txtHostID->findText(tempHostID));
        //这里省去主机名称，这个会根据主机编号自动填充
        ui->txtDefenceIndex->setCurrentIndex(ui->txtDefenceIndex->findText(tempDefenceIndex));
        ui->txtDefenceSound->setCurrentIndex(ui->txtDefenceSound->findText(tempDefenceSound));
        ui->txtDefenceImage->setCurrentIndex(ui->txtDefenceImage->findText(tempDefenceImage));
        ui->txtDefenceUser->setCurrentIndex(ui->txtDefenceUser->findText(tempDefenceUser));
        ui->txtDefenceX->setText(tempDefenceX);
        ui->txtDefenceY->setText(tempDefenceY);

        QList<myDefenceButton *> btn=ui->gboxMain->findChildren<myDefenceButton *>();
        //在地图中找到该防区,显示为布防颜色,区别于其他防区
        foreach (myDefenceButton *b, btn) {
            if (b->GetDefenceID()==tempDefenceID && b->GetHostID()==tempHostID){
                b->SetDefenceStatus(DefenceStatus_BuFang);//设置为布防状态
            }else{
                b->SetDefenceStatus(DefenceStatus_GuZhang);//设置为故障状态
            }
        }
    }
}

void frmDefence::on_txtDefenceSound_activated(const QString &arg1)
{
    //2014-3-23增加下拉选择声音文件时自动播放该声音
    if (arg1.length()>0){
        QSound sound(QString("%1%2%3").arg(myApp::AppPath).arg("SOUND/").arg(arg1));
        sound.play();
    }
}
