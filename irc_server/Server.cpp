/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalan-r <fgalan-r@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:15 by fgalan-r          #+#    #+#             */
/*   Updated: 2024/03/25 19:15:27 by fgalan-r         ###   ########.fr       */
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
	close(_fdServer);
	std::cout << "Server deleted" << std::endl;
}

static int createServerSocket(int port)
{
	struct sockaddr_in sAddress;
	int socket_fd;
	int status;

	// Prepare the address and port for the server socket
	sAddress.sin_family = AF_INET;                     // IPv4
	sAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
	sAddress.sin_port = htons(port);                   // Port
	// Create server socket
	socket_fd = socket(sAddress.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		std::cerr << "Socket error: " << errno << std::endl; // strerror(errno);
		return (-1);
	}
	std::cout << "Created server socket fd: " << socket_fd << std::endl;
	// Bind socket to address and port
	status = bind(socket_fd, (struct sockaddr *)&sAddress, sizeof(sAddress));
	if (status != 0)
	{
		std::cerr << "Blind error: " << errno << std::endl;  // strerror(errno);
		return (-1);
	}
	std::cout << "Bound socket to localhost port: " << port << std::endl;

	return (socket_fd);
}

static void accept_new_connection(int server_socket, struct pollfd *poll_fds, int *poll_count, int *poll_size)
{
	int client_fd;
	char msg_to_send[BUFSIZ];
	int status;

	client_fd = accept(server_socket, NULL, NULL);
	if (client_fd == -1)
	{
		fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
		return ;
	}
	//add_to_poll_fds(poll_fds, client_fd, poll_count, poll_size);
	if (*poll_count < *poll_size)
	{
		poll_fds[*poll_count].fd = client_fd;
		poll_fds[*poll_count].events = POLLIN;
		(*poll_count)++;
	}
	else
		std::cout << "error: maximum number of connected clients" << std::endl;

	printf("[Server] Accepted new connection on client socket %d.\n", client_fd);

	memset(&msg_to_send, '\0', sizeof msg_to_send);
	sprintf(msg_to_send, "Welcome. You are client fd [%d]\n", client_fd);
	status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
	if (status == -1)
		fprintf(stderr, "[Server] Send error to client %d: %s\n", client_fd, strerror(errno));
}

static void read_data_from_socket(int i, struct pollfd *poll_fds, int *poll_count, int server_socket)
{
	char buffer[BUFSIZ];
	char msg_to_send[BUFSIZ];
	int bytes_read;
	int status;
	int dest_fd;
	int sender_fd;

	sender_fd = poll_fds[i].fd;
	memset(&buffer, '\0', sizeof buffer);
	bytes_read = recv(sender_fd, buffer, BUFSIZ, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
		{
			printf("[%d] Client socket closed connection.\n", sender_fd);
		}
		else
		{
			fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
		}
		close(sender_fd); // Close socket
		//del_from_poll_fds(poll_fds, i, poll_count);
	}
	else
	{
		// Relays the received message to all connected sockets
		// but not to the server socket or the sender socket
		printf("[%d] Got message: %s", sender_fd, buffer);

		memset(&msg_to_send, '\0', sizeof msg_to_send);
		sprintf(msg_to_send, "[%d] says: %s", sender_fd, buffer);
		for (int j = 0; j < *poll_count; j++)
		{
			dest_fd = poll_fds[j].fd;
			if (dest_fd != server_socket && dest_fd != sender_fd)
			{
				status = send(dest_fd, msg_to_send, strlen(msg_to_send), 0);
				if (status == -1)
				{
					fprintf(stderr, "[Server] Send error to client fd %d: %s\n", dest_fd, strerror(errno));
				}
			}
		}
	}
}

void Server::serverLoop(void)
{
	int status;
	while (1)
	{
 		// Poll sockets to see if they are ready (2 second timeout)
		status = poll(&_poll_fds[0], _poll_count, 2000);
		if (status == -1)
		{
			std::cout << "Poll error: " << errno << std::endl;
			exit(1);
		}
		else if (status == 0)
		{
			// None of the sockets are ready
			printf("[Server] Waiting...\n");
			continue;
		}
				// Loop on our array of sockets
		for (int i = 0; i < _poll_count; i++)
		{
			if ((_poll_fds[i].revents & POLLIN) != 1)
			{
				// The socket is not ready for reading
				continue ;
			}
			printf("[%d] Ready for I/O operation\n", _poll_fds[i].fd);
			// The socket is ready for reading!
			if (_poll_fds[i].fd == _fdServer)
			{
				// Socket is our listening server socket
				accept_new_connection(_fdServer, &_poll_fds[0], &_poll_count, &_poll_size);
				// crear los usuarios del server
			}
			else
			{
				// Socket is a client socket, read it
				read_data_from_socket(i, &_poll_fds[0], &_poll_count, _fdServer);
				// interpretar las acciones de los usuarios
			}
		}
	}
}

void Server::initServer(void)
{
	struct pollfd	newFd;
	_fdServer = createServerSocket(_port);
	if (_fdServer < 0)
	{
		return ;
	}
	int status = listen(_fdServer, 10);
	if (status != 0)
	{
		std::cout << "Listen error: " << errno << std::endl;
		return ;
	}
	else
		std::cout << "Listening on port: " << _port << std::endl;
	newFd.fd = _fdServer;
	newFd.events = POLLIN;
	newFd.revents = 0;
	_poll_fds.push_back(newFd);
	_poll_count = 1;
	_poll_size = 20;
}
