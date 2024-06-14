#include "../display.h"
#include <arpa/inet.h>
#include <pthread.h>

#ifndef PROXY_H
#define PROXY_H

#define SUCCESS 1
#define FAIL 2
#define PORT 9990
#define SIZE 1024
#define USERNAME_SIZE 20

/**
 * @struct client_info
 * @brief Contient les informations des clients connectés au proxy.
 */
struct client_info {
    char ip_addr[INET_ADDRSTRLEN];    /** Adresse IPv4 du client. */
    char username[USERNAME_SIZE + 1]; /** Nom d'utilisateur du client. */
    unsigned char buf[2 * SIZE];      /** Message reçu du client / Message envoyé au client. */
    int buf_size;                     /** Taille du message reçu. */
    char ack;                         /** Tableau de ACK. */
};

/**
 * @struct args
 * @brief Arguments nécessaires au fonctionnement du thread qui recoit les clients
 * et du thread qui émet au serveur.
 */
struct args {
    int socket;                  /** Socket proxy / Socket serveur. */
    int *stop;                   /** Pointeur vers stop (qui vaut 0 ou 1 lorsqu'on écrit /quit). */
    struct pollfd *poll_fds;     /** Tableau de pollfd. */
    struct client_info *clients; /** Tableau de clients. */
    int *client_idx; /** Pointeur vers l'index du dernier client dont le proxy à reçu le message. */
    pthread_mutex_t *m; /** Mutex. */
    pthread_cond_t *c;  /** Condition. */
    int *rcv; /** Pointeur vers une variable qui indique que le message d'un client est reçu
                  ou qu'il est prêt à être envoyé au serveur (c-à-d qu'il a été bruité par le
                 proxy). */
};

/**
 * @struct noisemk_args
 * @brief Arguments nécessaires au fonctionnement du thread qui bruite les messages.
 */
struct noisemk_args {
    struct client_info *clients; /** Tableau de client. */
    int *client_idx; /** Pointeur vers l'index du dernier client dont le proxy à reçu le message. */
    int *stop;       /** Pointeur vers stop (qui vaut 0 ou 1 lorsqu'on écrit /quit). */
    pthread_mutex_t *m;      /** Mutex. */
    pthread_cond_t *c_rcv;   /** Condition relative à rcv. */
    pthread_cond_t *c_noisy; /** Condition relative à noisy. */
    int *rcv;   /** Pointeur vers une variable qui indique que le message d'un client est reçu. */
    int *noisy; /** Pointeur vers une variable qui indique qu'un message
                    est prêt à être envoyé au serveur (c-à-d qu'il a été bruité par le proxy).*/
};

#endif
