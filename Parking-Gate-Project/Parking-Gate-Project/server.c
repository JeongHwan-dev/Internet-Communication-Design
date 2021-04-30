#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

static int isThread = 1;
static int client_sock = 0;

void* ThreadFunction(void* arg) {
	char message2[100];
	int num;
	int regCar[5] = { 1234, 5678, 9012, 3456, 7890 };
	int check;

	while (isThread) {
		if (read(client_sock, message2, sizeof(message2) - 1) == -1) {
			printf("read error\n");
			exit(0);
		}

		num = atoi(message2);
		if (num >= 1000 && num <= 9999)
		{
			for (int i = 0; i < 5; i++) {
				if (num == regCar[i]) {
					check = 1;
					break;
				}
				else {
					check = 0;
				}
			}
			if (check == 1) {
				printf("\n\n--------------------------------\n");
				printf("  Car N%d is a Registered Car!\n", num);
				printf("--------------------------------\n");
				printf("\n  * Permission (pass/deny) : ");
			}
			else {
				printf("\n\n--------------------------------\n");
				printf("  Car N%d is a Unregistered Car!\n", num);
				printf("--------------------------------\n");
				printf("\n  * Permission (pass/deny) : ");
			}
		}
		if (strcmp(message2, "exit\n") == 0) {
			isThread = 0;
			exit(0);
		}
	}
	printf("end");
	pthread_exit(0);
}

void main() {
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	int server_sock = 0;
	struct sockaddr_in server_addr, client_addr;
	char message[100];
	int client_len = sizeof(client_addr);

	if ((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("socket error\n");
		return;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(23000);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("bind error\n");
		return;
	}
	if (listen(server_sock, 5) < 0) {
		printf("listen error\n");
		return;
	}

	printf("Waiting\n");

	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
	isThread = 1;
	pthread_create(&tid, &attr, &ThreadFunction, (void*)0);

	printf("\n\n==================================\n");
	printf(" CBNU Parking Management System\n");
	printf("==================================\n\n");

	while (1) {
		fgets(message, sizeof(message), stdin);

		if (strcmp(message, "exit\n") == 0) {
			isThread = 0;
			break;
		}
		write(client_sock, message, sizeof(message));
	}
	close(client_sock);
}