#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#ifndef DECODER_H
#define DECODER_H

/**
 * @struct decode_args
 * @brief Arguments nécessaires au fonctionnement du communique avec le proxy
 */
struct decode_args {
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
 * @brief Fonction exécutée par le thread qui s'occupe de decoder le message reçu.
 * @param args struct decode_args.
 */
void *decode(void *args);

#endif