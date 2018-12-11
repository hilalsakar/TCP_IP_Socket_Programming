#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include <vector>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
string ipAddress = "127.0.0.1";			// IP Address of the server
int port = 54000;						// Listening port # on the server
char buffer[1024];
int control;
int receive_size;
SOCKET sock;

string myString = ""; // for 22th line of code

vector <double> doubleVector;
void CharToString(char buffer[], int k) {
	for (int i  = k; i >= k-3 ; i--) // We know that double number's size is 4byte (k to k-3)
	{
		myString = buffer[i] + myString;
	}
}

void client_receive()
{
	int k = 15; // We know that each line size of data.txt is 15byte
	while (true) {
		if ((receive_size = recv(sock, buffer, 1024, 0)) == 0)
		{
			puts("recv failed.");
		}
		else {
			control = 1;
		}

		while (control) {
			CharToString(buffer, k);
			stringstream ss(myString);
			string token;
			while (getline(ss, token)) {
				doubleVector.push_back(stod(myString));
			}
			if (k > 0) {
				k -= 4; //We know that first line of data.txt have 4 double number
			}
			
			myString = "";
			if (doubleVector.size() == 4) { //We know that each line have 4 double numbers.
				for (int i = 0; i < doubleVector.size(); i++)
				{
					cout << doubleVector[i] << (i == (doubleVector.size() - 1) ? "\n" : " ");
				}
				control = 0;
			}
		}
		ZeroMemory(buffer, 1024);
		doubleVector.clear();
		k = 15; 
	}
	
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

	thread t1(client_receive);
	t1.join();

	getchar();
	return 0;
}
