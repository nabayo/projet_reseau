#include "../../header/proxy/noisemaker.h"
#include "../../header/proxy/proxy.h"

void *noisemake(void *args)
{
    srand(time(NULL));
    struct noisemk_args *a;
    int *stop;
    int client_idx;
    int buf_size;

    a = args;
    stop = a->stop;

    while (!*stop) {

        pthread_mutex_lock(a->m);
        while (!*a->rcv) // Attente de réception d'un message par le thread client_side.
            pthread_cond_wait(a->c_rcv, a->m);

        *a->rcv = 0;

        client_idx = *a->client_idx;

        buf_size = a->clients[client_idx].buf_size;

        // Bruitage aléatoire des caractères

        for (int i = 0; i < buf_size / 2; i++) {
            if (rand() % 100 == 0) {
                int changed_bits[ERROR_RATE]; // Indices des bits changés dans le caractère (0-8
                                              // dans le caractère, 9-16 dans le crc)
                int count = 0;
                while (count < ERROR_RATE) {
                    int bit = rand() % 8;
                    int crc = rand() % 2;
                    bool found = false;

                    // Recherche si le bit a déjà été modifié

                    for (int j = 0; j < ERROR_RATE; j++) {
                        if (changed_bits[j] == (crc + 1) * bit) {
                            found = true;
                            break;
                        }
                    }

                    // Modification du bit

                    if (!found) {
                        changed_bits[count] = (crc + 1) * bit;
                        a->clients[client_idx].buf[i + crc * (buf_size / 2)] =
                            chg_nth_bit(bit, a->clients[client_idx].buf[i + crc * (buf_size / 2)]);
                        count++;
                    }
                }
            }
        }

        pthread_cond_signal(a->c_noisy);
        *a->noisy = 1;
        pthread_mutex_unlock(a->m);
    }

    pthread_exit(NULL);
}