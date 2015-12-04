#ifndef MYAPI_H
#define MYAPI_H

#include <QObject>
#include <QGroupBox>
#include <QTableWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QHeaderView>
#include "mydefencebutton.h"

/* 说明:核心处理模块头文件
 * 功能:加载所有防区主机信息,实现布防撤防等操作
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-27  检查:2014-1-10
 */
class myAPI : public QObject
{
    Q_OBJECT
public:
    explicit myAPI(QObject *parent = 0);

    //添加快捷提示信息
    static void AddMessage(QString message);

    //添加事件信息
    static void AddEventInfo(QString TriggerType,QString TriggerContent,QString DefenceID,
                             QString DefenceName,QString HostID,QString HostName,int SubSystem);

    static void AddEventInfoUser(QString TriggerContent);
    static void AddEventInfoHost(QString TriggerContent,QString HostID,QString HostName);

    //设置控件（必须最前面设置）
    static void SetControl(QGroupBox *BoxMain, QTableWidget *TableWidget,
                           QTreeWidget *TreeWidget, QListWidget *ListWidget,
                           QScrollArea *ScrollArea,QGroupBox *BoxPanel);

    //初始化所有防区
    static void InitDefenceAll(QGroupBox *boxMain);
    static void InitDefenceAll();

    //初始化当前地图对应防区
    static void InitDefenceCurrent(QGroupBox *boxMain);
    static void InitDefenceCurrent();    

    //初始化主机防区树状图及防区缩略图
    static void InitTreeWidget();
    static void InitListWidget();

    //初始化主机状态图
    static void InitHostInfo();

    //根据主机编号获取主机的名称/参数
    //2014-3-20改为从数据库中查询
    static QString GetHostName(QString hostID);    
    static QString GetHostConnectValue1(QString hostID);
    static QString GetHostConnectValue2(QString hostID);
    static QString GetDefenceID(int tempID);

    //2014-3-15增加根据主机地址和键盘地址查找对应主机ID(龙阅脉冲主机专用)
    static void GetHostID_Name(QString hostConnectValue1,QString hostConnectValue2,
                               QString &hostID,QString &hostName);
    static QString GetDefenceID(QString hostID,QString defenceIndex);
    static QString GetDefenceName(QString hostID,QString defenceID);
    static void GetHostConnectValue(QString hostID,
                                    QString &hostConnectValue1,QString &hostConnectValue2);

    static bool IsExistHost(QString hostID);

    //根据主机编号和防区号获取该防区对应的报警声音
    static QString GetDefenceSound(QString hostID,QString defenceID);

    //获取防区类型字符串和防区类型
    static QString GetDefenceType(DefenceType type);
    static DefenceType GetDefenceType(QString type);

    //主机上线下线及错误处理
    static void HostConnect(QString hostID,QString hostName);
    static void HostDisConnect(QString hostID,QString hostName);
    static void HostError(QString hostID,QString hostName,QString error);

    //2014-3-22增加脉冲主机高低压显示处理
    static void HostHigh(QString hostID);
    static void HostLow(QString hostID);

    //布防操作
    static void BuFang(QString hostID,QString hostName);
    static void BuFang(QString hostID,QString hostName,int subSystem);
    static void BuFang(QString hostID,QString hostName,QString defenceID);

    //撤防操作
    static void CheFang(QString hostID,QString hostName);
    static void CheFang(QString hostID,QString hostName,int subSystem);
    static void CheFang(QString hostID,QString hostName,QString defenceID);

    //旁路操作
    static void PangLu(QString hostID,QString hostName);
    static void PangLu(QString hostID,QString hostName,QString defenceID);

    //报警操作
    static void BaoJing(QString hostID,QString hostName,QString defenceID);    

    //主机防区断开和故障操作
    static void GuZhang(QString hostID,QString hostName,QString defenceID);

    //主机防区恢复和软件复位操作
    static void HuiFu(QString hostID,QString hostName,QString defenceID);
    static void HuiFuSoft(QString hostID,QString hostName,QString defenceID);

};

#endif // MYAPI_H
