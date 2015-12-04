#ifndef MYHELPER_H
#define MYHELPER_H

#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QTime>
#include <QProcess>
#include <QDir>
#include <QApplication>
#include <QStyleFactory>
#include <QInputDialog>
#include <QStyle>
#include <QSettings>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QKeyEvent>
#include <QDebug>

/* 说明:全局辅助操作类实现文件
 * 功能:窗体显示/编码设置/开机启动/文件选择与复制/对话框等
 * 作者:刘典武  QQ:517216493
 * 时间:2013-12-30  检查:2014-1-10
 */
class myHelper:public QObject
{

public:

    //设置为开机启动
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
    {
        QSettings *reg=new QSettings(
                    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat);

        if (IsAutoRun){reg->setValue(AppName,AppPath);}
        else{reg->setValue(AppName,"");}
    }

    //设置编码为GB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);        
    }

    //设置编码为UTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);        
    }

    //设置应用程序样式
    static void SetStyle()
    {        
        QString qss="";
        qss+="QLineEdit{border:1px solid gray;border-radius:7px;padding:3px;}";
        qss+="QListWidget{border:1px solid gray;}";
        qss+="QListWidget::item{padding:0px;margin:3px;}";
        qss+="QTreeWidget{border:1px solid gray;}";
        qss+="QTreeWidget::item{padding:0px;margin:3px;}";
        qss+="QTableWidget{border:1px solid gray;}";
        qss+="QTableView{border:1px solid gray;}";
        qss+="QScrollArea{border:1px solid gray;}";        
        qApp->setStyleSheet(qss);
    }

    //判断是否是IP地址
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }

    //显示信息框,仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font: 12pt '宋体';");
        msg.setWindowTitle("提示");
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        msg.setCursor(QCursor(Qt::PointingHandCursor));
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.exec();
    }    

    //显示错误框,仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font: 12pt '宋体';");
        msg.setWindowTitle("错误");
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.setCursor(QCursor(Qt::PointingHandCursor));
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.exec();
    }    

    //显示询问框,确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font: 12pt '宋体';");
        msg.setWindowTitle("询问");
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);
        msg.setCursor(QCursor(Qt::PointingHandCursor));
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.addButton("取消",QMessageBox::RejectRole);

        return msg.exec();
    }    

    //显示标准输入框
    static QString ShowInputBox(QWidget *frm,QString info)
    {
        bool ok;
        return QInputDialog::getText(frm,"提示",info,QLineEdit::Password,"",&ok);
    }

    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; ) {
            hstr=str[i].toLatin1();
            if(hstr == ' ') {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2){
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    //16进制字符串转10进制
    static int StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //10进制字符串转10进制
    static int StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //2进制字符串转10进制
    static int StrBinToDecimal(QString strBin)
    {
        bool ok;
        return strBin.toInt(&ok,2);
    }

    //16进制字符串转2进制字符串
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<8){
            for (int i=0;i<8-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串一个字节
    static QString DecimalToStrBin1(int decimal)
    {
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<=8){
            for (int i=0;i<8-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串两个字节
    static QString DecimalToStrBin2(int decimal)
    {
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<=16){
            for (int i=0;i<16-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转16进制字符串,补零.
    static QString DecimalToStrHex(int decimal)
    {
        QString temp=QString::number(decimal,16);
        if (temp.length()==1){
            temp="0"+temp;
        }
        return temp;
    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }    

    //设置系统日期时间
    static void SetSystemDateTime(int year,int month,int day,int hour,int min,int sec)
    {
        QProcess p(0);

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();
    }

    //窗体居中,并且只有关闭按钮,不能调整大小
    static void FormOnlyCloseInCenter(QWidget *frm,int deskWidth,int deskHeigth)
    {        
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(deskWidth/2-frmX/2,deskHeigth/2-frmY/2);
        frm->move(movePoint);                          //窗体居中
        frm->setFixedSize(frmX,frmY);                  //窗体不能调整大小
        frm->setWindowFlags(frm->windowFlags() & Qt::WindowCloseButtonHint);//窗体只有关闭按钮
        frm->setAttribute(Qt::WA_DeleteOnClose);       //关闭时自动释放资源
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm,int deskWidth,int deskHeigth)
    {        
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(deskWidth/2-frmX/2,deskHeigth/2-frmY/2);
        frm->move(movePoint);
    }    

    //窗体不能改变大小
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }    

    //获取选择的文件
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(0,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的文件集合
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(0,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的目录
    static QString GetFolderName()
    {
        return QFileDialog::getExistingDirectory();;
    }

    //获取文件名,含拓展名
    static QString GetFileNameWithExtension(QString strFilePath)
    {
        QFileInfo fileInfo(strFilePath);
        return fileInfo.fileName();
    }

    //获取选择文件夹中的文件
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0){
            QDir myFolder(strFolder);
            if (myFolder.exists()){
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //文件夹是否存在
    static bool FolderIsExist(QString strFolder)
    {
        QDir tempFolder(strFolder);
        return tempFolder.exists();
    }

    //文件是否存在
    static bool FileIsExist(QString strFile)
    {
        QFile tempFile(strFile);
        return tempFile.exists();
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileIsExist(targetFile)){
            int ret=QMessageBox::information(0,"提示","文件已经存在,是否替换?",QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes){
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }

    //异或加密算法
    static QString getXorEncryptDecrypt(QString str, char key)
    {
        QByteArray data=str.toLatin1();
        int size=data.size();
        for(int i=0; i<size; i++){
            data[i] = data[i] ^ key;
        }
        return QLatin1String(data);
    }
};

#endif // MYHELPER_H
