#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <WinSock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

static int isThread = 1;
static int client_sock;
static char buf2[255];

unsigned __stdcall ThreadFunsion(void* arg) {
	while (isThread) {
		if (recv(client_sock, buf2, 255, 0) <= 0) exit(0);

		if (strcmp(buf2, "pass\n") == 0) {
			printf("\n----------------------------\n");
			printf("  GATE = OPEN \n");
			printf("  게이트가 열렸습니다.\n");
			printf("----------------------------\n\n\n\n");

			printf("  * 차량 번호를 입력하세요 : ");
		}
		else if (strcmp(buf2, "deny\n") == 0) {
			printf("\n----------------------------\n");
			printf("  GATE = CLOSE \n");
			printf("  등록되지 않은 차량입니다..\n");
			printf("----------------------------\n\n\n\n");

			printf("  * 차량 번호를 입력하세요 : ");
		}
		else if (strcmp(buf2, "exit\n") == 0) {
			isThread = 0;
			exit(0);
		}
	}
	return 0;
}

void main() {
	int server_sock;
	int client_len;
	WSADATA date;
	::WSAStartup(WINSOCK_VERSION, &date);
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	char buf[255];

	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr("192.168.35.168");
	clientaddr.sin_port = htons(23000);
	client_len = sizeof(clientaddr);

	if (connect(client_sock, (struct sockaddr*)&clientaddr, client_len) == SOCKET_ERROR) {
		printf("connect error\n");
		exit(0);
	}
	printf("Connected !!\n");

	isThread = 1;
	_beginthreadex(NULL, 0, ThreadFunsion, 0, 0, NULL);

	printf("\n\n==============================================\n");
	printf("=                                            =\n");
	printf("=  안녕하세요. 충북대학교 주차 타워 입니다.  =\n");
	printf("=                                            =\n");
	printf("==============================================\n\n");
	printf("  * 차량 번호를 입력하세요 : ");

	while (1) {
		fgets(buf, sizeof(buf), stdin);

		if (strcmp(buf, "exit\n") == 0) {
			isThread = 0;
			break;
		}
		send(client_sock, buf, sizeof(buf), 0);
	}
	closesocket(client_sock);
	::WSACleanup();
}