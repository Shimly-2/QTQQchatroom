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
	time(&t);//��ȡUnixʱ�����
	lt = localtime(&t);//תΪʱ��ṹ��

	/* ����ṹ����� */
	WSAData wsaData;

	/* �����׽��ְ汾����ʼ���׽��ֿ� */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	printf("[%d:%d:%d]: Start creating TCP server...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);

	/* �����׽��ֵ�ַ���� */
	SOCKADDR_IN local; 
	SOCKADDR_IN from;

	/* ��ַ�ṹ������Ĵ�С */
	int locallen = sizeof(local);
	int fromlen = sizeof(from);

	/* ����ipv4������ip��ַ�����˿ڣ�ip��ַ */
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); //target PC
	local.sin_port = htons(8081); // sever Port
	local.sin_family = AF_INET; //IPv4 Socket

	/* �����׽��־�� */
	SOCKET TCP_socket; 

	/* �����׽������� */
	TCP_socket = socket(AF_INET, SOCK_STREAM, NULL);

	/* �󶨶�Ӧ�ĵ�ַ���˿ں� */
	bind(TCP_socket, (SOCKADDR*)&local, sizeof(local)); 

	/* �������� */
	listen(TCP_socket, SOMAXCONN); 

	/* ����accept */
	int input_num, output_num;
	SOCKET new_TCP_socket; //build a new socket do new connection. the TCP_socket is just listenning not used to exchange data
	new_TCP_socket = accept(TCP_socket, (SOCKADDR*)&local, &locallen); //newConnection is used to exchange data with client
	
	while (1)
	{
		char buffer[1024] = "\0";
		time(&t);//��ȡUnixʱ�����
		lt = localtime(&t);//תΪʱ��ṹ��
		printf("[%d:%d:%d]: Waiting for message from client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
		if (recvfrom(new_TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, &locallen) != SOCKET_ERROR)
		{
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
			printf("[%d:%d:%d]: Received data from %s, get %s \n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(local.sin_addr), buffer);
			for (int i = 0; i < strlen(buffer); i++)
			{
				buffer[i] = buffer[i] + 1;
			}
			////��cilent����Ϣ
			sendto(new_TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, locallen);
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
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