#ifndef WINSOCKET_H
#define WINSOCKET_H

//#include <WinSock2.h>
#include <QThread>
#include <winsock.h>
#include <winsock2.h>

class WinSocket : public QThread
{
    Q_OBJECT
public:
    explicit WinSocket();
    ~WinSocket();
    void init(QString localIp,int localPort);
    void sendData(char data[]);
//    int recvData(QString data);
private:
    SOCKET msocket;
    SOCKET newconnection;
    sockaddr_in localAddr;
    sockaddr_in recvAddr;
    sockaddr_in remoteAddr;
    int locallen;
    bool initSuccess;
    char vrecvData[500];
    char vsendData[500];
    void run();
signals:
    void sendRecvData(char *data,int length,QString recvIp);
    void isDone(QString data);
public slots:
    void dealRecvData(char *data,int length,QString recvIp);
};


#endif // WINSOCKET_H
