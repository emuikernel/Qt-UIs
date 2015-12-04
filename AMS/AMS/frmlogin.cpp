#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "api/myhelper.h"
#include "api/myapi.h"
#include "api/myapp.h"
#include "frmmain.h"

/* 说明:操作员登录界面模块实现文件
 * 功能:当前操作员登录处理,将当前操作员写入配置信息
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
frmLogin::frmLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLogin)
{
    ui->setupUi(this);
    myHelper::FormInCenter(this,myApp::DeskWidth,myApp::DeskHeigth);
    myHelper::FormNotResize(this);
    this->InitForm();
}

frmLogin::~frmLogin()
{
    delete ui;
}

bool frmLogin::eventFilter(QObject *obj, QEvent *event)
{
    //用户按下回车键,触发登录信号.
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
        if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter) {
            this->on_btnLogin_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmLogin::InitForm()
{
    //加载用户信息链表到内存,当用户选择用户名和输入密码时,从内存中比较密码,速度极快.
    QString sql = "select [UserName],[UserPwd],[UserType] from [UserInfo]";
    QSqlQuery query;
    query.exec(sql);

    while(query.next()){
        ui->txtUserName->addItem(query.value(0).toString());
        //将对应用户的密码和类型存入链表,在用户登录时对比,比从数据库中查询速度要快.
        TempUserPwd<<query.value(1).toString();
        TempUserType<<query.value(2).toString();
    }

    //对应显示软件标题和工程商信息.
    ui->labTitle->setText(myApp::SoftTitle);
    ui->labCompany->setText(QString("技术支持:%1").arg(myApp::CompanyName));
    ui->txtUserName->setCurrentIndex(ui->txtUserName->findText(myApp::LastLoginer));    

    //2014-3-29增加软键盘处理
    ui->gboxSoftKey->setGeometry(30,76,261,61);
    ui->gboxSoftKey->setVisible(false);

    QList<QPushButton *> btn=ui->gboxSoftKey->findChildren<QPushButton *>();
    foreach (QPushButton *b, btn){
        connect(b,SIGNAL(clicked()),this,SLOT(buttonClick()));
    }
}

void frmLogin::buttonClick()
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

void frmLogin::on_btnLogin_clicked()
{
    QString UserPwd=ui->txtUserPwd->text();
    if (UserPwd==""){
        myHelper::ShowMessageBoxError("密码不能为空,请重新输入!");
        ui->txtUserPwd->setFocus();
        return;
    }

    int index=ui->txtUserName->currentIndex();
    if (UserPwd.toUpper()== TempUserPwd[index].toUpper()){
        //记录当前用户,写入到配置文件,下次启动时显示最后一次登录用户名.
        myApp::LastLoginer = ui->txtUserName->currentText();
        myApp::CurrentUserName = myApp::LastLoginer;
        myApp::CurrentUserPwd = TempUserPwd[index];
        myApp::CurrentUserType = TempUserType[index];

        myApp::WriteConfig();
        myAPI::AddEventInfoUser("用户登录");

        frmMain *frm=new frmMain;//实例化主界面对象.
        frm->showMaximized();//最大化窗体.
        this->hide();
    }else{
        myHelper::ShowMessageBoxError("密码错误,请重新输入!");
        ui->txtUserPwd->setFocus();
    }
}

void frmLogin::on_btnClose_clicked()
{
    qApp->quit();
}

void frmLogin::on_btnKey_clicked()
{
    ui->gboxSoftKey->setVisible(!ui->gboxSoftKey->isVisible());
}
