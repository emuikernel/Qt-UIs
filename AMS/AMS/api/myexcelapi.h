#ifndef MYEXCELAPI_H
#define MYEXCELAPI_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QStringList>

/* 说明:excel操作模块实现文件
 * 功能:写入excel文件
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class myExcelAPI
{
public:
    static void ToExcel(QString fileName,QString sheetName,
                        QString title,QString columnNames[],
                        int columnWidths[],int columnCount,QStringList content)
    {
        //逐个拼接xml字符,再保存为xls文件
        QStringList html;

        //头部固定信息
        html.append("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
        html.append("<?mso-application progid=\"Excel.Sheet\"?>");
        html.append("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"");
        html.append("xmlns:o=\"urn:schemas-microsoft-com:office:office\"");
        html.append("xmlns:x=\"urn:schemas-microsoft-com:office:excel\"");
        html.append("xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"");
        html.append("xmlns:html=\"http://www.w3.org/TR/REC-html40\"");
        html.append("xmlns:dt=\"uuid:C2F41010-65B3-11d1-A29F-00AA00C14882\">");

        //文件属性
        html.append("<DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">");
        html.append("<Author>administrator</Author>");
        html.append("<Created>"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss dddd")+"</Created>");
        html.append("</DocumentProperties>");

        //文档属性
        html.append("<CustomDocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">");
        html.append("<KSOProductBuildVer dt:dt=\"string\">2052-9.1.0.4180</KSOProductBuildVer>");
        html.append("</CustomDocumentProperties>");

        //Excel工作薄属性
        html.append("<ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">");
        html.append("<WindowWidth>21500</WindowWidth>");
        html.append("<WindowHeight>10480</WindowHeight>");
        html.append("<ProtectStructure>False</ProtectStructure>");
        html.append("<ProtectWindows>False</ProtectWindows>");
        html.append("</ExcelWorkbook>");

        //固定样式
        html.append("<Styles><Style ss:ID=\"s5\" ss:Name=\"货币[0]\">");
        html.append("<NumberFormat ss:Format=\"_ &quot;￥&quot;* #,##0_ ;_ &quot;￥&quot;* \\-#,##0_ ;_ &quot;￥&quot;* &quot;-&quot;_ ;_ @_ \"/></Style>");

        html.append("<Style ss:ID=\"s4\" ss:Name=\"百分比\">");
        html.append("<NumberFormat ss:Format=\"0%\"/></Style>");

        html.append("<Style ss:ID=\"s3\" ss:Name=\"千位分隔[0]\">");
        html.append("<NumberFormat ss:Format=\"_ * #,##0_ ;_ * \\-#,##0_ ;_ * &quot;-&quot;_ ;_ @_ \"/></Style>");

        html.append("<Style ss:ID=\"s2\" ss:Name=\"货币\">");
        html.append("<NumberFormat ss:Format=\"_ &quot;￥&quot;* #,##0.00_ ;_ &quot;￥&quot;* \\-#,##0.00_ ;_ &quot;￥&quot;* &quot;-&quot;??_ ;_ @_ \"/></Style>");

        html.append("<Style ss:ID=\"s1\" ss:Name=\"千位分隔\">");
        html.append("<NumberFormat ss:Format=\"_ * #,##0.00_ ;_ * \\-#,##0.00_ ;_ * &quot;-&quot;??_ ;_ @_ \"/></Style>");

        //用户默认样式
        html.append("<Style ss:ID=\"Default\" ss:Name=\"Normal\">");
        html.append("<Alignment/><Borders/><Font ss:FontName=\"宋体\" x:CharSet=\"134\" ss:Size=\"12\"/>");
        html.append("<Interior/><NumberFormat/><Protection/></Style>");

        //数据内容样式
        html.append("<Style ss:ID=\"Content\"><Alignment/>");
        html.append("<Borders><Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/></Borders>");
        html.append("<Font ss:FontName=\"宋体\" x:CharSet=\"134\" ss:Size=\"12\"/>");
        html.append("<Interior/><NumberFormat/><Protection/></Style>");

        //字段名样式
        html.append("<Style ss:ID=\"ColumnName\"><Alignment ss:Horizontal=\"Center\"/>");
        html.append("<Borders><Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/></Borders>");
        html.append("<Font ss:FontName=\"宋体\" x:CharSet=\"134\" ss:Size=\"12\"/>");
        html.append("<Interior/><NumberFormat/><Protection/></Style>");

        //表格标题样式
        html.append("<Style ss:ID=\"Title\"><Alignment ss:Horizontal=\"Center\"/>");
        html.append("<Borders><Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>");
        html.append("<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/></Borders>");
        html.append("<Font ss:FontName=\"宋体\" x:CharSet=\"134\" ss:Size=\"14\" ss:Bold=\"1\"/>");
        html.append("<Interior/><NumberFormat/><Protection/></Style>");

        //样式表结束
        html.append("</Styles>");

        //工作表开始
        html.append("<Worksheet ss:Name=\""+sheetName+"\">");

        int rowCount=content.count();//计算总行数

        //表格开始
        html.append("<Table ss:ExpandedColumnCount=\""+QString::number(columnCount)+"\"");
        html.append(" ss:ExpandedRowCount=\""+QString::number(rowCount)+"\"");
        html.append(" x:FullColumns=\"1\" x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"18\">");

        for (int i=0;i<columnCount;i++){
            html.append("<Column ss:StyleID=\"Default\" ss:AutoFitWidth=\"0\" ss:Width=\""+QString::number(columnWidths[i])+"\"/>");
        }

        //单元格数据
        //标题
        html.append("<Row ss:Height=\"25\"><Cell ss:StyleID=\"Title\" ss:MergeAcross=\""+QString::number(columnCount-1)+"\">");
        html.append("<Data ss:Type=\"String\">"+title);
        html.append("</Data></Cell></Row>");

        //逐个添加字段名
        html.append("<Row>");
        for (int i=0;i<columnCount;i++){
            html.append("<Cell ss:StyleID=\"ColumnName\"><Data ss:Type=\"String\">"+columnNames[i]+"</Data></Cell>");
        }
        html.append("</Row>");

        //逐个添加数据
        for (int i=0;i<rowCount;i++){
            html.append("<Row>");
            QString temp=content[i];
            QStringList value=temp.split(";");
            foreach (QString str, value){
                html.append("<Cell ss:StyleID=\"Content\"><Data ss:Type=\"String\">"+str+"</Data></Cell>");
            }
            html.append("</Row>");
        }

        //固定结尾格式
        html.append("</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">");
        html.append("<PageSetup><Header x:Margin=\"0.511805555555556\"/>");
        html.append("<Footer x:Margin=\"0.511805555555556\"/></PageSetup>");

        html.append("<Selected/><TopRowVisible>0</TopRowVisible><LeftColumnVisible>0</LeftColumnVisible>");
        html.append("<PageBreakZoom>100</PageBreakZoom><Panes><Pane><Number>3</Number><ActiveRow>1</ActiveRow>");
        html.append("<ActiveCol>2</ActiveCol><RangeSelection>R2C3</RangeSelection></Pane></Panes>");
        html.append("<ProtectObjects>False</ProtectObjects><ProtectScenarios>False</ProtectScenarios></WorksheetOptions></Worksheet>");

        html.append("</Workbook>");

        //写入文件
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QIODevice::Text)){
            QTextStream text(&file);
            text.setCodec("UTF-8");
            text<<html.join("");
        }
    }
};

#endif // MYEXCELAPI_H
