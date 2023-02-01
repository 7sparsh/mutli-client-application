#include"client.h"
#include<iostream>
#include<winsock.h>
#include<string>
#include<regex>
#define IP "192.168.0.111"

using namespace std;

//function to validate system calls(assess success/failure based on return values)
int Client::check(int returnVal, string func) {
	if (returnVal < 0) {
		cout << "Problem with " << func << endl;
		return 1;
	}
	else {
		cout << func << " " << "successful!" << endl;
	}
	return 0;
}

//function to initialize sockaddr_in to hold information about client(address family, address, port)
void Client::initializeSockaddr(struct sockaddr_in& serv, struct sockaddr_in &cli, int PORT) {
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	serv.sin_addr.S_un.S_addr = inet_addr(IP);

	cli.sin_family = AF_INET;
	cli.sin_addr.S_un.S_addr = inet_addr(IP);
}

//function to set up socket on the server side(socket, bind, listen system calls)
int Client::socketSetup(struct sockaddr_in& serv, struct sockaddr_in &cli) {
	// Create socket at client
	int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // address format, type of socket, protocol used
	check(clientSocket, "SOCKET CREATION");

	int iResult;

	//Binding
	iResult = bind(clientSocket, (struct sockaddr*)&cli, sizeof(cli));
	check(clientSocket, "BINDING");

	// Connect to server
	iResult = connect(clientSocket, (sockaddr*)&serv, sizeof(serv));
	check(iResult, "CONNECTION TO SERVER");

	return clientSocket;
}

//function to convert decimal character to hexa character
string Client::decToHexa(int n) {
	// char array to store hexadecimal number
	char hexaDeciNum[100];

	// counter for hexadecimal number array
	int i = 0;
	while (n != 0) {
		// temporary variable to store remainder
		int temp = 0;

		// storing remainder in temp variable.
		temp = n % 16;

		// check if temp < 10
		if (temp < 10) {
			hexaDeciNum[i] = temp + 48;
			i++;
		}
		else {
			hexaDeciNum[i] = temp + 55;
			i++;
		}

		n = n / 16;
	}

	string ans = "";

	// printing hexadecimal number array in reverse order
	for (int j = i - 1; j >= 0; j--)
		ans += hexaDeciNum[j];

	return ans;
}

//function to convert ascii string sequence to hexa sequence
string Client::toHexa(string message) {

	string hex = "";
	
	if (message.size() == 0) {
		return hex;
	}

	// Make a loop to iterate through
	// every character of ascii string
	for (int i = 0; i < message.length(); i++) {
		// Take a char from
		// position i of string
		char ch = message[i];

		// Cast char to integer and
		// find its ascii value
		int tmp = (int)ch;

		// Change this ascii value
		// integer to hexadecimal value
		string part = decToHexa(tmp);

		// Add this hexadecimal value
		// to final string.
		hex += part;
	}

	// Return the final
	// string hex
	return hex;
}

//function to extract destination address from received message using regex
string Client::destPort(string msg) {
	regex re1("dest:[0-9]+");
	smatch match;
	if (regex_search(msg, match, re1) == 1) {
		string st = match.str(0);
		int sizet = st.size();
		if (sizet > 10) {
			return "";
		}
		return st.substr(5);
	}
	else {
		return "";
	}
}

//function to extract main message from the received message using regex
string Client::getMessage(string msg) {
	regex re1("[^.]* am [^.]*\.");
	smatch match;
	if (regex_search(msg, match, re1) == 1) {
		string st = match.str(0);
		int sizet = st.size();
		return st;
	}
	else {
		return "";
	}
}

//fucntion to receive message from the user and send it to the server
void Client::sendMessage(int clientSocket, struct sockaddr_in& cli) {
	while (true) {

		int iResult;

		cout << "> ";
		char buffer[4096];
		gets_s(buffer, 4096);

		string msg(buffer);

		if (msg.size() == 0) {
			cout << "Cannot send an empty message! Try again." << endl;
			continue;
		}

		string clntPort = destPort(msg);

		string tempMsg(buffer);
		tempMsg = toHexa(tempMsg);
		char const *buffer1 = tempMsg.c_str();

		string tempPort = to_string(ntohs(cli.sin_port));
		char const* clientPort = tempPort.c_str();

		if (clntPort == tempPort) {
			string message(buffer);
			message = getMessage(buffer);
			cout << "Message: " << message << endl << endl;
			continue;
		}

		iResult = send(clientSocket, clientPort, 256, 0);
		check(iResult, "PORT TRANSFER");

		iResult = send(clientSocket, buffer1, 4096, 0);
		check(iResult, "MESSAGE SENDING");

		if (buffer[0] == 'w' and buffer[1] == 'q') {
			cout << "Terminating client!!" << endl;
			break;
		}

		cout << endl;
	}
}

