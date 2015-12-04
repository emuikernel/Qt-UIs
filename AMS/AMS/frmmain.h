#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QGroupBox>
#include <QListWidgetItem>

namespace Ui {
class frmMain;
}

/* 说明:主界面模块头文件
 * 功能:将防区主机数据加载到主界面,初始化工具栏状态栏等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

protected slots:
    void closeEvent(QCloseEvent *event);//重写关闭事件
    void resizeEvent(QResizeEvent *);//重写窗体大小改变事件,重新布置防区位置
    //bool eventFilter(QObject *obj, QEvent *event);//事件过滤器,用来过滤鼠标双击事件

private slots:    
    void ShowForm();                //显示对应界面
    void ShowDateTime();            //动态显示在线时长及当前日期时间
    void BackupDatabase();          //备份数据库    
    void on_listWidget_itemClicked(QListWidgetItem *item);//列表选择事件

private:
    Ui::frmMain *ui;

    QGroupBox *boxPanel;
    QLabel *labWelcom;  //欢迎标签
    QLabel *labUser;    //当前用户标签
    QLabel *labLive;    //已运行时间标签
    QLabel *labTime;    //当前时间标签
    QLabel *labBaoXiu;  //报修电话标签
    QTimer *timerDate;  //计算在线时长和显示当前时间定时器对象   

    void InitForm();    //初始化界面    
    int UseRun;         //是否启用运行限制
    int RunTime;        //运行时间限制

};

#endif // FRMMAIN_H
