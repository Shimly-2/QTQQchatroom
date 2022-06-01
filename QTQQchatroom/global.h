#ifndef GLOBAL_H
#define GLOBAL_H

#include<QString>
#include"winsocket.h"

extern QString name;
extern WinSocket *mysocket;
extern QString ip;
extern int port;
extern char namelist[512];
extern int namelength;

#endif // GLOBAL_H
