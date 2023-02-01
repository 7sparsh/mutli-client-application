#include <iostream>
#include <string>
#include <winsock.h>
#include"client.h"

#define IP "192.168.0.111"
#define PORT 9000

using namespace std;

int main() {

	int iResult;

	Client obj;

	// Initialize WinSock
	WSADATA ws;
	iResult = WSAStartup(MAKEWORD(2, 2), &ws);
	obj.check(iResult, "WSA");

	//create structs for stroing info
	struct sockaddr_in serv, cli;

	//calling public member function for initializing serv and cli
	obj.initializeSockaddr(serv, cli, PORT);

	//calling public member function for creating a client socket
	int clientSocket = obj.socketSetup(serv, cli);

	int clientsz = sizeof(cli);
	getsockname(clientSocket, (struct sockaddr*)&cli, &clientsz);
	cout << endl << ntohs(cli.sin_port) << endl;

	cout << endl << "Type <wq> to terminate client." << endl << endl;

	//sending message to the server
	obj.sendMessage(clientSocket, cli);

	closesocket(clientSocket);
	return 0;
}
