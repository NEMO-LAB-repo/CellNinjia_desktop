#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Metadata_Header_cb_dcall.h"



int S_D_V_decode(uint8_t *hex_data, size_t file_size, uint16_t logcode, int *index_in, uint64_t time_in_us_total, int crc_check, int check_format, int pkt_type, int msg_len);
long get_file_size(const char *filename);

void decode_B083(uint8_t *hex_data, size_t length, int *index);
void decode_B0A1(uint8_t *hex_data, size_t length, int *index);
void decode_B0B6(uint8_t *hex_data, size_t length, int *index);
void decode_B0CD(uint8_t *hex_data, size_t length, int *index);
void decode_B116(uint8_t *hex_data, size_t length, int *index);
void decode_B12A(uint8_t *hex_data, size_t length, int *index);
void decode_B144(uint8_t *hex_data, size_t length, int *index);
void decode_B167(uint8_t *hex_data, size_t length, int *index);
void decode_B168(uint8_t *hex_data, size_t length, int *index);
void decode_B169(uint8_t *hex_data, size_t length, int *index);
void decode_B16A(uint8_t *hex_data, size_t length, int *index);
void decode_B16C(uint8_t *hex_data, size_t length, int *index);
void decode_B16D(uint8_t *hex_data, size_t length, int *index);
void decode_B17A(uint8_t *hex_data, size_t length, int *index);
void decode_B17D(uint8_t *hex_data, size_t length, int *index);
void decode_B17F(uint8_t *hex_data, size_t length, int *index);
void decode_B181(uint8_t *hex_data, size_t length, int *index);
void decode_B184(uint8_t *hex_data, size_t length, int *index);
void decode_B18F(uint8_t *hex_data, size_t length, int *index);
void decode_B196(uint8_t *hex_data, size_t length, int *index);
void decode_B197(uint8_t *hex_data, size_t length, int *index);
void decode_B1B2(uint8_t *hex_data, size_t length, int *index);
void decode_B173(uint8_t *hex_data, size_t length, int *index, uint64_t time_in_us_total, uint16_t logcode);
void decode_B814(uint8_t *hex_data, size_t length, int *index, int msg_len);
void decode_B815(uint8_t *hex_data, size_t length, int *index);
void decode_B822(uint8_t *hex_data, size_t length, int *index);
void decode_B823(uint8_t *hex_data, size_t length, int *index);
void decode_B825(uint8_t *hex_data, size_t length, int *index);
void decode_B826(uint8_t *hex_data, size_t length, int *index);
void decode_B840(uint8_t *hex_data, size_t length, int *index);
void decode_B842(uint8_t *hex_data, size_t length, int *index);
void decode_B84B(uint8_t *hex_data, size_t length, int *index);
void decode_B84E(uint8_t *hex_data, size_t length, int *index);
void decode_B857(uint8_t *hex_data, size_t length, int *index);
void decode_B885(uint8_t *hex_data, size_t length, int *index);
void decode_B887(uint8_t *hex_data, size_t length, int *index);




#ifdef __cplusplus
}
#endif