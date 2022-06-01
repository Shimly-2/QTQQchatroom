/**
 *  winsocket class flie                                                      *
 *                                                                            *
 *  This file is part of our work.                                            *
 *                                                                            *
 *  This program is tu use windows api to creter socket                       *
 *                                                                            *
 *  @file     winsocket.cpp                                                   *
 *  @brief    winsock2                                                        *
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

#include "winsocket.h"
#include <QMessageBox>
#include <QDebug>
#include <winsock2.h>
#include <winsock.h>

/**
 * @brief   WinSocket::WinSocket
 * @param   QWidget
 * @return  None
 * @note    构造函数
 */
WinSocket::WinSocket()
{
    initSuccess=false;
}

/**
 * @brief   WinSocket::~WinSocket
 * @param   QWidget
 * @return  None
 * @note    析构函数
 */
WinSocket::~WinSocket()
{
   if(initSuccess)
   {
       ::closesocket(msocket);
       ::WSACleanup();
       requestInterruption();//要求中断
       wait();//等待线程释放
   }
}

/**
 * @brief   WinSocket::init
 * @param   QWidget
 * @return  None
 * @note    初始化函数
 */
void WinSocket::init(QString localIp,int localPort)
{
    int retValue;
    /* 初始化网络环境 */
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 1);
    retValue=::WSAStartup(sockVersion, &wsaData);
    if(retValue!=0)
    {
        QMessageBox::information(NULL,"错误提示","Socket环境初始化失败");
        return;
    }
    /* 创建socket */
    msocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (msocket == INVALID_SOCKET)
    {
        QMessageBox::information(NULL,"错误提示","创建socket出错");
        ::WSACleanup();
        return;
    }

    int locallen = sizeof(localAddr);
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = inet_addr(localIp.toLatin1());
    localAddr.sin_port = ::htons(localPort);
    if (::connect(msocket, (SOCKADDR*)&localAddr, locallen) != 0) //Connection
    {
        MessageBoxA(NULL, "Blad Connection", "Error", MB_OK | MB_ICONERROR);
        return ;
    }

    //绑定函数
//    retValue= ::bind(msocket, (struct sockaddr*)&localAddr, sizeof(sockaddr_in));
//    if(retValue==-1)
//    {
//        QMessageBox::information(NULL,"错误提示","绑定IP和端口出错");
//        ::closesocket(msocket);
//        ::WSACleanup();
//        return;
//    }
//    //信号槽连接函数
//    connect(this,SIGNAL(sendRecvData(char *,int,QString)),this,SLOT(dealRecvData(char *,int,QString)));
//    //设置阻塞函数
    int imode = 0;
//    ::setsockopt(msocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
    ioctlsocket(msocket,FIONBIO,(u_long FAR*)&imode);

    int nNetTimeout=500;//ms
    ::setsockopt(msocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
    if(retValue==SOCKET_ERROR)
    {
        QMessageBox::information(NULL,"错误提示","阻塞函数设置失败");
        ::closesocket(msocket);
        ::WSACleanup();
        return;
    }

    //线程调用
    start();
    initSuccess=true;
}

/**
 * @brief   WinSocket::run
 * @param   None
 * @return  None
 * @note    线程运行函数，通过start间接调用
 */
void WinSocket::run()
{
    int length=0;
    int sockAddrSize=sizeof(sockaddr_in);
    while(false==this->isInterruptionRequested())
    {

       memset(vrecvData,0,500);
       length=0;
       length=::recvfrom(msocket, (char*)vrecvData,sizeof(vrecvData), 0, (struct sockaddr*)&recvAddr, &sockAddrSize);
//       qDebug()<<length;
       if(length!=-1)
       {
           memcpy(vsendData,vrecvData,length);
           QString data(vsendData);
           emit isDone(data);
//           emit sendRecvData(vsendData,length,::inet_ntoa(recvAddr.sin_addr));
       }
    }
}

/**
 * @brief   WinSocket::dealRecvData
 * @param   char    data[]
 * @param   int     length
 * @param   QString recvIp
 * @return  None
 * @note    处理recvdata
 */
void WinSocket::dealRecvData(char data[],int length,QString recvIp)
{
    qDebug()<<data<<" "<<length<<" "<<recvIp;
}

/**
 * @brief   WinSocket::sendData
 * @param   char   data[]
 * @return  None
 * @note    socket发送字符数组
 */
void WinSocket::sendData(char data[])
{
    if(initSuccess){
         qDebug()<<"socket sending";
        ::sendto(msocket,data,sizeof(data),NULL,(struct sockaddr*)&localAddr,locallen);
    }
}





