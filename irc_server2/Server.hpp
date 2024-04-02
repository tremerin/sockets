/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:27 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/04/02 16:46:30 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include "Client.hpp"
# include "sockets.h"

# define RED "\e[1;31m"		// red color
# define WHI "\e[0;37m"		// white color
# define GRE "\e[1;32m"		// green color
# define YEL "\e[1;33m"		// yellow color

class Client;

class Server										
{
private:
	int 							_port;				// server port
	int								_serverFd;			// server socket file descriptor
	std::string						_pass;				// server password
	std::vector<Client>				_clients;			// vector of clients
	std::vector<struct pollfd>		_fds;				// vector of pollfd
	static bool						_signal;			// static boolean for signal
	
public:
	Server(int port, std::string pass);						
	~Server();

	void serverInit();									// server initialization
	void serSocket();									// server socket creation
	void acceptNewClient();								// accept new client
	void receiveNewData(int fd);						// receive new data from a registered client

	static void signalHandler(int signum); 				// signal handler
 
	void closeFds();									// close file descriptors
	void clearClients(int fd);							// clear clients
};

#endif
