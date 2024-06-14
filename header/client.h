#include "display.h"
#include "utils.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef CLIENT_H
#define CLIENT_H

#define PORT 9990
#define SIZE 1024
#define USERNAME_SIZE 20
#define SUCCESS 1
#define FAIL 2

/**
 * @brief Affiche comment utiliser le programme.
 * @param argv0 Chemin vers le programme.
 */
void usage(char *argv0);

#endif