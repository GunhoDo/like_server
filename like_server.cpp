#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

#define PACKET_LENGTH 4

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::string;

int main()
{
	int ssock;
	int csock;
	int clen;
	struct sockaddr_in client_addr, server_addr;
	char recvBufSize[PACKET_LENGTH]; //파일 크기를 받을 만큼만
	int nFileSize = 0;

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

	csock = accept(ssock, (struct sockaddr*)&client_addr, &clen);
	

	if (recv(csock, recvBufSize, PACKET_LENGTH, 0) <= 0) //csock 통신으로 recvBufSize에 4만큼 받는다
	{
		closesocket(csock);
	}

	memcpy(&nFileSize, recvBufSize, PACKET_LENGTH); // nFileSize에 recvBufSize에 있는 값(전체 크기4)를 옮긴다

	char* pData = new char[nFileSize];
	if (recv(csock, pData, nFileSize, 0) <= 0) //csock 통신으로 pData에 nFileSize만큼 받는다
	{
		closesocket(csock);
	}

	// 파일 쓰기 모드로 생성 및 열기
	ofstream ofStream;


	char filename[256];
	sprintf_s(filename, "%d.png", csock);

	// std::ios::out    파일에 쓰기
	// std::ios::binary 바이너리 모드로 쓰기
	ofStream.open(filename, std::ios::out);

	// 정상이면 쓴다
	if (ofStream.is_open())
	{
		// 파일에 쓰기
		ofStream.write(pData, nFileSize);
	}
		// 파일을 닫는다
	ofStream.close();

	delete[] pData;
	/*
	if (send(csock, recvBuf, MAXBUF, 0) <= 0)
	{
		free(recvBuf);
		closesocket(csock);
	}
	*/
	WSACleanup();

	return 0;
}