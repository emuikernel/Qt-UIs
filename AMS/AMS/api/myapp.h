#ifndef MYAPP_H
#define MYAPP_H

#include <QString>

class myApp
{
public:
    static QString AppPath;             //应用程序路径
    static QString CurrentImage;        //当前防区对应地图
    static bool IsMove;                 //防区是否可以移动,在防区管理中当用户选择编辑和添加防区时为真
    static QString EmailContent;        //转发邮件正文
    static int DeskWidth;               //桌面宽度
    static int DeskHeigth;              //桌面高度
    static QString KeyData;             //注册码密文

    static QString CurrentUserName;     //当前用户名
    static QString CurrentUserPwd;      //当前用户密码
    static QString CurrentUserType;     //当前用户类型（值班员、管理员）

    static QString SoftTitle ;          //软件标题
    static QString SoftVersion;         //软件版本
    static QString CompanyName;         //开发商名称
    static QString ContactName;         //联系人
    static QString ContactTel;          //联系电话
    static QString LastLoginer;         //最后一次登录用户名,用来自动登录时需要加载的用户信息

    static bool UseServer;              //是否启用双向通信,DS7400主机的话为假
    static int ServerPort;              //启用远程控制的服务端口
    static bool UseSound;               //是否启用报警声音    
    static bool AwalysPlay;             //是否循环播放报警声音

    static bool AutoRun;                //是否开机启动,在windows下有用
    static bool AutoLogin;              //是否自动登录
    static bool AutoBank;               //是否自动备份数据库
    static QString TimeBank;            //自动备份数据的模式,每月还是每年
    
    static bool UseLinkMap;             //是否启用地图联动
    static QString LinkMapCOM;          //地图联动串口
    static bool UseLinkMsg;             //是否启用短信联动
    static QString LinkMsgCOM;          //短信联动串口
    static QString MsgTel;              //短信转发手机号码
    static QString MsgTemplet;          //短信模板

    static bool UseLinkNETSend;         //是否启用网络转发
    static QString LinkNETSendAddr1;    //转发地址1
    static QString LinkNETSendAddr2;    //转发地址2
    static QString LinkNETSendAddr3;    //转发地址3
    static bool UseLinkNETReceive;      //是否启用网络接收
    static int LinkNETReceivePort;      //网络接收端口

    static bool UsePopup;               //是否启用右下角弹窗
    static bool UseEmail;               //是否启用电子邮件转发
    static QString EmailAddr;           //发件人地址
    static QString EmailPwd;            //发件人密码
    static QString EmailSend;           //收件人地址

    static void ReadConfig();           //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig();          //写入配置文件,在更改配置文件、程序关闭时调用

    static QString NETHostType;         //2014-3-20增加宏泰主机与龙阅脉冲主机的判断

};

#endif // MYAPP_H
