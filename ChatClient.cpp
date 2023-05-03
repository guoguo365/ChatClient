#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET serverSocket;

// 接收服务器数据
void receiveMsg() {
	while (true)
	{
		char recvBuff[256];
		int r = recv(serverSocket, recvBuff, 255, NULL);
		if (r > 0) {

			// 添加结束符 \0
			recvBuff[r] = 0;
			printf(">>:%s\n", recvBuff);
		}
	}
	
}

int main() {

	// 1. 确定协议版本信息
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("确定协议版本信息错误：%d\n", GetLastError());
		return -1;
	}
	printf("确定协议版本信息成功！\n");

	// 2. 创建socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket) {
		printf("创建socket失败：%d\n", GetLastError());
		return -1;
	}
	printf("创建socket成功！\n");

	// 3. 确定服务器协议地址蹙
	SOCKADDR_IN sAddr = { 0 };
	sAddr.sin_family = AF_INET; // socket函数第一个参数
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.4.30.167");
	sAddr.sin_port = htons(9527);

	// 4. 连接服务器
	int r = connect(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (-1 == r) {

		// 8. 关闭socket
		closesocket(serverSocket);

		// 9. 清理协议版本信息
		WSACleanup();
		printf("连接服务器失败: %d\n", GetLastError());
		return -1;
	}
	printf("连接服务器端成功！\n");

	// 接收消息
	CreateThread(NULL, NULL,
		(LPTHREAD_START_ROUTINE) receiveMsg,
		NULL, NULL, NULL);
	

	// 5. 通信
	char buff[256];
	while (1)
	{
		printf("发啥子:");
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);
	}

	// 6. 关闭socket
	closesocket(serverSocket);

	// 7. 清理协议版本信息
	WSACleanup();

	return 0;
}