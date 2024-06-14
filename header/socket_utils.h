#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

/**
 * @brief Créer un socket récepteur.
 * @param port Port utilisé par la socket.
 * @return Renvoie le socket récepteur.
 */
int create_socket(int port);

#endif