#include "../../header/server/server.h"
#include "../../header/server/decoder.h"
#include "../../header/server/proxy_side.h"
#include "../../header/socket_utils.h"

int main()
{
    int server_socket;
    server_socket = create_socket(PORT);
    printf("Server start\n");
    int stop;
    char cmd[64];
    pthread_t proxy_side;
    pthread_t decoder;
    struct proxy_side_args p_args;
    struct decode_args d_args;
    pthread_mutex_t m;
    pthread_cond_t c_rcv;
    pthread_cond_t c_rdy;
    int rcv;
    int rdy;
    char ack;
    int buf_size;

    unsigned char buf[2 * SIZE] = {0};
    rcv = 0;
    rdy = 0;
    stop = 0;
    ack = 0;

    // Initialisation des threads.

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c_rcv, NULL);
    pthread_cond_init(&c_rdy, NULL);

    p_args.server_socket = server_socket;
    p_args.stop = &stop;
    p_args.buf = buf;
    p_args.m = &m;
    p_args.c_rcv = &c_rcv;
    p_args.c_rdy = &c_rdy;
    p_args.rcv = &rcv;
    p_args.rdy = &rdy;
    p_args.ack = &ack;
    p_args.buf_size = &buf_size;

    d_args.stop = &stop;
    d_args.buf = buf;
    d_args.m = &m;
    d_args.c_rcv = &c_rcv;
    d_args.c_rdy = &c_rdy;
    d_args.rcv = &rcv;
    d_args.rdy = &rdy;
    d_args.ack = &ack;
    d_args.buf_size = &buf_size;

    pthread_create(&proxy_side, NULL, wait_proxy, &p_args);
    pthread_create(&decoder, NULL, decode, &d_args);

    while (!stop) {
        scanf("%s", cmd);
        if (strncmp(cmd, "/quit", 5) == 0) {
            pthread_cancel(decoder);
            stop = 1;
        }
    }

    // Libération de la mémoire.

    pthread_join(proxy_side, NULL);
    pthread_join(decoder, NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c_rcv);
    pthread_cond_destroy(&c_rdy);

    printf("Server end\n");

    close(server_socket);

    return 0;
}