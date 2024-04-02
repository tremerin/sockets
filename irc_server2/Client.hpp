/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:57:55 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/04/02 16:21:09 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "sockets.h"

class Client											
{
private:
	int         _fd;					    // client file descriptor
	std::string _iPadd;					    // client ip address
    
public:
	Client();
    ~Client();	
    									
	int     getFd();						// getter for fd

	void    setFd(int fd);					// setter for fd
	void    setIpAdd(std::string ipadd); 	// setter for ipadd
};

#endif