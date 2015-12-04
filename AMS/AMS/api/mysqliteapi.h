#ifndef MYSQLITEAPI_H
#define MYSQLITEAPI_H

#include <QtSql>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QObject>
#include "myapp.h"

/* 说明:sqlite数据库操作头文件
 * 功能:用来对数据库进行查询等处理
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-19  检查:2014-1-10
 */
class mySqliteAPI : public QObject
{
    Q_OBJECT
public:
    explicit mySqliteAPI(QObject *parent = 0);

    //设置需要显示数据的表格,数据翻页对应的按钮
    void SetControl(QTableView *tableView,QLabel *labInfo,
                    QPushButton *btnFirst,QPushButton *btnPre,
                    QPushButton *btnNext,QPushButton *btnLast);

    //绑定数据,带行数和条件
    void BindData(QString tableName,int rowsCount,
                  QString columnNames[], int columnWidths[]);
    void BindDataSelect(QString tableName,QString where,int rowsCount,
                        QString columnNames[], int columnWidths[]);

    //绑定数据,带行数和条件和排序
    void BindData(QString tableName,QString orderColumn,QString orderType,int rowsCount,
                  QString columnNames[], int columnWidths[]);
    void BindDataSelect(QString tableName,QString orderColumn,
                        QString orderType,QString where,int rowsCount,
                        QString columnNames[], int columnWidths[]);

    //执行sql语句
    bool ExecuteSql(QString sql);

    //绑定数据到下拉框
    void BindData(QString columnName,QString tableName,QComboBox *cbox);

private:
    void BindData(QString sql);

private slots:
    void First();       //第一页
    void Previous();    //上一页
    void Next();        //下一页
    void Last();        //末一页
};

#endif // MYSQLITEAPI_H
