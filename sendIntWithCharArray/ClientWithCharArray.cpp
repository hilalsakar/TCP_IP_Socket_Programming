#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
string ipAddress = "127.0.0.1";			// IP Address of the server
int port = 54000;						// Listening port # on the server
char buffer[160];
char buffer2[160];
int receive_size;
SOCKET sock;

char * client_receive()
{
	if ((receive_size = recv(sock, buffer2, 160, 0)) == 0)
	{
		puts("recv failed.");
	}
	puts("reply received\n");
	buffer2[receive_size] = '\0';

	int * bufIntPtr;
	bufIntPtr = (int*)&buffer2;
	char buffer3[4];
	buffer3[0] = (char)((*bufIntPtr >> 24) & 0xff);
	buffer3[1] = (char)((*bufIntPtr >> 16) & 0xff);
	buffer3[2] = (char)((*bufIntPtr >> 8) & 0xff);
	buffer3[3] = (char)((*bufIntPtr) & 0xff);
	int *a;
	a = (int*)&buffer3;
	cout << *a << endl;

	return buffer2;
}

int main(int argc, char *argv[])
{

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << WSAGetLastError << endl;
		return 1;
	}
	cout << "Initialized\n";

	// Create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Created socket\n";


	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		return 1;
	}

	int  willSendValue = 1;
	
	buffer2[0] = (char)((willSendValue >> 24) & 0xff);
	buffer2[1] = (char)((willSendValue >> 16) & 0xff);
	buffer2[2] = (char)((willSendValue >> 8) & 0xff);
	buffer2[3] = (char)((willSendValue) & 0xff);


	if (send(sock, (char*)&buffer2, sizeof(int), 0) < 0)
	{
		puts("send failed.");
		return 1;
	}
	puts("data send\n");

	thread t1(client_receive);
	t1.join();

	getchar();
	return 0;
}
