#include "frmlogout.h"
#include "ui_frmlogout.h"
#include "api/myhelper.h"
#include "api/myapp.h"
#include "api/myapi.h"

/* 说明:操作员退出界面模块实现文件
 * 功能:当前操作员退出处理
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
frmLogout::frmLogout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLogout)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    ui->labTitle->setText(myApp::SoftTitle);    

    //2014-3-29增加软键盘处理
    ui->gboxSoftKey->setGeometry(30,37,261,61);
    ui->gboxSoftKey->setVisible(false);

    QList<QPushButton *> btn=ui->gboxSoftKey->findChildren<QPushButton *>();
    foreach (QPushButton *b, btn){
        connect(b,SIGNAL(clicked()),this,SLOT(buttonClick()));
    }
}

frmLogout::~frmLogout()
{
    delete ui;
}

void frmLogout::buttonClick()
{
    QPushButton *btn=(QPushButton *)sender();
    QString objectName=btn->objectName();
    QString text=ui->txtUserPwd->text();

    if (objectName=="btnDelete"){
        if (text.length()>0){
            ui->txtUserPwd->setText(text.mid(0,text.length()-1));
        }
    }else if (objectName=="btnQ"){
        ui->txtUserPwd->setText(text+"Q");
    }else if (objectName=="btnW"){
        ui->txtUserPwd->setText(text+"W");
    }else if (objectName=="btnE"){
        ui->txtUserPwd->setText(text+"E");
    }else if (objectName=="btnR"){
        ui->txtUserPwd->setText(text+"R");
    }else if (objectName=="btnT"){
        ui->txtUserPwd->setText(text+"T");
    }else if (objectName=="btnY"){
        ui->txtUserPwd->setText(text+"Y");
    }else if (objectName=="btnU"){
        ui->txtUserPwd->setText(text+"U");
    }else if (objectName=="btnI"){
        ui->txtUserPwd->setText(text+"I");
    }else if (objectName=="btnO"){
        ui->txtUserPwd->setText(text+"O");
    }else if (objectName=="btnP"){
        ui->txtUserPwd->setText(text+"P");
    }else if (objectName=="btnA"){
        ui->txtUserPwd->setText(text+"A");
    }else if (objectName=="btnS"){
        ui->txtUserPwd->setText(text+"S");
    }else if (objectName=="btnD"){
        ui->txtUserPwd->setText(text+"D");
    }else if (objectName=="btnF"){
        ui->txtUserPwd->setText(text+"F");
    }else if (objectName=="btnG"){
        ui->txtUserPwd->setText(text+"G");
    }else if (objectName=="btnH"){
        ui->txtUserPwd->setText(text+"H");
    }else if (objectName=="btnJ"){
        ui->txtUserPwd->setText(text+"J");
    }else if (objectName=="btnK"){
        ui->txtUserPwd->setText(text+"K");
    }else if (objectName=="btnL"){
        ui->txtUserPwd->setText(text+"L");
    }else if (objectName=="btnZ"){
        ui->txtUserPwd->setText(text+"Z");
    }else if (objectName=="btnX"){
        ui->txtUserPwd->setText(text+"X");
    }else if (objectName=="btnC"){
        ui->txtUserPwd->setText(text+"C");
    }else if (objectName=="btnV"){
        ui->txtUserPwd->setText(text+"V");
    }else if (objectName=="btnB"){
        ui->txtUserPwd->setText(text+"B");
    }else if (objectName=="btnN"){
        ui->txtUserPwd->setText(text+"N");
    }else if (objectName=="btnM"){
        ui->txtUserPwd->setText(text+"M");
    }else if (objectName=="btn0"){
        ui->txtUserPwd->setText(text+"0");
    }else if (objectName=="btn1"){
        ui->txtUserPwd->setText(text+"1");
    }else if (objectName=="btn2"){
        ui->txtUserPwd->setText(text+"2");
    }else if (objectName=="btn3"){
        ui->txtUserPwd->setText(text+"3");
    }else if (objectName=="btn4"){
        ui->txtUserPwd->setText(text+"4");
    }else if (objectName=="btn5"){
        ui->txtUserPwd->setText(text+"5");
    }else if (objectName=="btn6"){
        ui->txtUserPwd->setText(text+"6");
    }else if (objectName=="btn7"){
        ui->txtUserPwd->setText(text+"7");
    }else if (objectName=="btn8"){
        ui->txtUserPwd->setText(text+"8");
    }else if (objectName=="btn9"){
        ui->txtUserPwd->setText(text+"9");
    }
}

bool frmLogout::eventFilter(QObject *obj, QEvent *event)
{
    //如果用户按下了回车键,则触发退出信号.
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
        if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter) {
            this->on_btnLogout_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmLogout::on_btnLogout_clicked()
{
    if (ui->txtUserPwd->text().toUpper()==myApp::CurrentUserPwd.toUpper()){
        myApp::WriteConfig();//保存配置
        myAPI::AddEventInfoUser("用户退出");
        qApp->quit();
    }else{
        myHelper::ShowMessageBoxError("密码错误,请重新输入!");
        ui->txtUserPwd->setFocus();
    }
}

void frmLogout::on_btnClose_clicked()
{
    this->close();
}

void frmLogout::on_btnKey_clicked()
{
    ui->gboxSoftKey->setVisible(!ui->gboxSoftKey->isVisible());
}
