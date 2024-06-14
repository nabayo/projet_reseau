#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef PROXY_SIDE_H
#define PROXY_SIDE_H

#define TIMEOUT 5000

/**
 * @struct proxy_side_args
 * @brief Arguments nécessaires au fonctionnement du communique avec le proxy
 */
struct proxy_side_args {
    int server_socket;  /** Socket serveur. */
    int *stop;          /** Pointeur vers stop (qui vaut 0 ou 1 lorsqu'on écrit /quit). */
    unsigned char *buf; /** Message reçu du proxy. */
    int *buf_size;      /** Taille du message reçu. */
    char *ack;
    int *rcv; /** Pointeur vers une variable qui indique que le message est prêt à être décoder. */
    int *rdy; /** Pointeur vers une variable qui indique que le message est prêt à être envoyé au
                 proxy. */
    pthread_mutex_t *m;    /** Mutex. */
    pthread_cond_t *c_rcv; /** Condition relative à rcv. */
    pthread_cond_t *c_rdy; /** Condition relative à rdy. */
};

/**
 * @brief Fonction exécutée par le thread qui s'occupe de la communication avec le proxy.
 * @param args struct proxy_side_args.
 */
void *wait_proxy(void *args);

#endif