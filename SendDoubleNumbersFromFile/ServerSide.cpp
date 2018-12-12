#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#pragma comment (lib, "ws2_32.lib")
using namespace std;
char buf[1024];
int i = 0;

//convert to String vector into char array
void VectorToChar(char bufferArray[],vector<string> v, int i) {
	for (int index1 = i, index2 = i; index1 < (v.size()+i); index1++)
	{
		strcpy(&bufferArray[index2], v[index1].c_str());   
		index2 += v[index1].size();
	}
}

void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}
	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on
	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	string line;
	ifstream fin;

	fin.open("data.txt");
	if (fin.is_open())
	{
		vector <string> StringVector;
		while (getline(fin, line))
		{
			stringstream ss(line);
			while (getline(ss, line, ',')) {   //take each element of line without ','
				StringVector.push_back(line);
			}
			VectorToChar(buf, StringVector, i);
			if (send(clientSocket, (char*)&buf, 1024, 0) < 0) {
				puts("send failed\n");
			}
			ZeroMemory(buf, 1024);
			StringVector.clear();
		}
	}
	system("pause");
}
