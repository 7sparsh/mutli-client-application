#include<string>
#include<unordered_map>
using namespace std;

class Server {
public:
	int check(int returnVal, string func);

	void initializeSockaddr(struct sockaddr_in &serv, int PORT);

	int socketSetup(struct sockaddr_in& serv);
	
	string hexaToASCII(string hex);
	
	string destPort(string msg);
	
	string cliPort(string msg);
	
	string getMessage(string msg);
	
	void displayMessage(string port, string msg, unordered_map<string, string>& linkTable);
	
	void populateMap(unordered_map<string, string>& linkTable, string port);
};