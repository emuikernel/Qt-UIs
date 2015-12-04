#include "mysqliteapi.h"
#include <QHeaderView>
#include <QAbstractItemView>
#include "myhelper.h"

int StartIndex;             //分页开始索引,每次翻页都变动
QString TempSql;            //临时SQL语句
QString Sql;

int RowsCount;              //每页显示行数
int RowsCountAll;           //总行数
int PageCount;              //总页数
int PageCurrent;            //当前第几页

QTableView *TableView;      //显示数据的表格对象
QLabel *LabInfo;            //显示翻页信息
QPushButton *BtnFirst;      //第一页按钮对象
QPushButton *BtnPre;        //上一页按钮对象
QPushButton *BtnNext;       //下一页按钮对象
QPushButton *BtnLast;       //末一页按钮对象

QSqlQueryModel *QueryModel; //查询模型

/* 说明:sqlite数据库操作实现文件
 * 功能:用来对数据库进行查询等处理
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-19  检查:2014-1-10
 */
mySqliteAPI::mySqliteAPI(QObject *parent) :
    QObject(parent)
{    
    StartIndex=0;
    TempSql="";
    Sql="";
    RowsCount=0;
    RowsCountAll=0;
    PageCount=0;
    PageCurrent=1;
}

//设置显示数据的表格控件,当前翻页信息的标签控件等
void mySqliteAPI::SetControl(QTableView *tableView, QLabel *labInfo, QPushButton *btnFirst, QPushButton *btnPre, QPushButton *btnNext, QPushButton *btnLast)
{
    TableView=tableView;
    LabInfo=labInfo;
    BtnFirst=btnFirst;
    BtnPre=btnPre;
    BtnNext=btnNext;
    BtnLast=btnLast;
    QueryModel=new QSqlQueryModel(TableView);

    //挂载翻页按钮事件
    connect(BtnFirst,SIGNAL(clicked()),this,SLOT(First()));
    connect(BtnPre,SIGNAL(clicked()),this,SLOT(Previous()));
    connect(BtnNext,SIGNAL(clicked()),this,SLOT(Next()));
    connect(BtnLast,SIGNAL(clicked()),this,SLOT(Last()));
}

//执行指定SQL
bool mySqliteAPI::ExecuteSql(QString sql)
{
    QSqlQuery query;
    return query.exec(sql);
}

//绑定数据到下拉框
void mySqliteAPI::BindData(QString columnName, QString tableName, QComboBox *cbox)
{
    QSqlQuery query;
    query.exec("select "+columnName+" from "+tableName+" order by "+columnName+" asc");
    while(query.next()){
        cbox->addItem(query.value(0).toString());
    }
}

void mySqliteAPI::BindData(QString sql)
{       
    QueryModel->setQuery(sql);
    TableView->setModel(QueryModel);
    LabInfo->setText(QString("共有%1条数据  共%2页  当前第%3页").arg(RowsCountAll).arg(PageCount).arg(PageCurrent));
}

//分页绑定数据到表格
void mySqliteAPI::BindData( QString tableName, int rowsCount,QString columnNames[], int columnWidths[])
{
    StartIndex=0;//重置开始索引
    PageCurrent=1;
    RowsCount=rowsCount;

    //开始分页绑定数据前,计算好总数据量以及行数
    TempSql="select count(*) from "+tableName;
    QSqlQuery query;
    query.prepare(TempSql);
    query.exec();
    query.first();
    RowsCountAll= query.value(0).toInt();

    int yushu=RowsCountAll % RowsCount;
    //不存在余数,说明是整行,例如300%5==0
    if (yushu==0){
        if (RowsCountAll>0 && RowsCountAll<RowsCount){
            PageCount=1;
        }else{
            PageCount=RowsCountAll/RowsCount;
        }
    }else{
        PageCount=RowsCountAll/RowsCount+1;
    }

    TempSql="select * from "+tableName+" where 1=1";
    Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
    BindData(Sql);

    //依次设置列标题、列宽等
    for (int i=0;i<TableView->model()->columnCount();i++){
        QueryModel->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        TableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }


    TableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    TableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式为单行选中
    TableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    TableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(0,255,0,200)}"
                              "QTableView::item:selected{background-color:#008B00}");
}

//分页绑定数据到表格、排序方式
void mySqliteAPI::BindData( QString tableName,QString orderColumn,QString orderType, int rowsCount,QString columnNames[], int columnWidths[])
{
    StartIndex=0;//重置开始索引
    PageCurrent=1;
    RowsCount=rowsCount;

    //开始分页绑定数据前,计算好总数据量以及行数
    TempSql="select count(*) from "+tableName;
    QSqlQuery query;
    query.prepare(TempSql);
    query.exec();
    query.first();
    RowsCountAll= query.value(0).toInt();

    int yushu=RowsCountAll % RowsCount;
    //不存在余数,说明是整行,例如300%5==0
    if (yushu==0){
        if (RowsCountAll>0 && RowsCountAll<RowsCount){
            PageCount=1;
        }else{
            PageCount=RowsCountAll/RowsCount;
        }
    }else{
        PageCount=RowsCountAll/RowsCount+1;
    }

    TempSql="select * from "+tableName+" where 1=1 order by "+orderColumn+" "+orderType;
    Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
    BindData(Sql);

    //依次设置列标题、列宽等
    for (int i=0;i<TableView->model()->columnCount();i++){
        QueryModel->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        TableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }


    TableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    TableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式为单行选中
    TableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    TableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(0,255,0,50)}"
                              "QTableView::item:selected{background-color:#008B00}");
}

//分页绑定查询数据到表格
void mySqliteAPI::BindDataSelect(QString tableName, QString where, int rowsCount, QString columnNames[], int columnWidths[])
{
    StartIndex=0;//重置开始索引
    PageCurrent=1;
    RowsCount=rowsCount;

    //开始分页绑定数据前,计算好总数据量以及行数
    TempSql="select count(*) from "+tableName+" "+where;
    QSqlQuery query;
    query.prepare(TempSql);//分行写的时候注意空格不能少
    query.exec();
    query.first();
    RowsCountAll= query.value(0).toInt();

    int yushu=RowsCountAll % RowsCount;
    //不存在余数,说明是整行,例如300%5==0
    if (yushu==0){
        if (RowsCountAll>0 && RowsCountAll<RowsCount){
            PageCount=1;
        }else{
            PageCount=RowsCountAll/RowsCount;
        }
    }else{
        PageCount=RowsCountAll/RowsCount+1;
    }

    TempSql="select * from "+tableName+" "+where;
    Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
    BindData(Sql);

    //依次设置列标题、列宽等
    for (int i=0;i<TableView->model()->columnCount();i++){
        QueryModel->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        TableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }


    TableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    TableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式为单行选中
    TableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    TableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(0,255,0,50)}"
                              "QTableView::item:selected{background-color:#008B00}");
}

//分页绑定查询数据到表格、排序方式
void mySqliteAPI::BindDataSelect(QString tableName,QString orderColumn,QString orderType, QString where, int rowsCount, QString columnNames[], int columnWidths[])
{
    StartIndex=0;//重置开始索引
    PageCurrent=1;
    RowsCount=rowsCount;

    //开始分页绑定数据前,计算好总数据量以及行数
    TempSql="select count(*) from "+tableName+" "+where;
    QSqlQuery query;
    query.prepare(TempSql);//分行写的时候注意空格不能少
    query.exec();
    query.first();
    RowsCountAll= query.value(0).toInt();

    int yushu=RowsCountAll % RowsCount;
    //不存在余数,说明是整行,例如300%5==0
    if (yushu==0) {
        if (RowsCountAll>0 && RowsCountAll<RowsCount){
            PageCount=1;
        }else{
            PageCount=RowsCountAll/RowsCount;
        }
    }else{
        PageCount=RowsCountAll/RowsCount+1;
    }

    TempSql="select * from "+tableName+" "+where+" order by "+orderColumn+" "+orderType;
    Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
    BindData(Sql);

    //依次设置列标题、列宽等
    for (int i=0;i<TableView->model()->columnCount();i++){
        QueryModel->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        TableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }


    TableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    TableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式为单行选中
    TableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    TableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(0,255,0,50)}"
                              "QTableView::item:selected{background-color:#008B00}");
}

//第一页
void mySqliteAPI::First()
{
    if (PageCount>1){
        StartIndex=0;
        PageCurrent=1;
        Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
        BindData(Sql);
    }
}

//上一页
void mySqliteAPI::Previous()
{
    if (PageCurrent>1){
        PageCurrent--;
        StartIndex-=RowsCount;
        Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
        BindData(Sql);
    }
}

//下一页
void mySqliteAPI::Next()
{
    if (PageCurrent<PageCount){
        PageCurrent++;
        StartIndex+=RowsCount;
        Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
        BindData(Sql);
    }
}

//末一页
void mySqliteAPI::Last()
{
    if (PageCount>0){
        StartIndex=(PageCount-1)*RowsCount;
        PageCurrent=PageCount;
        Sql=QString("%1 limit %2,%3;").arg(TempSql).arg(StartIndex).arg(RowsCount);//组织分页SQL语句
        BindData(Sql);
    }
}
