#include "../../header/proxy/client_side.h"
#include "../../header/proxy/proxy.h"

void *wait_client(void *args)
{
    struct args *a;
    int proxy_socket;
    int *stop;

    int poll_result;
    int connected;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int client_socket;
    int buf_size;

    struct pollfd *poll_fds;
    struct client_info *clients;

    a = args;

    proxy_socket = a->socket;
    stop = a->stop;
    poll_fds = a->poll_fds;
    clients = a->clients;

    poll_fds[0].fd = proxy_socket;
    poll_fds[0].events = POLLIN;

    connected = 0;

    while (!*stop) {
        poll_result = poll(poll_fds, connected + 1, 0);
        if (poll_result > 0) {
            if (poll_fds[0].revents & POLLIN) {

                addr_len = sizeof(client_addr);
                client_socket = accept(proxy_socket, (struct sockaddr *)&client_addr, &addr_len);

                if (client_socket == -1) {
                    perror("accept");
                    continue;
                }

                for (int i = 1; i < MAX_CLIENTS + 1;
                     i++) { // Recherche de places dans le tableau de pollfd.

                    if (poll_fds[i].fd ==
                        0) { // Acceptation de la connexion si une place est trouvée.
                        poll_fds[i].fd = client_socket;
                        poll_fds[i].events = POLLIN;

                        inet_ntop(AF_INET, &client_addr.sin_addr, clients[i - 1].ip_addr,
                                  INET_ADDRSTRLEN);
                        recv(poll_fds[i].fd, clients[i - 1].username, USERNAME_SIZE, 0);
                        printf("Connection from %s\n%s is connected\n\n", clients[i - 1].ip_addr,
                               clients[i - 1].username);

                        send(client_socket, &(char){SUCCESS}, 1, 0);

                        connected++;
                        break;
                    }
                    if (i == MAX_CLIENTS) // Refus de connexion dans le cas où le tableau est plein.
                        send(client_socket, &(char){FAIL}, 1, 0);
                }
            }

            for (int i = 1; i < MAX_CLIENTS + 1; i++) {
                // Attente de réception d'un message de la part du client i - 1.
                if (poll_fds[i].fd > 0 && poll_fds[i].revents & POLLIN) {
                    buf_size = recv(poll_fds[i].fd, clients[i - 1].buf, 2 * SIZE, 0);
                    if (buf_size > 0) {
                        clients[i - 1].buf_size = buf_size;

                        CLIENT_DISPLAY(clients[i - 1].username, (char *)(clients[i - 1].buf));

                        pthread_mutex_lock(a->m);
                        pthread_cond_signal(a->c);
                        *a->rcv = 1;
                        *a->client_idx = i - 1;
                        pthread_mutex_unlock(a->m);

                    } else { // En cas de déconnexion du client i - 1.
                        poll_fds[i].fd = 0;
                        poll_fds[i].events = 0;
                        poll_fds[i].revents = 0;
                        printf("%s is disconnected\n\n", clients[i - 1].username);
                        clients[i - 1].username[0] = 0;

                        connected--;
                    }
                }

                // Attente de réception d'un ACK par le thread server_side de la part du serveur et
                // envoie de cet ACK au client i - 1.
                if (poll_fds[i].fd > 0 && poll_fds[i].revents & POLLOUT) {
                    send(poll_fds[i].fd, &a->clients[i - 1].ack, 1, 0);
                    poll_fds[i].events = POLLIN;
                }
            }
        }
    }

    pthread_exit(NULL);
}
