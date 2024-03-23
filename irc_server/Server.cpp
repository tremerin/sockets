/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:15 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/23 17:54:08 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _password(pass)
{
    std::cout << "Server created" << std::endl;
    std::cout << "port: " << _port << std::endl;
    std::cout << "pass: " << _password << std::endl;
}

Server::~Server()
{
    std::cout << "Server deleted" << std::endl;
}

void Server::initServer(void)
{

}