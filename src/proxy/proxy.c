#include "../../header/proxy/proxy.h"
#include "../../header/proxy/client_side.h"
#include "../../header/proxy/noisemaker.h"
#include "../../header/proxy/server_side.h"
#include "../../header/socket_utils.h"

int main()
{
    int proxy_socket, server_socket;
    pthread_t client_side, server_side, noisemaker;
    struct args c_args;
    struct args s_args;
    struct noisemk_args n_args;
    int stop;
    char cmd[64];
    pthread_mutex_t m;
    pthread_cond_t c_rcv;
    pthread_cond_t c_noisy;
    int rcv;
    int noisy;
    int client_idx;

    struct pollfd poll_fds[MAX_CLIENTS + 1] = {0};
    struct client_info clients[MAX_CLIENTS] = {0};

    stop = 0;
    rcv = 0;
    noisy = 0;

    // Initialisation des threads.

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c_rcv, NULL);
    pthread_cond_init(&c_noisy, NULL);

    proxy_socket = create_socket(PORT);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    c_args.socket = proxy_socket;
    c_args.stop = &stop;
    c_args.poll_fds = poll_fds;
    c_args.clients = clients;
    c_args.m = &m;
    c_args.c = &c_rcv;
    c_args.rcv = &rcv;
    c_args.client_idx = &client_idx;

    n_args.stop = &stop;
    n_args.clients = clients;
    n_args.client_idx = &client_idx;
    n_args.m = &m;
    n_args.c_rcv = &c_rcv;
    n_args.c_noisy = &c_noisy;
    n_args.rcv = &rcv;
    n_args.noisy = &noisy;

    s_args.socket = server_socket;
    s_args.stop = &stop;
    s_args.poll_fds = poll_fds;
    s_args.clients = clients;
    s_args.m = &m;
    s_args.c = &c_noisy;
    s_args.rcv = &noisy;
    s_args.client_idx = &client_idx;

    printf("Proxy start\n");

    pthread_create(&server_side, NULL, talk_server, &s_args);
    pthread_create(&noisemaker, NULL, noisemake, &n_args);
    pthread_create(&client_side, NULL, wait_client, &c_args);

    while (!stop) {
        scanf("%s", cmd);
        if (strncmp(cmd, "/quit", 5) == 0) {
            pthread_cancel(noisemaker);
            stop = 1;
        }
    }

    // Libération de la mémoire.

    pthread_join(client_side, NULL);
    pthread_join(server_side, NULL);
    pthread_join(noisemaker, NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c_rcv);
    pthread_cond_destroy(&c_noisy);

    printf("Proxy end\n");

    close(proxy_socket);

    return 0;
}
