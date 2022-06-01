////UDP client

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
		MessageBoxA(NULL, "Winsock startup error", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}
	printf("[%d:%d:%d]: Start creating UDP client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
	
	/* �����׽��ֵ�ַ���� */
	SOCKADDR_IN server;

	/*��ַ�ṹ������Ĵ�С*/
	int len = sizeof(server);

	/* ����ipv4������ip��ַ�����˿ڣ�ip��ַ */
	server.sin_family = AF_INET;
	server.sin_port = htons(8081); ///server�ļ����˿�
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); ///server�ĵ�ַ
	
	/* �����׽��־�� */
	SOCKET UDP_socket;

	/* �����׽������� */
	UDP_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* ����server */
	if (connect(UDP_socket, (SOCKADDR*)& server, len) != 0) //TCP_socket
	{
		MessageBoxA(NULL, "Blad UDP_socket", "Error", MB_OK | MB_ICONERROR);
		return ;
	}

	while (1)
	{
		char buffer[1024] = "\0";
		time(&t);//��ȡUnixʱ�����
		lt = localtime(&t);//תΪʱ��ṹ��
		printf("[%d:%d:%d]: Please input message(input bye to quit): ", lt->tm_hour, lt->tm_min, lt->tm_sec);
		scanf("%s", buffer);
		if (strcmp(buffer, "bye") == 0)
		{
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
			printf("[%d:%d:%d]: UDP client quit\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
			Sleep(100);
			closesocket(UDP_socket);
			break;
		}
		if (sendto(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & server, len) != SOCKET_ERROR)
		{
			//printf("sending..\n");
			Sleep(100);
			if (recvfrom(UDP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & server, &len) != SOCKET_ERROR)
			{
				time(&t);//��ȡUnixʱ�����
				lt = localtime(&t);//תΪʱ��ṹ��
				printf("[%d:%d:%d]: Receive from UDP server: %s\n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, buffer);
			}
				
		}
	}
	closesocket(UDP_socket);
	WSACleanup();

}