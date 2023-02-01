# mutli-client-application
A C++ application that supports bi-directional communication between a server and multiple clients.

The application consists of two main components: Server and Client. 

Server:
 > server.h: consists of class declaration.  
 > server.cpp: consists of implementation of every member function.  
 > serverNew.cpp: Consists of the implementation of the server.
 
 "serverNew.cpp" is the first file to be executed, for the application to run.
 
Client: 
 > client.h: consists of class declaration.  
 > client.cpp: consists of implementation of every member function.  
 > clientNew.cpp: Consists of the implementation of the client.
 
After "serverNew.cpp" has been executed, "clientNew.cpp" must be executed to connect to the server. The application supports multiple instances of the client and each 
instance of the client exists on a different port.


