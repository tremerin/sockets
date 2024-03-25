/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:04 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/23 20:56:58 by fgalan-r         ###   ########.fr       */
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
		server.initServer();
		server.serverLoop();
	}
	else
	{
		std::cout << "error" << std::endl;
	}

	return (0);
}