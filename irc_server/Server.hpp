/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:27 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/23 18:04:18 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include "Client.hpp"
# include "sockets.h"

class Client;

class Server
{
private:
	//server socket creation:
	int						_fdServer;
	int						_port;

	//client sockets:
	struct pollfd 			*poll_fds;
	int						poll_size;
	int						poll_count;

	//server
	std::string				_password;
	std::map<int, Client> 	_users;

public:
	Server(int port, std::string pass);
	~Server();

	void initServer(void);
};

#endif