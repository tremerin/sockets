/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:04 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/29 04:37:28 by fgalan-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"


int main(int argc, char **argv)
{
	if (argc == 3)
	{
		// validar y pasar argumentos
		Server server(4242, argv[2]);
		try
		{
			server.initServer();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
		server.serverLoop();
	}
	else
	{
		std::cout << "error" << std::endl;
	}

	return (0);
}