#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SERVER_SIDE_H
#define SERVER_SIDE_H

#define HOST_SERVER "127.0.0.1"
#define PORT_SERVER 9980

/**
 * @brief Effectue la connexion au server.
 * @param dest_socket Socket de destination.
 */
void connection(int dest_socket);

/**
 * @brief Fonction exécutée par le thread qui s'occupe de la communication vers le serveur.
 * @param args struct args.
 */
void *talk_server(void *args);

#endif
