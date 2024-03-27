/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:27 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/27 02:59:15 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include <vector>
# include "Client.hpp"
# include "sockets.h"

class Client;

class Server
{
private:
	//server socket creation:
	int							_fdServer;
	int							_port;

	//socket fds:
	std::vector<struct pollfd>	_poll_fds;
	int							_poll_size;
	int							_poll_count;

	//server
	std::string					_password;
	std::map<int, Client> 		_users;

public:
	Server(int port, std::string pass);
	~Server();

	void initServer(void);
	void serverLoop(void);
};

#endif
