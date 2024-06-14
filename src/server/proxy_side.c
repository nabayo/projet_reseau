#include "../../header/server/proxy_side.h"
#include "../../header/server/server.h"

void *wait_proxy(void *args)
{
    struct proxy_side_args *a;
    int server_socket;
    int proxy_socket;
    int poll_result;
    int connected;
    int *stop;
    struct sockaddr_in proxy_addr;
    socklen_t addr_len;
    int username_size;
    int buf_size;
    unsigned char *buf;

    unsigned char message[SIZE] = {0};
    char proxy_ip[INET_ADDRSTRLEN] = {0};
    char username[USERNAME_SIZE] = {0};

    a = args;

    server_socket = a->server_socket;
    stop = a->stop;
    buf = a->buf;

    struct pollfd poll_fds[2] = {0};

    poll_fds[0].fd = server_socket;
    poll_fds[0].events = POLLIN;

    connected = 0;

    while (!*stop) {
        poll_result = poll(poll_fds, connected + 1, TIMEOUT);
        if (poll_result > 0) {
            if (poll_fds[0].revents & POLLIN) {

                addr_len = sizeof(proxy_addr);
                proxy_socket = accept(server_socket, (struct sockaddr *)&proxy_addr, &addr_len);
                if (proxy_socket == -1) {
                    perror("accept");
                    continue;
                }

                if (poll_fds[1].fd == 0) { // Acceptation de la connexion.

                    poll_fds[1].fd = proxy_socket;
                    poll_fds[1].events = POLLIN;

                    send(proxy_socket, &(char){SUCCESS}, 1, 0);
                    inet_ntop(AF_INET, &proxy_addr.sin_addr, proxy_ip, INET_ADDRSTRLEN);
                    printf("Connection from %s\n\n", proxy_ip);

                    connected++;
                } else { // En cas de connexion alors qu'un proxy est déjà connecté. Refus de
                         // connexion.
                    send(proxy_socket, &(char){FAIL}, 1, 0);
                }
            }

            if (poll_fds[1].fd > 0 &&
                poll_fds[1].revents & POLLIN) { // Attente de réception d'un message.
                username_size = recv(poll_fds[1].fd, username, USERNAME_SIZE, 0);

                if (username_size > 0) {
                    buf_size = recv(poll_fds[1].fd, buf, 2 * SIZE, 0);


                    memcpy(message, buf, SIZE);
                    PROCLI_DISPLAY(username, message);

                    pthread_mutex_lock(a->m);
                    pthread_cond_signal(a->c_rcv);
                    *a->buf_size = buf_size;
                    *a->rcv = 1;
                    pthread_mutex_unlock(a->m);

                    poll_fds[1].events = POLLOUT;

                } else { // En cas de déconnexion du proxy.
                    poll_fds[1].fd = 0;
                    poll_fds[1].events = 0;
                    poll_fds[1].revents = 0;
                    printf("Proxy is disconnected\n");
                    close(proxy_socket);
                    connected--;
                }
            }

            // Attente de décodage du message pour envoyer l'ACK au proxy.
            if (poll_fds[1].fd > 0 && poll_fds[1].revents & POLLOUT) {
                pthread_mutex_lock(a->m);
                while (!*a->rdy)
                    pthread_cond_wait(a->c_rdy, a->m);
                *a->rdy = 0;
                pthread_mutex_unlock(a->m);

                send(poll_fds[1].fd, a->ack, 1, 0);
                poll_fds[1].events = POLLIN;
            }
        }
    }
    close(server_socket);
    pthread_exit(NULL);
}