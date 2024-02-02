#include "crc.h"

void init_CRC_Table(const uint8_t poly, uint8_t *table, const uint16_t table_size)
{
    uint8_t remainder;
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend << (WIDTH - 8);

        for (uint8_t b = 8; b > 0; --b)
        {
            if (remainder & TOP_BIT)
            {
                remainder = (remainder << 1) ^ poly;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
        table[dividend] = remainder;
    }
}

uint8_t get_CRC_Hash(const uint8_t *buf, const uint16_t len, const uint8_t *table)
{
    uint8_t data;
    uint8_t remainder = 0;

    for (uint16_t index = 0; index < len; ++index)
    {
        data = buf[index] ^ (remainder >> (WIDTH - 8));
        remainder = table[data] ^ (remainder << 8);
    }

    return (remainder);
}
