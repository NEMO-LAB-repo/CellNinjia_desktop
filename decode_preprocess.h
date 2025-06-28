#ifndef LOG_PROCESS_H
#define LOG_PROCESS_H

#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
#include "./asn1_group/host.h"

bool generate_diag_config(int serial_fd, const char* msgtype_list[], int logcode_num);

bool send_msg_to_serial(int serial_fd, const char* mask_msg, int length);

void enter_binary(std::vector<uint8_t>& data);
void decode();

#endif // LOG_PROCESS_H