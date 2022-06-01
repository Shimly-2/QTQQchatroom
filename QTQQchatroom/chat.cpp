/**
 *  chat class flie                                                           *
 *                                                                            *
 *  This file is part of our work.                                            *
 *                                                                            *
 *  This program is tu make the chat gui                                      *
 *                                                                            *
 *  @file     chat.cpp                                                        *
 *  @brief    chat gui                                                        *
 *  Details.                                                                  *
 *                                                                            *
 *  @author   JYY 1950049                                                     *
 *  @email    1950049@tongji.edu.cn                                           *
 *  @version  1.0.0.1                                                         *
 *  @date     2022/5/29                                                       *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *  Remark         : Description                                              *
 *----------------------------------------------------------------------------*
 *  Change History :                                                          *
 *  <Date>     | <Version> | <Author>       | <Description>                   *
 *----------------------------------------------------------------------------*
 *  2022/05/29 | 1.0.0.1   | jyy            | Create file                     *
 *----------------------------------------------------------------------------*
 *                                                                            *
 */

#include "chat.h"
#include "ui_chat.h"
#include<QDesktopWidget>
#include"winsocket.h"
#include<QThread>
#include <QtCore>
#include"global.h"
#include"QDateTime"

/**
 * @brief   Chat::Chat
 * @param   QWidget
 * @return  None
 * @note    构造函数
 */
Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    /* 初始化UI */
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    QFile file(":/myqss/scrollbar.qss");
    file.open(QFile::ReadOnly);
    ui->textBrowser->verticalScrollBar()->setStyleSheet(file.readAll());
    ui->label_8->installEventFilter(this);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height()-20)/2);

    /* 表情包发送函数关联 */
    emotion=new Emotion();
    connect(emotion,SIGNAL(imgurl(QString)),this,SLOT(sendemotion(QString)));
    emotion->move((QApplication::desktop()->width() - width())/2+200,(QApplication::desktop()->height() - height()-20)/2);

    /* 初始化好友列表 */
    ui->comboBox->addItem("server");
    char user[12]="\0";
    qDebug()<<"[Chat.cpp line70:]"<<namelength<<namelist;
    int userlen=0;
    /* 解析namelist，获取当前用户列表 */
    for(int i=0;i<namelength;i++)
    {
        if(namelist[i]!='|')
        {
            user[userlen]=namelist[i];
            userlen++;
        }
        if(namelist[i+1] == '|' || i == namelength-1)
        {
            i++;
            QString temp(user);
            qDebug()<<"[Talk.cpp line84:]"<<user;
            ui->comboBox->addItem(temp);
            for(int j=0;j<12;j++)
            {
                user[j]='\0';
            }
            userlen=0;
        }
    }

    /* 连接传递信号槽 */
    connect(mysocket,SIGNAL(isDone(QString)),this,SLOT(timerUpDate(QString)));

}

/**
 * @brief   Chat::~Chat
 * @param   None
 * @return  None
 * @note    析构函数
 */
Chat::~Chat()
{
    delete ui;
}
void Chat::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
         this->setMouseTracking(true);
          mouse_press = true;
          //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
          move_point = event->pos();
          qDebug()<<"移动1";
          qDebug()<<event->source();
     }

}

/**
 * @brief   Chat::mouseReleaseEvent
 * @param   QMouseEvent
 * @return  None
 * @note    释放鼠标动作
 */
void Chat::mouseReleaseEvent(QMouseEvent *event)
{
    //设置鼠标为未被按下
        mouse_press = false;

}

/**
 * @brief   Chat::mouseMoveEvent
 * @param   QMouseEvent
 * @return  None
 * @note    移动鼠标动作
 */
void Chat::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
  //  qDebug()<<"mouse_press="<<event->globalPos();
        if(mouse_press)
        {

            //鼠标相对于屏幕的位置
            QPoint move_pos = event->globalPos();
           //移动主窗体位置
           this->move(move_pos - move_point);
        }
}

/**
 * @brief   Chat::eventFilter
 * @param   QObject
 * @param   QEvent
 * @return  bool
 * @note    事件动作
 */
bool Chat::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label_5)
    {
         hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_4){
        showMinimized();
        qDebug()<<"最小化";
        qDebug()<<mouse_press;

    }
    /* 表情发送 */
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_8){

        qDebug()<<ui->label_8->x();
        qDebug()<<x();
        emotion->move(x()-10,ui->label_8->y()-100);
        emotion->show();

    }
    return false;
}

/**
 * @brief   Chat::showEvent
 * @param   QShowEvent
 * @return  None
 * @note    显示动作
 */
void Chat::showEvent(QShowEvent *event)
{
     qDebug()<<"出现了";
     mouse_press=false;
     qDebug()<<mouse_press;
}

/**
 * @brief   Chat::on_pushButton_2_clicked
 * @param   None
 * @return  None
 * @note    发送按钮
 */
void Chat::on_pushButton_2_clicked()
{
    /* 获取当前要发送的信息 */
    QString content=ui->textEdit->toPlainText();
    qDebug()<<content.length();

    /* 获取当前要发送的对象 */
    QString current = ui->comboBox->currentText();
    QString send = "\0";
    QString len=QString::number(content.length(), 10);
    qDebug()<<len;

    /* 完成发送协议 */
    send = "c" + current;
    int namelen=current.length();
    for(int i=namelen+1;i<=12;i++)
    {
        send =send + "!";
    }
    if(content.length()<10) send = send + "00" + len;
    else if(content.length()>=10 && content.length()<100) send = send + "0" + len;
    else send = send + len;
    send = send + content;
    char buff[512]="\0";
    char* ptr;
    QByteArray ba;
    ba = send.toLatin1();
    ptr = ba.data();
    strcpy_s(buff, ptr);//strcpy_s(a,b)将b拷贝给a保存到文件夹中
    qDebug()<<"[Chat.cpp line235 send info:]"<<buff;
    mysocket->sendData(buff);

    /* 将发送内容显示在gui中 */
    if(content.length()>=23){
        qDebug()<<"进入了这个";
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("hh:mm:ss");
        qDebug()<<current_date;
        QString s1="<img width='30' height='30' src='qrc:/10.JPG' style='float:right;margin-left:15px;margin-right:15px;'><div style=\"float:right;font-size:12px;font-family:'Microsoft YaHei';margin-left:220px;\">"+current_date+"</div><div style='background:rgb(18,183,245);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:60px;'><span style='font-size:25px;font-family:'Microsoft YaHei';margin-top:2px;'>"+content+"</span></div><br><br><br>";
        ui->textBrowser->insertHtml(s1);
    }
    else{
        int flag=60+(24-content.length())*15;
        QString myflag=QString::number(flag, 10);;
        qDebug()<<myflag;
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("hh:mm:ss");
        qDebug()<<current_date;
        QString s1="<img width='30' height='30' src='qrc:/10.JPG' style=\"float:right;margin-left:15px;margin-right:15px;\"><div style=\"float:right;font-size:12px;font-family:'Microsoft YaHei';margin-left:220px;\">"+current_date+"</div><div style=\"background:rgb(18,183,245);border-radius: 5px;padding-top:40px;padding-bottom:40px;float:right;height=80px;margin-top:8px;margin-right:45px;padding:50px;margin-left:"+myflag+"px;\">";
        s1+="<span style=\"font-size:25px;color：white;font-family:'Microsoft YaHei';line-height:150%;margin-top:40px;padding-top:40px;\">"+content+"</span></div><br clear=all><br clear=all><br clear=all>";
        qDebug()<<s1;
        ui->textBrowser->insertHtml(s1);
    }
    ui->textEdit->clear();
}

/**
 * @brief   Chat::sendemotion
 * @param   QString
 * @return  None
 * @note    发送表情
 */
void Chat::sendemotion(QString s)
{
    qDebug()<<"选择了发送表情";
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("hh:mm:ss");
    qDebug()<<current_date;
    QString s1="<img width='30' height='30' src='qrc:/10.JPG' style='float:right;margin-left:15px;margin-right:15px;'><div style=\"float:right;font-size:12px;font-family:'Microsoft YaHei';margin-left:220px;\">"+current_date+"</div><div style='background:rgb(18,183,245);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:387px;'><img src='"+s+"'></div><br><br><br>";
    ui->textBrowser->insertHtml(s1);
}

/**
 * @brief   Chat::timerUpDate
 * @param   QString
 * @return  None
 * @note    解析服务器的聊天信息
 */
void Chat::timerUpDate(QString content2)
{
    /* 获取更新用户列表 */
    char user[12]="\0";
    int len = 0;
    char chatting[512] = "\0";
    int chatlen = 0;
    char namelistnew[512]="\0";
    QString stingname(namelist);
    qDebug()<<"[Chat.cpp line289 get chatlist]:"<<content2;
    /* 将QString放入char中 */
    if(content2[0]=="t")
    {
        for(int i = 16;i<content2.length();i++)
        {
            namelistnew[len] = content2[i].toLatin1();
            len++;
            if(content2[i+1]=="$")break;
        }
        qDebug()<<"[Chat.cpp line299 namelistnew]:"<<namelistnew;
        int userlen=0;

        /* 解析更新用户列表 */
        for(int i=0;i<len;i++)
        {
            if(namelistnew[i]!='|')
            {
                user[userlen]=namelistnew[i];
                userlen++;
            }
            if(namelistnew[i+1] == '|' || i == len-1)
            {
                i++;
                QString temp(user);
                qDebug()<<"[Chat.cpp line314 campale chatlist]:"<<temp<<stingname<<stingname.contains(temp,Qt::CaseSensitive);
                if(!stingname.contains(temp,Qt::CaseSensitive))
                {
                    namelist[namelength++]='|';
                    for(int i = 0;i<userlen;i++)
                    {
                        namelist[namelength++] = user[i];
                    }
                    ui->comboBox->addItem(temp);
                }
                for(int j=0;j<12;j++)
                {
                    user[j]='\0';
                }
                userlen=0;
            }
        }
        qDebug()<<"[Chat.cpp line332 updata chatlist]:"<<namelist;
    }

    /* 获取聊天信息 */
    if(content2[0]=="c")
    {
        /* 获取聊天内容 */
        for(int i = 16;i<content2.length();i++)
        {
            chatting[chatlen] = content2[i].toLatin1();
            chatlen++;
            if(content2[i+1]=="$")break;
        }
        int flag=60+(24-chatlen)*15;
        QString mycontent(chatting);
        QString myflag=QString::number(flag, 10);;
        qDebug()<<chatlen;

        /* 显示在gui中 */
        if(chatlen != 0)
        {
            if(chatlen>=23){
                qDebug()<<"进入了这个";
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("hh:mm:ss");
                qDebug()<<current_date;
                QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-left:15px;margin-right:15px;'><div style=\"float:right;font-size:12px;font-family:'Microsoft YaHei';margin-left:220px;\">"+current_date+"</div><div style='background:rgb(229,229,229);padding-top:8px;float:left;margin-top:10px;margin-right:45px;padding:10px;margin-left:45px;'><span style='font-size:25px;font-family:'Microsoft YaHei';margin-top:2px;'>"+mycontent+"</span></div><br><br><br>";
                ui->textBrowser->insertHtml(s1);
            }
            else{
                QString myflag2="40";
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("hh:mm:ss");
                qDebug()<<current_date;
                QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-left:15px;margin-right:15px;'><div style=\"float:right;font-size:12px;font-family:'Microsoft YaHei';margin-left:220px;\">"+current_date+"</div><div style='background:rgb(229,229,229);padding-top:8px;float:left;margin-top:10px;margin-right:"+myflag+"px;padding:10px;margin-left:45px;'>";
                s1+="<span style='font-size:25px;font-family:'Microsoft YaHei';line-height:150%;margin-top:40px;padding-top:40px;'>"+mycontent+"</span></div><br><br><br>";
                qDebug()<<s1;
                ui->textBrowser->insertHtml(s1);
            }
        }
    }

}
