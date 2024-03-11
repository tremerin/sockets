//Obtener las direcciones IP asociadas a un nombre de host 

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int ac, char **av)
{
	struct addrinfo hints;           // Parametros para getaddrinfo()
	struct addrinfo *res;            // Resultado de getaddrinfo()
	struct addrinfo *r;              // Pointeur pour itérer sur les résultats
	int status;                      // Valor de retorno de getaddrinfo()
	char buffer[INET6_ADDRSTRLEN];   // Buffer para reconvertir la direccion IP

	if (ac != 2)
	{
		fprintf(stderr, "usage: /a.out hostname\n");
		return (1);
	}
	memset(&hints, 0, sizeof hints);  // Inicializa la estructura
	hints.ai_family = AF_UNSPEC;      // AF_UNSPEC = IPv4 o IPv6, AF_INET = IPv4, AF_INET6 = IPv6
	hints.ai_socktype = SOCK_STREAM;  // SOCK_STREAM = TCP, SOCK_DGRAM = UDP

	// Recupera las direcciones IP asociadas
	status = getaddrinfo(av[1], 0, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (2);
	}

	printf("IP adresses for %s:\n", av[1]);

	r = res;
	while (r != NULL)
	{
		void *addr;
		// IPv4
		if (r->ai_family == AF_INET)
		{ 
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)r->ai_addr;
			inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
			printf("IPv4: %s\n", buffer);
		}
		else
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)r->ai_addr;
			inet_ntop(r->ai_family, &(ipv6->sin6_addr), buffer, sizeof buffer);
			printf("IPv6: %s\n", buffer);
		}
		r = r->ai_next;
	}
	freeaddrinfo(res);
	return (0);
}
