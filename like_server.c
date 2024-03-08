#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAXBUF 256

int main()
{
	int ssock;
	int csock;
	int clen;
	struct sockaddr_in client_addr, server_addr;
	char recvBuf[MAXBUF];
	char buf[MAXBUF] = "¾È³ç!";

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int nErrorStatus;

    nErrorStatus = WSAStartup(wVersionRequested, &wsaData);

	if (nErrorStatus != 0)
	{
		return 0;
	}

	if ((ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		return 0;
	}

	clen = sizeof(client_addr);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(3317);

	if (bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		return 0;
	}

	if (listen(ssock, 8) < 0)
	{
        return 0;
	}

	//while (1)
	{
		csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);

		if (recv(csock, recvBuf, MAXBUF, 0) <= 0)
		{
			closesocket(csock);
		}

		if (send(csock, buf, MAXBUF, 0) <= 0)
		{
			closesocket(csock);
		}

		if (recv(csock, recvBuf, MAXBUF, 0) <= 0)
		{
			closesocket(csock);
		}
	}

	WSACleanup();

	return 0;
}
