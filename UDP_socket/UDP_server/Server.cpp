/**
 *  This file is part of my work.                                             *
 *                                                                            *
 *  This program is tu make the UDP Server                                    *
 *                                                                            *
 *  @file     Server.cpp                                                      *
 *  @brief    UDP Server                                                      *
 *  Details.                                                                  *
 *                                                                            *
 *  @author   JYY 1950049                                                     *
 *  @email    1950049@tongji.edu.cn                                           *
 *  @gitee    https://gitee.com/jin-yiyang                                    *
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

#include <stdio.h>
#include <Winsock2.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")

void main()
{
	time_t t;
	struct tm* lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。

	/* 定义结构体变量 */
	WSADATA wsaData;
	int iErrorCode;

	/* 定义套接字版本，初始化套接字库 */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)) 
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}
	
	/* 创建套接字地址类型 */
	SOCKADDR_IN local;
	SOCKADDR_IN from;

	/* 地址结构体变量的大小 */
	int fromlen = sizeof(from);

	/* 设置ipv4，设置ip地址变量端口，ip地址 */
	local.sin_family = AF_INET;//IPv4 Socket
	local.sin_port = htons(8081); // sever Port
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); ///本机
	
	/* 定义套接字句柄 */
	SOCKET UDP_socket;

	/* 定义套接字类型 */
	UDP_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* 绑定对应的地址跟端口号 */
	bind(UDP_socket, (struct sockaddr*) & local, sizeof(local));

	printf("[%d:%d:%d]: Start creating UDP server...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
	while (1)
	{
		/* 接受数据缓冲区域 */
		char buffer[1024] = "\0";
		time(&t);//获取Unix时间戳。
		lt = localtime(&t);//转为时间结构。
		printf("[%d:%d:%d]: Waiting for message from client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
		
		/* 在这进行阻塞直到收到消息 */
		if (recvfrom(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & from, &fromlen) != SOCKET_ERROR)
		{
			time(&t);//获取Unix时间戳。
			lt = localtime(&t);//转为时间结构。
			printf("[%d:%d:%d]: Received data from %s, get %s \n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(from.sin_addr), buffer);
			for (int i = 0; i < strlen(buffer); i++)
			{
				buffer[i] = buffer[i] + 1;
			}
			////给cilent发信息
			sendto(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & from, fromlen);
			time(&t);//获取Unix时间戳。
			lt = localtime(&t);//转为时间结构。
			printf("[%d:%d:%d]: Canculate input data + 1, sending data... \n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(from.sin_addr), buffer);

		}
	}
	closesocket(UDP_socket);
	WSACleanup();
}