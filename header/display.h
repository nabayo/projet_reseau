#include <stdio.h>

#ifndef DISPLAY_H
#define DISPLAY_H

/** Affichage message avec en-tête serveur.*/
#define SERVER_DISPLAY(buf) printf("\e[1;37;41m Server \e[0;31m\e[0m %s\n", buf);

/** Affichage message avec en-tête proxy.*/
#define PROXY_DISPLAY(buf) printf("\e[1;37;45m Proxy \e[0;35m\e[0m %s\n", buf);

/** Affichage message avec client serveur.*/
#define CLIENT_DISPLAY(username, buf) printf("\e[1;37;44m %s \e[0;34m\e[0m %s\n", username, buf);

/** Affichage message avec client serveur.*/
#define PROCLI_DISPLAY(username, buf)                                                              \
    printf("\e[1;37;45m Proxy \e[0m\e[0;35;44m\e[1;37;44m %s \e[0;34m\e[0m %s\n", username,  \
           buf);

/** Message connexion réussie avec le serveur.*/
#define S_SERVER_MSG "Successfully connected\n"

/** Message connexion échouée avec le serveur.*/
#define F_SERVER_MSG "A proxy is already connected\nConnection failed\n"

/** ACK.*/
#define ACK_MSG "Received successfully.\n"

/** NACK.*/
#define NACK_MSG "Error on message.\n"

/** Message renvoyé.*/
#define RESEND_MSG "Message resent.\n"

/** Message connexion réussie avec le proxy.*/
#define WELCOME_MSG "\n \e[0;32m\e[1;37;42m %s \e[0;32m\e[0m\n\n", "Welcome !"

/** Message connexion échouée avec le proxy car le proxy est plein.*/
#define AFULL_MSG "\n \e[0;30m\e[1;37;40m %s \e[0;30m\e[0m\n\n", "Sorry, we are full."

#endif