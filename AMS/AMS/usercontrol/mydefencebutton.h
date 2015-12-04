#ifndef MYDEFENCEBUTTON_H
#define MYDEFENCEBUTTON_H

#include <QWidget>
#include <QTimer>

//防区类型  00-07
enum DefenceType
{
    DefenceType_zhoujie=0,    //周界防区
    DefenceType_ruqin=1,      //入侵防区
    DefenceType_jinji=2,      //紧急防区
    DefenceType_xiaofang=3,   //消防防区
    DefenceType_wuxiao=4      //无效防区
};

//防区状态  布防、撤防、报警、旁路、断开、故障
enum DefenceStatus
{
    DefenceStatus_BuFang=0,
    DefenceStatus_CheFang=1,
    DefenceStatus_BaoJing=2,
    DefenceStatus_PangLu=3,
    DefenceStatus_DuanKai=4,
    DefenceStatus_GuZhang=5
};

/* 说明:自定义报警按钮头文件
 * 功能:提供六种防区状态选择,附带防区号防区名等信息
 * 作者:刘典武  QQ:517216493
 * 时间:2014-1-2  检查:2014-1-10
 */
class myDefenceButton: public QWidget
{
    Q_OBJECT
public:

    explicit myDefenceButton(QWidget *parent=0);

    //构造函数,直接传入一个防区所需要的所有属性
    explicit myDefenceButton(QWidget *parent, QString defenceID, QString defenceName,
                             DefenceType defenceType, int subSystem, QString hostID, QString hostName, int defenceIndex, QString defenceSound,
                             QString defenceImage, QString defenceUser, int defenceX, int defenceY, DefenceStatus defenceStatus);


    void SetDefenceID(QString defenceID);                       //设置防区ID
    void SetDefenceStatus(DefenceStatus defenceStatus);         //设置防区状态,例如防区报警、防区旁路等
    QString GetDefenceID()const{return defenceID;}              //获取防区ID
    QString GetDefenceName()const{return defenceName;}          //获取防区名称
    DefenceType GetDefenceType()const{return defenceType;}      //获取防区类型
    int GetSubSystem()const{return subSystem;}                  //获取防区对应子系统
    QString GetHostID()const{return hostID;}                    //获取防区对应主机ID
    QString GetHostName()const{return hostName;}                //获取防区对应主机名称
    int GetDefenceIndex()const{return defenceIndex;}            //获取防区索引
    QString GetDefenceSound()const{return defenceSound;}        //获取防区报警声音
    QString GetDefenceImage()const {return defenceImage;}       //获取防区对应地图文件名称
    QString GetDefenceUser()const{return defenceUser;}          //获取防区用户
    int GetDefenceX()const{return defenceX;}                    //获取防区X坐标
    int GetDefenceY()const{return defenceY;}                    //获取防区Y坐标
    DefenceStatus GetDefenceStatus()const{return defenceStatus;}//获取防区状态

    //2014-3-21增加高压低压显示
    void SetHigh();
    void SetLow();

signals:

protected:
    void paintEvent(QPaintEvent *event);        //重写绘制函数
    bool eventFilter(QObject *, QEvent *);      //过滤鼠标按下事件

private slots:
    void BaoJingChange();           //执行报警改变操作

private:
    QString defenceID;      //防区号
    QString defenceName;    //防区名
    DefenceType defenceType;//防区类型  00--07
    int subSystem;          //防区对应子系统
    QString hostID;         //防区对应主机编号
    QString hostName;       //防区对应主机名称
    int defenceIndex;       //防区索引
    QString defenceSound;   //防区报警声音
    QString defenceImage;   //防区地图
    QString defenceUser;    //防区用户
    int defenceX;           //防区X坐标
    int defenceY;           //防区Y坐标
    DefenceStatus defenceStatus;//防区状态

    QRadialGradient *currentRG;
    bool IsRed;             //当前报警颜色是否为红色
    QTimer *myTimer;        //控制报警闪烁时钟

};

#endif //MYDEFENCEBUTTON_H
