#include "../header/utils.h"

uint8_t set_nth_bit(int n, uint8_t m) { return 1 << (7 - n) | m; }

uint8_t read_nth_bit(int n, uint8_t m) { return (m >> (7 - n)) & 1; }

uint8_t chg_nth_bit(int n, uint8_t m) { return 1 << (7 - n) ^ m; }

void print_word(int k, uint8_t m)
{
    printf("%s", "0b");
    for (int i = 0; i < k; i++) {
        printf("%i", (m >> (7 - i)) & 1);
    }
    printf("%c", '\n');
}

int bin_digits(uint16_t bin)
{
    int digits = 0;
    while (bin) {
        bin = bin >> 1;
        digits++;
    }
    return digits;
}

uint8_t crc(uint16_t divd, uint16_t divr)
{
    int divd_digits, divr_digits;
    divd_digits = bin_digits(divd);
    divr_digits = bin_digits(divr);
    while (divd_digits >= divr_digits) {
        divd = divd ^ (divr << (divd_digits - divr_digits));
        divd_digits = bin_digits(divd);
        divr_digits = bin_digits(divr);
    }
    return divd;
}
