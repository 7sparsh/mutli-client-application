#include<string>
using namespace std;

class Client {
public:
	int check(int returnVal, string func);
	void initializeSockaddr(struct sockaddr_in& serv, struct sockaddr_in& cli, int PORT);
	int socketSetup(struct sockaddr_in& serv, struct sockaddr_in &cli);
	void sendMessage(int clientSocket, struct sockaddr_in& cli);
	string decToHexa(int);
	string toHexa(string message);
	string destPort(string msg);
	string getMessage(string msg);
};