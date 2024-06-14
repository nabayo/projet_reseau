#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef CLIENT_SIDE_H
#define CLIENT_SIDE_H

#define MAX_CLIENTS 10

/**
 * @brief Fonction exécutée par le thread qui s'occupe de la communication avec les clients.
 * @param args struct args.
 */
void *wait_client(void *args);

#endif
