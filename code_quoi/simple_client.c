#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4242  // puerto al que intentara conectar

int main(int ac, char **av)
{
	printf("---- CLIENT ----\n\n");
	struct sockaddr_in sa;
	int socket_fd;
	int status;
	char buffer[BUFSIZ];
	int bytes_read;
	char *msg;
	int msg_len;
	int bytes_sent;

	if (ac != 2)
	{
		printf("Usage: ./client \"Message to send\"");
		return (1);
	}

	// la direccion a la intentara conectar el cliente
	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET; // IPv4
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
	sa.sin_port = htons(PORT);

	// crea el socket
	socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		fprintf(stderr, "socket fd error: %s\n", strerror(errno));
		return (2);
	}
	printf("Created socket fd: %d\n", socket_fd);

	// intenta conectar el socket a la direccion del servidor
	status = connect(socket_fd, (struct sockaddr *)&sa, sizeof sa);
	if (status != 0)
	{
		fprintf(stderr, "connect error: %s\n", strerror(errno));
		return (3);
	}
	printf("Connected socket to localhost port %d\n", PORT);

	// envia un mensaje al servidor
	msg = av[1];
	msg_len = strlen(msg);
	bytes_sent = send(socket_fd, msg, msg_len, 0);
	if (bytes_sent == -1)
	{
		fprintf(stderr, "send error: %s\n", strerror(errno));
	}
	else if (bytes_sent == msg_len)
	{
		printf("Sent full message: \"%s\"\n", msg);
	}
	else
	{
		printf("Sent partial message: %d bytes sent.\n", bytes_sent);
	}

	// recive el mensaje del servidor
	bytes_read = 1;
	while (bytes_read >= 0)
	{
		bytes_read = recv(socket_fd, buffer, BUFSIZ, 0);
		if (bytes_read == 0)
		{
			printf("Server closed connection.\n");
			break ;
		}
		else if (bytes_read == -1)
		{
			fprintf(stderr, "recv error: %s\n", strerror(errno));
			break ;
		}
		else
		{
			buffer[bytes_read] = '\0';
			printf("Message received: \"%s\"\n", buffer);
			break ;
		}
	}

	printf("Closing socket\n");
	close(socket_fd);
	return (0);
}
