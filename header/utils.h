#include <stdint.h>
#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

#define POLY_GEN 0b110111001

/**
 * @brief Mets le n-ième bit à 1.
 * @param n n-ième bit à modifier (en partant de 0).
 * @param m Mot de 8 bits à modifier.
 * @return Renvoie un mot de 8 bits.
 */
uint8_t set_nth_bit(int n, uint8_t m);

/**
 * @brief Lis le n-ième bit de m.
 * @param n n-ième bit à modifier (en partant de 0).
 * @param m Mot de 8 bits.
 * @return Renvoie le n-ième bit de m.
 */
uint8_t read_nth_bit(int n, uint8_t m);

/**
 * @brief Change le n-ième bit de m.
 * @param n n-ième bit à modifier (en partant de 0).
 * @param m Mot de 8 bits à modifier.
 * @return Renvoie un mot de 8 bits.
 */
uint8_t chg_nth_bit(int n, uint8_t m);

/**
 * @brief Affiche les k premier bits de m.
 * @param k Nombre de bit à afficher.
 * @param m Mot de 8 bits à afficher.
 */
void print_word(int k, uint8_t m);

/**
 * @brief Calcule le nombre de chiffres significatifs de la forme binaire d'un mot.
 * @param bin Mot de 8 bits.
 * @return Renvoie un entier positif
 */
int bin_digits(uint16_t bin);

/**
 * @brief Donne le CRC
 * @param divd Message d'origine.
 * @param divr Polynôme générateur.
 * @return Renvoie un mot de 8 bits.
 */
uint8_t crc(uint16_t divd, uint16_t divr);

#endif
