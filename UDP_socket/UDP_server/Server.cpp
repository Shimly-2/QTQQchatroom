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
	time(&t);//��ȡUnixʱ�����
	lt = localtime(&t);//תΪʱ��ṹ��

	/* ����ṹ����� */
	WSADATA wsaData;
	int iErrorCode;

	/* �����׽��ְ汾����ʼ���׽��ֿ� */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)) 
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}
	
	/* �����׽��ֵ�ַ���� */
	SOCKADDR_IN local;
	SOCKADDR_IN from;

	/* ��ַ�ṹ������Ĵ�С */
	int fromlen = sizeof(from);

	/* ����ipv4������ip��ַ�����˿ڣ�ip��ַ */
	local.sin_family = AF_INET;//IPv4 Socket
	local.sin_port = htons(8081); // sever Port
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); ///����
	
	/* �����׽��־�� */
	SOCKET UDP_socket;

	/* �����׽������� */
	UDP_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* �󶨶�Ӧ�ĵ�ַ���˿ں� */
	bind(UDP_socket, (struct sockaddr*) & local, sizeof(local));

	printf("[%d:%d:%d]: Start creating UDP server...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
	while (1)
	{
		/* �������ݻ������� */
		char buffer[1024] = "\0";
		time(&t);//��ȡUnixʱ�����
		lt = localtime(&t);//תΪʱ��ṹ��
		printf("[%d:%d:%d]: Waiting for message from client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
		
		/* �����������ֱ���յ���Ϣ */
		if (recvfrom(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & from, &fromlen) != SOCKET_ERROR)
		{
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
			printf("[%d:%d:%d]: Received data from %s, get %s \n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(from.sin_addr), buffer);
			for (int i = 0; i < strlen(buffer); i++)
			{
				buffer[i] = buffer[i] + 1;
			}
			////��cilent����Ϣ
			sendto(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & from, fromlen);
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
			printf("[%d:%d:%d]: Canculate input data + 1, sending data... \n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, inet_ntoa(from.sin_addr), buffer);

		}
	}
	closesocket(UDP_socket);
	WSACleanup();
}