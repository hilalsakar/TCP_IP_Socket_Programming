#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
string ipAddress = "127.0.0.1";			// IP Address of the server
int port = 54000;						// Listening port # on the server
char const *message;
char bufferint[160];
char bufferfloat[160];
int receive_size;
SOCKET sock;

char * client_receive()
{
	if ((receive_size = recv(sock, bufferint, 160, 0)) == 0)
	{
		puts("recv failed.");
	}

	puts("reply received\n");
	int  intVar = 0;
	int * bufintPtr = &intVar;
	bufintPtr = (int*)&bufferint;

	bufferint[receive_size] = '\0';
	cout << *bufintPtr << endl;
	return bufferint;
}

char * client_receiveFloat()
{
	if ((receive_size = recv(sock, bufferfloat, 160, 0)) == 0)
	{
		puts("recv failed.");
	}

	float  floatVar = 0.00;
	float * bufFloatPtr = &floatVar;
	bufFloatPtr = (float*)&bufferfloat;

	bufferfloat[receive_size] = '\0';
	cout << *bufFloatPtr << endl;
	return bufferfloat;
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

	int intVariable = 56;
	float floatVariable = 56.64;

	if (send(sock, (char*)&floatVariable, sizeof(float), 0) < 0)
	{
		puts("send failed.");
		return 1;
	}
	puts("data send float\n");

	if (send(sock, (char*) &intVariable, sizeof(float), 0) < 0)
	{
		puts("send failed.");
		return 1;
	}
	puts("data send int\n");

	thread t1(client_receive);
	thread t2(client_receiveFloat);

	t1.join();
	t2.join();
	getchar();
	return 0;
}
