#ifndef FRMPOPUP_H
#define FRMPOPUP_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class frmPopup;
}

/* 说明:右下角弹出临时消息界面模块头文件
 * 功能:主机上线下线防区报警等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-25  检查:2014-1-10
 */
class frmPopup : public QWidget
{
    Q_OBJECT

public:
    explicit frmPopup(QWidget *parent = 0);
    ~frmPopup();

    //设置主机编号/主机名称/防区编号/事件内容
    void SetMessage(QString HostID,QString HostName,QString DefenceID,QString TriggerContent);

protected slots:    
    void paintEvent(QPaintEvent *);         //重写绘制事件
    void mouseMoveEvent(QMouseEvent *e);    //重写鼠标移动事件
    void mousePressEvent(QMouseEvent *e);   //重写鼠标按下事件

private slots:
    void on_btnReset_clicked();

private:
    Ui::frmPopup *ui;
    QPoint lastPos,tempPos,newPos;      //窗体移动时的临时坐标
    void drawWindowShadow(QPainter &p); //绘制边框

    QString hostID;
    QString hostName;
    QString defenceID;
    QString triggerContent;
};

#endif // frmPopup_H
