#include "../../header/proxy/server_side.h"
#include "../../header/proxy/proxy.h"

void connection(int dest_socket)
{
    int connect_result;
    struct sockaddr_in addr;
    char status;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_SERVER);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton(HOST_SERVER, &(addr.sin_addr));

    connect_result = connect(dest_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (connect_result < 0) {
        perror("Connect result");
        exit(1);
    }

    recv(dest_socket, &status, 1, 0);
    if (status == SUCCESS) {
        printf("%s\n", S_SERVER_MSG);
    } else {
        printf("%s\n", F_SERVER_MSG);
        exit(1);
    }
}

void *talk_server(void *args)
{
    int dest_socket;
    int recv_result;
    int *stop;
    struct args *a;
    int client_idx;

    a = args;

    dest_socket = a->socket;
    stop = a->stop;

    connection(dest_socket);

    while (!*stop) {

        pthread_mutex_lock(a->m);
        while (!*a->rcv) // Attente du bruitage du message par le thread noisemaker.
            pthread_cond_wait(a->c, a->m);

        *a->rcv = 0;

        client_idx = *a->client_idx;

        // Envoie du message bruité au serveur.
        PROXY_DISPLAY((char *)a->clients[client_idx].buf);
        send(dest_socket, a->clients[client_idx].username, USERNAME_SIZE, 0);
        send(dest_socket, a->clients[client_idx].buf, a->clients[client_idx].buf_size, 0);

        // Attente de réception de l'ACK de la part du serveur.
        recv_result = recv(dest_socket, &a->clients[client_idx].ack, 1, 0);
        if (recv_result < 1) {
            printf("Connection interrupted\n");
            break;
        }

        if (a->clients[client_idx].ack) {
            SERVER_DISPLAY(ACK_MSG);
        } else {
            SERVER_DISPLAY(NACK_MSG);
        }

        a->poll_fds[client_idx + 1].events = POLLOUT;

        pthread_mutex_unlock(a->m);
    }
    close(dest_socket);

    return 0;
}
