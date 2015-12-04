#ifndef FRMHOSTINFO_H
#define FRMHOSTINFO_H

#include <QWidget>

namespace Ui {
class frmHostInfo;
}

/* 说明:主机微型操作界面模块头文件
 * 功能:对单个主机进行布防撤防操作
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class frmHostInfo : public QWidget
{
    Q_OBJECT

public:
    explicit frmHostInfo(QWidget *parent = 0);
    ~frmHostInfo();

    void SetHostInfo(QString hostType,QString hostID,QString hostName,QString hostConnectValue1,QString hostConnectValue2);
    void SetOnLine(bool onLine);
    void SetStatus(QString status);

    QString GetHostType()const{return hostType;}
    QString GetHostID()const{return hostID;}
    QString GetHostName()const{return hostName;}
    QString GetHostConnectValue1()const{return hostConnectValue1;}
    QString GetHostConnectValue2()const{return hostConnectValue2;}

private slots:
    void on_btnBuFang_clicked();

    void on_btnCheFang_clicked();

    void on_btnHigh_clicked();

    void on_btnLow_clicked();

private:
    Ui::frmHostInfo *ui;

    QString hostType;
    QString hostID;
    QString hostName;
    QString hostConnectValue1;
    QString hostConnectValue2;
};

#endif // FRMHOSTINFO_H
