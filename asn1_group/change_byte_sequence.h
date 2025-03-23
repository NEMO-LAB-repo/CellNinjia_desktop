#include <stdint.h>
#include <stdio.h>

uint16_t convert_2_bytes_little_to_big(uint16_t little_endian);
uint32_t convert_3_bytes_little_to_big(uint32_t little_endian);
uint32_t convert_4_bytes_little_to_big(uint32_t little_endian);
uint64_t convert_8_bytes_little_to_big(uint64_t little_endian);
void convert_endianess(uint8_t* hex_data, int *index, size_t length);
void print_hex(const uint8_t *data, size_t start, size_t end);