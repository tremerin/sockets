#include <iostream>
#include <vector>			// vector
#include <sys/socket.h>		// socket()
#include <sys/types.h>		// socket()
#include <netinet/in.h>		// sockaddr_in
#include <fcntl.h>			// fcntl()
#include <unistd.h>			// close()
#include <arpa/inet.h>		// inet_ntoa()
#include <poll.h>			// poll()
#include <csignal>			// signal()
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RED "\e[1;31m"		// red color
#define WHI "\e[0;37m"		// white color
#define GRE "\e[1;32m"		// green color
#define YEL "\e[1;33m"		// yellow color

class Client											// class client
{
private:
	int Fd;												// client file descriptor
	std::string IPadd;									// client ip address
public:
	Client(){};											// default constructor
	int GetFd(){return Fd;} 							// getter for fd

	void SetFd(int fd){Fd = fd;}						// setter for fd
	void setIpAdd(std::string ipadd){IPadd = ipadd;} 	// setter for ipadd
};

class Server											// class for server
{
private:
	int Port;											// server port
	int SerSocketFd;									// server socket file descriptor
	static bool Signal;									// static boolean for signal
	std::vector<Client> clients;						// vector of clients
	std::vector<struct pollfd> fds;						// vector of pollfd
public:
	Server(){SerSocketFd = -1;} 						// default constructor

	void ServerInit();									// server initialization
	void SerSocket();									// server socket creation
	void AcceptNewClient();								// accept new client
	void ReceiveNewData(int fd);						// receive new data from a registered client

	static void SignalHandler(int signum); 				// signal handler
 
	void CloseFds();									// close file descriptors
	void ClearClients(int fd);							// clear clients
};

//-------------------------------------------------------------------------------------------------//
// clear the clients
void Server::ClearClients(int fd)
{
	// remove the client from the pollfd
	for(size_t i = 0; i < fds.size(); i++)
	{ 
		if (fds[i].fd == fd)
			{fds.erase(fds.begin() + i); break;}
 	}
	// remove the client from the vector of clients
 	for(size_t i = 0; i < clients.size(); i++)
	{ 
		if (clients[i].GetFd() == fd)
		{
			clients.erase(clients.begin() + i); 
			break;
		}
 	}
}

// initialize the static boolean
bool Server::Signal = false; 


void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	// set the static boolean to true to stop the server
	Server::Signal = true; 
}

void Server::CloseFds()
{
	// close all the clients
	for(size_t i = 0; i < clients.size(); i++)
	{ 
		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(clients[i].GetFd());
	}
	// close the server socket
	if (SerSocketFd != -1)
	{
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(SerSocketFd);
	}
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024]; // buffer for the received data
	memset(buff, 0, sizeof(buff)); // clear the buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0); // receive the data
	// check if the client disconnected
	if(bytes <= 0)
	{ 
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd); // clear the client
		close(fd); // close the client socket
	}
	// print the received data
 	else
	{ 
		buff[bytes] = '\0';
		std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}

void Server::AcceptNewClient()
{
	// create a new client
	Client cli; 
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);
	// accept the new client
	int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len); 
	if (incofd == -1)
  	{
		std::cout << "accept() failed" << std::endl; 
		return;
	}
	// set the socket option (O_NONBLOCK) for non-blocking socket
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) 
	{
		std::cout << "fcntl() failed" << std::endl; 
		return;
	}

	NewPoll.fd = incofd;						// add the client socket to the pollfd
	NewPoll.events = POLLIN;					// set the event to POLLIN for reading data
	NewPoll.revents = 0;						// set the revents to 0

	cli.SetFd(incofd);							// set the client file descriptor
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr))); // convert the ip address to string and set it
	clients.push_back(cli);						// add the client to the vector of clients
	fds.push_back(NewPoll);						// add the client socket to the pollfd

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::SerSocket()
{
	int en = 1;
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET;			// set the address family to ipv4
	add.sin_addr.s_addr = INADDR_ANY;	// set the address to any local machine address
	add.sin_port = htons(this->Port);	// convert the port to network byte order (big endian)

	// create the server socket
	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); 
	// check if the socket is created
	if (SerSocketFd == -1) 
		throw(std::runtime_error("faild to create socket"));
	// set the socket option (SO_REUSEADDR) to reuse the address
	if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) 
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	// set the socket option (O_NONBLOCK) for non-blocking socket
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	// bind the socket to the address
	if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) 
		throw(std::runtime_error("faild to bind socket"));
	// listen for incoming connections and making the socket a passive socket
	if (listen(SerSocketFd, SOMAXCONN) == -1) 
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = SerSocketFd;  // add the server socket to the pollfd
	NewPoll.events = POLLIN;   // set the event to POLLIN for reading data
	NewPoll.revents = 0;       // set the revents to 0
	fds.push_back(NewPoll);    // add the server socket to the pollfd
}

void Server::ServerInit()
{
	this->Port = 4242;
	SerSocket(); // create the server socket

	std::cout << GRE << "Server <" << SerSocketFd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	// run the server until the signal is received
	while (Server::Signal == false)
	{ 
		// wait for an event
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false) 
			throw(std::runtime_error("poll() faild"));
		// check all file descriptors
		for (size_t i = 0; i < fds.size(); i++)
		{
			// check if there is data to read
			if (fds[i].revents & POLLIN)
			{ 
    			if (fds[i].fd == SerSocketFd)
     				AcceptNewClient(); 			// accept new client
    			else
     				ReceiveNewData(fds[i].fd);	// receive new data from a registered client
			}
		}
	}
	CloseFds(); // close the file descriptors when the server stops
}

int main()
{
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler);	// catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); // catch the signal (ctrl + \)
		ser.ServerInit();						// initialize the server
	}
	catch(const std::exception& e)
	{
		ser.CloseFds();							// close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}