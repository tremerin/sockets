/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:15 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/04/02 16:47:40 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass)
{
	std::cout << "Server created" << std::endl;
	std::cout << "port: " << _port << std::endl;
	std::cout << "pass: " << _pass << std::endl;
}

Server::~Server()
{
	std::cout << "Server deleted" << std::endl;
}

// clear the clients
void Server::clearClients(int fd)
{
	// remove the client from the pollfd
	for(size_t i = 0; i < _fds.size(); i++)
	{ 
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i); 
			break;
		}
 	}
	// remove the client from the vector of clients
 	for(size_t i = 0; i < _clients.size(); i++)
	{ 
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i); 
			break;
		}
 	}
}

// initialize the static boolean
bool Server::_signal = false; 


void Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	// set the static boolean to true to stop the server
	Server::_signal = true; 
}

void Server::closeFds()
{
	// close all the clients
	for(size_t i = 0; i < _clients.size(); i++)
	{ 
		std::cout << RED << "Client <" << _clients[i].getFd() << "> Disconnected" << WHI << std::endl;
		close(_clients[i].getFd());
	}
	// close the server socket
	if (_serverFd != -1)
	{
		std::cout << RED << "Server <" << _serverFd << "> Disconnected" << WHI << std::endl;
		close(_serverFd);
	}
}
