#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET serverSocket;

// ���շ���������
void receiveMsg() {
	while (true)
	{
		char recvBuff[256];
		int r = recv(serverSocket, recvBuff, 255, NULL);
		if (r > 0) {

			// ��ӽ����� \0
			recvBuff[r] = 0;
			printf(">>:%s\n", recvBuff);
		}
	}
	
}

int main() {

	// 1. ȷ��Э��汾��Ϣ
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("ȷ��Э��汾��Ϣ����%d\n", GetLastError());
		return -1;
	}
	printf("ȷ��Э��汾��Ϣ�ɹ���\n");

	// 2. ����socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket) {
		printf("����socketʧ�ܣ�%d\n", GetLastError());
		return -1;
	}
	printf("����socket�ɹ���\n");

	// 3. ȷ��������Э���ַ��
	SOCKADDR_IN sAddr = { 0 };
	sAddr.sin_family = AF_INET; // socket������һ������
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.4.30.167");
	sAddr.sin_port = htons(9527);

	// 4. ���ӷ�����
	int r = connect(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (-1 == r) {

		// 8. �ر�socket
		closesocket(serverSocket);

		// 9. ����Э��汾��Ϣ
		WSACleanup();
		printf("���ӷ�����ʧ��: %d\n", GetLastError());
		return -1;
	}
	printf("���ӷ������˳ɹ���\n");

	// ������Ϣ
	CreateThread(NULL, NULL,
		(LPTHREAD_START_ROUTINE) receiveMsg,
		NULL, NULL, NULL);
	

	// 5. ͨ��
	char buff[256];
	while (1)
	{
		printf("��ɶ��:");
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);
	}

	// 6. �ر�socket
	closesocket(serverSocket);

	// 7. ����Э��汾��Ϣ
	WSACleanup();

	return 0;
}