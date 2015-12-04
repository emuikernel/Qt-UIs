#ifndef FRMDEFENCE_H
#define FRMDEFENCE_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "mydefencebutton.h"

namespace Ui {
class frmDefence;
}

/* 说明:防区管理界面模块头文件
 * 功能:添加删除修改防区/调整防区位置等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class frmDefence : public QDialog
{
    Q_OBJECT

public:
    explicit frmDefence(QWidget *parent = 0);
    ~frmDefence();

protected:
    void closeEvent(QCloseEvent *);                 //重写关闭事件
    bool eventFilter(QObject *obj, QEvent *event);  //事件过滤器,识别按钮移动

private slots:
    void on_txtHostID_currentIndexChanged(int index);       //主机编号下拉选择改动处理    
    void on_txtDefenceImage_currentIndexChanged(const QString &arg1);//防区地图切换
    void on_btnAdd_clicked();       //添加防区
    void on_btnDelete_clicked();    //删除防区
    void on_btnUpdate_clicked();    //修改防区
    void on_btnMap_clicked();       //导入地图
    void on_btnMove_clicked();      //移动防区位置
    void on_treeDefence_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_txtDefenceSound_activated(const QString &arg1);

private:
    Ui::frmDefence *ui;
    myDefenceButton *btnTemp;       //临时防区对象
    bool IsAddOrUpdateDefence;      //用来处理当前鼠标按下是否显示临时防区
    QString tempDefenceID;          //临时防区号
    QString tempHostID;             //临时主机编号

    void InitForm();                //初始化界面

    QStringList TempDefenceIDHostID;//防区号链表,用来对比是否重复    
    void ReadDefenceInfo();         //加载所有防区

    QStringList TempHostID;         //主机编号链表
    QStringList TempHostName;       //主机名称链表
    void ReadHostInfo();            //加载主机信息
    void GetImageFile();            //加载地图目录下所有图片文件
    void GetSoundFile();            //加载声音目录下所有声音文件

    //校验防区信息
    bool IsExistDefence(QString DefenceID, QString HostID);
    bool CheckDefenceInfo(QString DefenceName,QString HostID,
                          QString DefenceImage,QString DefenceX,QString DefenceY);
    //修改防区位置坐标信息
    void UpdateDefenceInfo(QString DefenceID,QString HostID,int DefenceX,int DefenceY);
};

#endif // FRMDEFENCE_H
