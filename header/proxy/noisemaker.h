#include "../utils.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef NOISEMAKER_H
#define NOISEMAKER_H

#define ERROR_RATE 1

/**
 * @brief Fonction exécutée par le thread qui s'occupe de bruiter les messages reçus.
 * @param args struct noisemk_args.
 */
void *noisemake(void *args);

#endif