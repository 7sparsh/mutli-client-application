#include"server.h"
#include<iostream>
#include<winsock.h>
#include<regex>
#include<string>
#include<unordered_map>
#define IP "192.168.0.111"

using namespace std;

//function to validate system calls(assess success/failure based on return values)
int Server::check(int returnVal, string func) {
	if (returnVal < 0) {
		cout << "Problem with " << func << endl;
		return 1;
	}
	else {
		cout << func << " " << "successful!" << endl;
	}
	return 0;
}

//function to initialize sockaddr_in to hold information about server(address family, address, port)
void Server::initializeSockaddr(struct sockaddr_in& serv, int PORT) {
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	serv.sin_addr.S_un.S_addr = inet_addr(IP);
}

//function to set up socket on the server side(socket, bind, listen system calls)
int Server::socketSetup(struct sockaddr_in& serv) {
	//create a socket on the server side
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // address format, type of socket, protocol used
	check(serverSocket, "SOCKET CREATION");

	int iResult;

	//binding socket to local port
	iResult = bind(serverSocket, (struct sockaddr*)&serv, sizeof(serv));
	check(iResult, "BIND");

	//listen to requests
	iResult = listen(serverSocket, 5); /*second argument is backlog: maximum length of the queue of
	pending connections to accept. Look up "SOMAXCONN"*/
	check(iResult, "LISTEN TO PORT");

	cout << endl;

	return serverSocket;
}

//function to convert hex sequence to ascii sequence
string Server::hexaToASCII(string hex) {
	string ascii = "";
	for (size_t i = 0; i < hex.length(); i += 2) {
		//taking two characters from hex string
		string part = hex.substr(i, 2);
		//changing it into base 16
		char ch = stoul(part, nullptr, 16);
		//putting it into the ASCII string
		ascii += ch;
	}
	return ascii;
}

//function to extract destination address from received message using regex
string Server::destPort(string msg) {
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

//function to extract client port(source port) from received message using regex
string Server::cliPort(string msg) {
	regex re1("clnt:[0-9]+");
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
string Server::getMessage(string msg) {
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

//function to display message on the console
void Server::displayMessage(string port, string msg, unordered_map<string, string>&linkTable) {

	string clientPort = cliPort(msg);
	string destinationPort = destPort(msg);

	string output = "9000 got, " + getMessage(msg);

	if (clientPort == "" or destinationPort == "") {
		output = "Ports missing. Please send a message with valid source and destination ports.";
	}
	else if (destinationPort == "9000") {
		output += " from " + clientPort + " and it is for me";
	}
	else if (linkTable.find(port) != linkTable.end()) {
		if (linkTable[port] == "UP") {
			output = "Port active. Message delivered!";
		}
		else {
			output = "Port inactive. Message discarded!";
		}
	}
	else {
		output = "Port does not exist. Message discarded!";
	}

	cout << "Client " << clientPort << ": " << output << endl;
}

//fucntion to add a port to a map or update it's status
void Server::populateMap(unordered_map<string, string>& linkTable, string port) {
	if (linkTable.find(port) == linkTable.end()) {
		linkTable[port] = "UP";
	}
	else {
		if (linkTable[port] == "UP") {
			linkTable[port] = "DOWN";
		}
		else {
			linkTable[port] = "UP";
		}
	}
}


