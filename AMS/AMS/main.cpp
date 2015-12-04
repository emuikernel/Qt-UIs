#include <QApplication>
#include <QTranslator>
#include <QSharedMemory>
#include <QDesktopWidget>
#include "api/myhelper.h"
#include "api/myapp.h"
#include "api/myapi.h"
#include "frmmain.h"
#include "frmlogin.h"

/* 说明:主函数入口文件
 * 功能:打开数据库,初始化所有防区主机配置文件等信息
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("AMS");        //设置应用程序名称
    a.setApplicationVersion("V1.0");    //设置应用程序版本
    myHelper::SetUTF8Code();            //设置程序编码为UTF-8
    myHelper::SetStyle();               //设置应用程序样式

    QTranslator translator;             //加载中文字符
    translator.load(":/image/qt_zh_CN.qm");
    a.installTranslator(&translator);

    //赋值当前应用程序路径和桌面宽度高度
    myApp::AppPath=QApplication::applicationDirPath()+"/";
    myApp::DeskWidth=qApp->desktop()->availableGeometry().width();
    myApp::DeskHeigth=qApp->desktop()->availableGeometry().height()-40;

    //创建共享内存,判断是否已经运行程序
    QSharedMemory mem("SystemName");
    if(!mem.create(1)){
        myHelper::ShowMessageBoxError("程序已运行,软件将自动关闭!");
        return 1;
    }

    //判断注册码文件是否存在,并计算是否过期
    if (!myHelper::FileIsExist(myApp::AppPath+"key.dll")){
        myHelper::ShowMessageBoxError("注册码文件不存在或者损坏,程序将自动关闭！");
        return 1;
    }

    //读取注册码文件
    QFile key(myApp::AppPath+"key.dll");
    key.open(QFile::ReadOnly);
    myApp::KeyData=key.readLine();
    key.close();

    //将从注册码文件中的密文解密,与当前时间比较是否到期
    myApp::KeyData=myHelper::getXorEncryptDecrypt(myApp::KeyData,110);
    QStringList data=myApp::KeyData.split("|");
    if (data.count()!=4){
        myHelper::ShowMessageBoxError("注册码文件已损坏,程序将自动关闭!");
        return 1;
    }

    //如果启用了时间限制
    if (data[0]=="1"){
        QString nowDate=QDate::currentDate().toString("yyyy-MM-dd");
        if (nowDate>data[1]){
            myHelper::ShowMessageBoxError("软件已到期,请联系供应商更新注册码！");
            return 1;
        }
    }

    //判断当前数据库文件是否存在(如果数据库打开失败则终止应用程序)
    if (!myHelper::FileIsExist(myApp::AppPath+"AMS.db")){
        myHelper::ShowMessageBoxError("数据库文件不存在,程序将自动关闭！");
        return 1;
    }

    QSqlDatabase DbConn;
    DbConn=QSqlDatabase::addDatabase("QSQLITE");
    DbConn.setDatabaseName(myApp::AppPath+"AMS.db");

    //创建数据库连接并打开(如果数据库打开失败则终止应用程序)
    if (!DbConn.open()){
        myHelper::ShowMessageBoxError("打开数据库失败,程序将自动关闭！");
        return 1;
    }

    //程序加载时先加载所有配置信息
    myApp::ReadConfig();    

    //如果启用了自动登录,则打开主界面,并加载最后一次登录用户信息,未启用自动登录则加载登录界面
    if (myApp::AutoLogin){
        QSqlQuery query;
        query.exec("select [UserPwd],[UserType] from [UserInfo] where [UserName]='"+myApp::LastLoginer+"'");
        query.next();
        myApp::CurrentUserName=myApp::LastLoginer;
        myApp::CurrentUserPwd=query.value(0).toString();
        myApp::CurrentUserType=query.value(1).toString();
        myAPI::AddEventInfoUser("用户自动登录");

        frmMain w;
        w.showMaximized();//最大化显示主界面
        return a.exec();
    }else{
        frmLogin w;
        w.show();//显示登录界面
        return a.exec();
    }
}
