#include "chatthread.h"
#include "winsocket.h"
#include<QtDebug>

ChatThread::ChatThread(QObject *parent)
    :QThread(parent)
{

}

ChatThread::~ChatThread()
{

}

void ChatThread::run()
{
    qDebug()<<"in thread";
     winsocket=new WinSocket();
     winsocket->init("127.0.0.1",8081);
//     tcpsocket->setSocketDescriptor(socketDesciptor);
     connect(winsocket,SIGNAL(isDone()),this,SLOT(handlmessage()));
}
void ChatThread::handlmessage()
{
    qDebug()<<"handlmessage";
    char buff2[1024]="\0";
    winsocket->recvData(buff2);
    QString content2(buff2);
    if(content2.length()!=0)
    {
        emit isDone();  //发送完成信号
    }
//        QByteArray qba= tcpsocket->readAll(); //读取
//        qDebug()<<qba;
//        QString ss=QVariant(qba).toString();
//        qDebug()<<ss;
}



//void ChatThread::run()
//{
//    //很复杂的数据处理
//    //需要耗时5秒
//    sleep(5);

//    emit isDone();  //发送完成信号
//}
