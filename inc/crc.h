#ifndef CRC_H
#define CRC_H

#include <stdint.h>

#define WIDTH 8
#define TOP_BIT (1 << (WIDTH - 1))

void init_CRC_Table(const uint8_t poly, uint8_t *table, const uint16_t table_size);
uint8_t get_CRC_Hash(const uint8_t *buf, const uint16_t len, const uint8_t *table);

#endif // CRC_H
