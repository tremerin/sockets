/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:57:44 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/04/02 16:22:24 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() 
{
    std::cout << "Client created" << std::endl;
}

Client::~Client()
{
    std::cout << "Client deleted" << std::endl;
}

//get
int     Client::getFd() {return _fd;}

//set
void    Client::setFd(int fd) {_fd = fd;}
void    Client::setIpAdd(std::string ipadd) {_iPadd = ipadd;}
