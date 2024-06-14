#include "../../header/server/decoder.h"
#include "../../header/server/server.h"

void *decode(void *args)
{
    int *stop;
    unsigned char *buf;
    struct decode_args *a;
    unsigned char buffer[2];
    a = args;

    stop = a->stop;
    buf = a->buf;

    while (!*stop) {

        pthread_mutex_lock(a->m);
        while (!*a->rcv) // Attente de réception d'un message par le thread proxy_side.
            pthread_cond_wait(a->c_rcv, a->m);
        *a->rcv = 0;
        pthread_mutex_unlock(a->m);

        // Division euclidienne sur le message reçu pour verifier l'intégrité du message.
        // Met ACK à 1 si le message est le bon et ACK à 0 sinon.

        *a->ack = 1;

        for (int i = 0; i < *a->buf_size / 2; i++) {
            memcpy(buffer, &buf[i], 1);
            memcpy(buffer + 1, &buf[i + (*a->buf_size / 2)], 1);
            if (crc((((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1]), POLY_GEN) != 0) {
                *a->ack = 0;
                break;
            }
        }

        pthread_mutex_lock(a->m);
        pthread_cond_signal(a->c_rdy);
        *a->rdy = 1;
        pthread_mutex_unlock(a->m);
    }

    pthread_exit(NULL);
}