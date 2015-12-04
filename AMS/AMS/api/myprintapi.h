#ifndef MYPRINTAPI_H
#define MYPRINTAPI_H

#include <QTextDocument>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QString>

/* 说明:打印辅助操作类实现文件
 * 功能:打印数据,导出数据到PDF
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class myPrintAPI
{
public:
    static void Print(QString title,QString columnNames[],
                      int columnWidths[],int columnCount,QStringList content)
    {
        QStringList html;
        int rowCount=content.count();//计算总行数

        //表格开始
        html.append("<table border='1' style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;' cellspacing='0' cellpadding='5'>");

        //标题占一行,居中显示
        html.append("<tr><td colspan='");
        html.append(QString::number(columnCount));
        html.append("' style='vertical-align:middle;'><p align='center'>");
        html.append(title);
        html.append("</p></td></tr>");

        //循环构建数据
        for(int i=0;i<rowCount;i++)
        {
            QStringList value=content[i].split(";");
            QString temp="";

            temp +="<tr>";

            //构建列
            temp+="<td width='"+QString::number(columnWidths[0])+"'>";
            temp+=columnNames[0]+":"+value[0];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[1])+"'>";
            temp+=columnNames[1]+":"+value[1];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[2])+"'>";
            temp+=columnNames[2]+":"+value[2];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[3])+"'>";
            temp+=columnNames[3]+":"+value[3];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[4])+"'>";
            temp+=columnNames[4]+":"+value[4];
            temp+="</td>";

            temp+="</tr>";
            html.append(temp);
        }

        html.append("</table>");

        //调用打印机打印
        QPrinter print;
        print.setOutputFormat(QPrinter::NativeFormat);
        print.setPageSize(QPrinter::A4);

        QTextDocument txt;
        txt.setHtml(html.join(""));
        txt.print(&print);
    }

    static void PrintPDF(QString fileName,QString title,QString columnNames[],
                         int columnWidths[],int columnCount,QStringList content)
    {
        QStringList html;
        int rowCount=content.count();//计算总行数

        //表格开始
        html.append("<table border='1' style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;' cellspacing='0' cellpadding='5'>");

        //标题占一行,居中显示
        html.append("<tr><td colspan='");
        html.append(QString::number(columnCount));
        html.append("' style='vertical-align:middle;'><p align='center'>");
        html.append(title);
        html.append("</p></td></tr>");

        //循环构建数据
        for(int i=0;i<rowCount;i++)
        {
            QStringList value=content[i].split(";");
            QString temp="";

            temp +="<tr>";

            //构建列
            temp+="<td width='"+QString::number(columnWidths[0])+"'>";
            temp+=columnNames[0]+":"+value[0];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[1])+"'>";
            temp+=columnNames[1]+":"+value[1];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[2])+"'>";
            temp+=columnNames[2]+":"+value[2];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[3])+"'>";
            temp+=columnNames[3]+":"+value[3];
            temp+="</td>";

            temp+="<td width='"+QString::number(columnWidths[4])+"'>";
            temp+=columnNames[4]+":"+value[4];
            temp+="</td>";

            temp+="</tr>";
            html.append(temp);
        }

        html.append("</table>");

        //调用打印机导出PDF
        QPrinter print;
        print.setOutputFileName(fileName);
        print.setOutputFormat(QPrinter::PdfFormat);
        print.setPageSize(QPrinter::A4);

        QTextDocument txt;
        txt.setHtml(html.join(""));
        txt.print(&print);
    }
};

#endif // MYPRINTAPI_H
