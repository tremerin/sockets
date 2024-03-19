#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#define PORT 4242  // puerto del servidor
#define BACKLOG 10 // maxmo conexiones del servidor

int main(void)
{
    std::cout << "---- SERVER ----\n\n" << std::endl;
    struct sockaddr_in sa;
    int socket_fd; //socket donde el servidor recive las peticiones
    int client_fd; //despues de una peticion para hablar con el cliente
    int status;
    struct sockaddr_storage client_addr; 
    socklen_t addr_size;
    char buffer[BUFSIZ];
    int bytes_read;

    // direccion y puerto para el socket del servidor
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(PORT);

    // creacion del socket
    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        fprintf(stderr, "socket fd error: %s\n", strerror(errno));
        return (1);
    }
    printf("Created server socket fd: %d\n", socket_fd);

    // liga el socket con una direccion
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        return (2);
    }
    printf("Bound socket to localhost port %d\n", PORT);

    // pone al socket en modo escucha
    printf("Listening on port %d\n", PORT);
    status = listen(socket_fd, BACKLOG);
    if (status != 0) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        return (3);
    }

    // crea el socket para hablar con el cliente despues de aceptar una peticion
    addr_size = sizeof client_addr;
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd == -1) {
        fprintf(stderr, "client fd error: %s\n", strerror(errno));
        return (4);
    }
    printf("Accepted new connection on client socket fd: %d\n", client_fd);

    // recibe el mensaje del cliente desde el socket cliente
    bytes_read = 1;
    while (bytes_read >= 0) {
        printf("Reading client socket %d\n", client_fd);
        bytes_read = recv(client_fd, buffer, BUFSIZ, 0);
        if (bytes_read == 0) {
            printf("Client socket %d: closed connection.\n", client_fd);
            break ;
        }
        else if (bytes_read == -1) {
            fprintf(stderr, "recv error: %s\n", strerror(errno));
            break ;
        }
        else {
            // imprime el mesanje  completo
            const char *msg = "Got your message.";
            int msg_len = strlen(msg);
            int bytes_sent;

            buffer[bytes_read] = '\0';
            printf("Message received from client socket %d: \"%s\"\n", client_fd, buffer);
                        
            bytes_sent = send(client_fd, msg, msg_len, 0);
            if (bytes_sent == -1) {
                fprintf(stderr, "send error: %s\n", strerror(errno));
            }
            else if (bytes_sent == msg_len) {
                printf("Sent full message to client socket %d: \"%s\"\n", client_fd, msg);
            }
            else {
                printf("Sent partial message to client socket %d: %d bytes sent.\n", client_fd, bytes_sent);
            }
        }
    }

    printf("Closing client socket\n"); 
    close(client_fd); // cierra el socket que hablaba con el cliente
    printf("Closing server socket\n"); 
    close(socket_fd); // cierra el socket propio del server

    return (0);
}