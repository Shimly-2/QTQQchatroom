/**
 *  This file is part of my work.                                             *
 *                                                                            *
 *  This program is tu make the TCP Server                                    *
 *                                                                            *
 *  @file     Server.cpp                                                      *
 *  @brief    TCP Server                                                      *
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

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <time.h>

int main()
{
	time_t t;
	struct tm* lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。

	/* 定义结构体变量 */
	WSAData wsaData;

	/* 定义套接字版本，初始化套接字库 */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	printf("[%d:%d:%d]: Start creating TCP server...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);

	/* 创建套接字地址类型 */
	SOCKADDR_IN local; 
	SOCKADDR_IN from;

	/* 地址结构体变量的大小 */
	int locallen = sizeof(local);
	int fromlen = sizeof(from);

	/* 设置ipv4，设置ip地址变量端口，ip地址 */
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); //target PC
	local.sin_port = htons(8081); // sever Port
	local.sin_family = AF_INET; //IPv4 Socket

	/* 定义套接字句柄 */
	SOCKET TCP_socket; 

	/* 定义套接字类型 */
	TCP_socket = socket(AF_INET, SOCK_STREAM, NULL);

	/* 绑定对应的地址跟端口号 */
	bind(TCP_socket, (SOCKADDR*)&local, sizeof(local)); 

	/* 开启监听 */
	listen(TCP_socket, SOMAXCONN); 

	/* 开启accept */
	int input_num, output_num;
	SOCKET new_TCP_socket; //build a new socket do new connection. the TCP_socket is just listenning not used to exchange data
	new_TCP_socket = accept(TCP_socket, (SOCKADDR*)&local, &locallen); //newConnection is used to exchange data with client
	
	while (1)
	{
		char buffer[1024] = "\0";
		time(&t);//获取Unix时间戳。
		lt = localtime(&t);//转为时间结构。
		printf("[%d:%d:%d]: Waiting for message from client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
		if (recvfrom(new_TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, &locallen) != SOCKET_ERROR)
		{
			time(&t);//获取Unix时间戳。
			lt = localtime(&t);//转为时间结构。
			printf("[%d:%d:%d]: Received data from %s, get %s \n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(local.sin_addr), buffer);
			for (int i = 0; i < strlen(buffer); i++)
			{
				buffer[i] = buffer[i] + 1;
			}
			////给cilent发信息
			sendto(new_TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, locallen);
			time(&t);//获取Unix时间戳。
			lt = localtime(&t);//转为时间结构。
			printf("[%d:%d:%d]: Canculate input data + 1, sending data... \n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(local.sin_addr), buffer);

		}
		//Sleep(1000);
	}
	//cleanup
	closesocket(TCP_socket);
	closesocket(new_TCP_socket);
	WSACleanup();
	return 0;
}