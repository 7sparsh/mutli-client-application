#include<iostream>
#include<winsock.h>
#include <sstream>
#include<unordered_map>
#include"server.h"

#define IP "192.168.0.111"
#define PORT 9000

using namespace std;

int main() {

	int iResult;

	unordered_map<string, string>linkTable;

	//object of class "Server"
	Server obj;

	//initializing socket environment for socket comms
	WSADATA ws;
	iResult = WSAStartup(MAKEWORD(2, 2), &ws);
	obj.check(iResult, "WSA");

	struct sockaddr_in serv;

	//calling public member function for initializing sockaddr_in
	obj.initializeSockaddr(serv, PORT);

	//calling public member function for creating a server socket
	int serverSocket = obj.socketSetup(serv);


	fd_set clients;
	FD_ZERO(&clients);

	FD_SET(serverSocket, &clients);

	while (true) {
		fd_set temp = clients;

		int socketCount = select(0, &temp, NULL, NULL, NULL);

		for (int i = 0; i < socketCount; i++) {
			int sock = temp.fd_array[i];
			
			string tempSock;

			if (sock == serverSocket) {
				int clientSocket = accept(serverSocket, NULL, NULL);
				

				FD_SET(clientSocket, &clients);
			}

			else {
				char buffer[4096];
				char clientPort[256];
				ZeroMemory(buffer, 4096);

				// Receive message
				recv(sock, clientPort, 256, 0);

				string port(clientPort);
				obj.populateMap(linkTable, port);

				int bytesIn = recv(sock, buffer, 4096, 0);

				if (bytesIn <= 0)
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &clients);
					break;
				}
				else {

					string receivedMsg(buffer);

					//converting to string
					string asciiString = obj.hexaToASCII(receivedMsg);

					if (asciiString == "wq") {
						cout << endl << "Client " << clientPort << ": " << "terminated!" << endl;
						obj.populateMap(linkTable, port);
					}
					else {
						char const* buffer1 = asciiString.c_str();

						obj.displayMessage(port, asciiString, linkTable);
					}
				}
			}
		}
	}


	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(serverSocket, &clients);
	closesocket(serverSocket);

	while (clients.fd_count > 0) {
		// Get the socket number
		int sock = clients.fd_array[0];
		cout << sock << endl;

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &clients);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();

	return 0;
}



