#include"global.h"
//#include<QString>

QString name = "\0";
/* 创建server连接全局变量 */
WinSocket *mysocket=new WinSocket();
QString ip = "127.0.0.1";
int port  = 8081;
char namelist[512]="\0";
int namelength = 0;
