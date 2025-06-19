#include "decode_preprocess.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <sstream>

constexpr double PER_SECOND = 52428800.0;
constexpr double PER_USECOND = PER_SECOND / 1.0e6;
constexpr int ESCAPE_XOR = 0x20;
typedef unsigned short     UINT16;
typedef unsigned char      UINT8;

static std::string current_buffer;

int get_item_id(int type_id) 
{
    constexpr int ITEM_ID_MASK = 0x00000FFF;
    return type_id & ITEM_ID_MASK;
}

int get_equip_id(int type_id) 
{
    constexpr int EQUIP_ID_MASK = 0x0000F000;
    return (type_id & EQUIP_ID_MASK) >> 12;
}

static UINT16 calc_crc (UINT8 *data, size_t len, UINT16 crc)
{
    static const UINT16 table[256] = {
    0x0000U,0x1189U,0x2312U,0x329BU,0x4624U,0x57ADU,0x6536U,0x74BFU,
    0x8C48U,0x9DC1U,0xAF5AU,0xBED3U,0xCA6CU,0xDBE5U,0xE97EU,0xF8F7U,
    0x1081U,0x0108U,0x3393U,0x221AU,0x56A5U,0x472CU,0x75B7U,0x643EU,
    0x9CC9U,0x8D40U,0xBFDBU,0xAE52U,0xDAEDU,0xCB64U,0xF9FFU,0xE876U,
    0x2102U,0x308BU,0x0210U,0x1399U,0x6726U,0x76AFU,0x4434U,0x55BDU,
    0xAD4AU,0xBCC3U,0x8E58U,0x9FD1U,0xEB6EU,0xFAE7U,0xC87CU,0xD9F5U,
    0x3183U,0x200AU,0x1291U,0x0318U,0x77A7U,0x662EU,0x54B5U,0x453CU,
    0xBDCBU,0xAC42U,0x9ED9U,0x8F50U,0xFBEFU,0xEA66U,0xD8FDU,0xC974U,
    0x4204U,0x538DU,0x6116U,0x709FU,0x0420U,0x15A9U,0x2732U,0x36BBU,
    0xCE4CU,0xDFC5U,0xED5EU,0xFCD7U,0x8868U,0x99E1U,0xAB7AU,0xBAF3U,
    0x5285U,0x430CU,0x7197U,0x601EU,0x14A1U,0x0528U,0x37B3U,0x263AU,
    0xDECDU,0xCF44U,0xFDDFU,0xEC56U,0x98E9U,0x8960U,0xBBFBU,0xAA72U,
    0x6306U,0x728FU,0x4014U,0x519DU,0x2522U,0x34ABU,0x0630U,0x17B9U,
    0xEF4EU,0xFEC7U,0xCC5CU,0xDDD5U,0xA96AU,0xB8E3U,0x8A78U,0x9BF1U,
    0x7387U,0x620EU,0x5095U,0x411CU,0x35A3U,0x242AU,0x16B1U,0x0738U,
    0xFFCFU,0xEE46U,0xDCDDU,0xCD54U,0xB9EBU,0xA862U,0x9AF9U,0x8B70U,
    0x8408U,0x9581U,0xA71AU,0xB693U,0xC22CU,0xD3A5U,0xE13EU,0xF0B7U,
    0x0840U,0x19C9U,0x2B52U,0x3ADBU,0x4E64U,0x5FEDU,0x6D76U,0x7CFFU,
    0x9489U,0x8500U,0xB79BU,0xA612U,0xD2ADU,0xC324U,0xF1BFU,0xE036U,
    0x18C1U,0x0948U,0x3BD3U,0x2A5AU,0x5EE5U,0x4F6CU,0x7DF7U,0x6C7EU,
    0xA50AU,0xB483U,0x8618U,0x9791U,0xE32EU,0xF2A7U,0xC03CU,0xD1B5U,
    0x2942U,0x38CBU,0x0A50U,0x1BD9U,0x6F66U,0x7EEFU,0x4C74U,0x5DFDU,
    0xB58BU,0xA402U,0x9699U,0x8710U,0xF3AFU,0xE226U,0xD0BDU,0xC134U,
    0x39C3U,0x284AU,0x1AD1U,0x0B58U,0x7FE7U,0x6E6EU,0x5CF5U,0x4D7CU,
    0xC60CU,0xD785U,0xE51EU,0xF497U,0x8028U,0x91A1U,0xA33AU,0xB2B3U,
    0x4A44U,0x5BCDU,0x6956U,0x78DFU,0x0C60U,0x1DE9U,0x2F72U,0x3EFBU,
    0xD68DU,0xC704U,0xF59FU,0xE416U,0x90A9U,0x8120U,0xB3BBU,0xA232U,
    0x5AC5U,0x4B4CU,0x79D7U,0x685EU,0x1CE1U,0x0D68U,0x3FF3U,0x2E7AU,
    0xE70EU,0xF687U,0xC41CU,0xD595U,0xA12AU,0xB0A3U,0x8238U,0x93B1U,
    0x6B46U,0x7ACFU,0x4854U,0x59DDU,0x2D62U,0x3CEBU,0x0E70U,0x1FF9U,
    0xF78FU,0xE606U,0xD49DU,0xC514U,0xB1ABU,0xA022U,0x92B9U,0x8330U,
    0x7BC7U,0x6A4EU,0x58D5U,0x495CU,0x3DE3U,0x2C6AU,0x1EF1U,0x0F78U,
    };

    crc = crc ^ 0xFFFFU;
    while (len > 0)
    {
        crc = table[*data ^ (UINT8)crc] ^ (crc >> 8);
        data++;
        len--;
    }
    crc = crc ^ 0xFFFFU;
    return crc;
}

std::string encode_hdlc_frame (const char *payld, int length) 
{
    std::string result;
    UINT16 crc16 = calc_crc((UINT8 *)payld, length, 0);
    for (int i = 0; i < length + 2; i++) {
        char c;
        if (i < length)
        {
            c = payld[i];
        }
        else if (i == length)   
        {
            c = crc16 & 0x00FF;
        }
        else
        {
            c = (crc16 & 0xFF00) >> 8;
        }

        if (c == '\x7d' || c == '\x7e') {
            result.append(1, '\x7d');
            result.append(1, c ^ ESCAPE_XOR); 
        } else
            result.append(1, c);
    }
    result.append(1, '\x7e');
    return result;
}

bool send_msg_to_serial(int serial_fd, const char* msg, int length) 
{
    std::string Config = encode_hdlc_frame(msg, length);

    /*Debug log cfg*/
    // FILE* log_file = fopen("sent_cfg.log", "a+");
    // if (log_file) {
    //     fprintf(log_file, "--- Frame (size: %zu bytes) ---\n", frame.size());
    //     fprintf(log_file, "HEX: ");
    //     for (size_t i = 0; i < frame.size(); ++i) {
    //         fprintf(log_file, "%02X ", (unsigned char)frame[i]);
            
    //         if ((i + 1) % 16 == 0 && i < frame.size() - 1) {
    //             fprintf(log_file, "\n     ");
    //         }
    //     }
    //     fprintf(log_file, "\n\n");
    //     fclose(log_file);
    // }
    ssize_t bytes_written = write(serial_fd, Config.c_str(), Config.size());

    if (bytes_written < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
        return false;
    }
    
    return true;
}

// Only need to setmask in config for USB mode
bool generate_diag_config(int serial_fd, uint16_t* logcode, int logcode_num)
{
    // output file
    // std::ofstream cfg_file("cfg_generate.txt", std::ios::out);
    // if (!cfg_file.is_open()) {
    //     std::cerr << "Failed to open cfg_generate.txt for writing" << std::endl;
    //     return false;
    // }

    /*Disable part*/ 
    int buf_disable_size = sizeof(char) * 4 + sizeof(int);
    char* buf_disable = new char[buf_disable_size];
    buf_disable[0] = 115;
    buf_disable[1] = 0;
    buf_disable[2] = 0;
    buf_disable[3] = 0;
    *((int *)(buf_disable + 4)) = (int) 0;
    
    bool send_flag_disable = send_msg_to_serial(serial_fd, buf_disable, buf_disable_size);
    std::cout << "Sent disable config\n" << std::endl;

    //set_mask
    int equip_id = -1;
    int highest = 0;
    
    for(int i = 0; i < logcode_num; i++)
    {
        int id = logcode[i];
        int e = get_equip_id(id);
        if (equip_id == -1) 
        {
            equip_id = e;
            highest = (highest > get_item_id(id)) ? highest : get_item_id(id);
        } 
        else if (equip_id == e) 
        {
            highest = (highest > get_item_id(id)) ? highest : get_item_id(id);
        } 
        else 
        {
            equip_id = -1;
            break;
        }
    }
    switch (equip_id) 
    {
        case 0x00000001:    // CDMA, _1xEV, ......
            highest = 0x00000FD3;
            break;
        case 0x00000004:    // WCDMA, ......
            highest = 0x00000920;
            break;
        case 0x00000007:    // UMTS, ......
            highest = 0x00000B56;
            break;
        case 0x0000000b:    // LTE, 5G ......
            highest = 0x00000FFF;
            break;
        default:
            break;
    }
    int mask_len = (highest / 8) + 1;
    int buf_size = sizeof(char) * 4 + sizeof(int) * 3 + mask_len;
    
    char* buf = new char[buf_size];
    buf[0] = 115; // 0x73
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    *((int *)(buf + 4)) = 3;
    *((int *)(buf + 8)) = equip_id;
    *((int *)(buf + 12)) = highest + 1;
    char* mask = buf + 16;
    std::memset(mask, 0, mask_len);
    
    for (int i = 0; i < logcode_num; i++) 
    {
        int id = logcode[i] & 0x0FFF;
        mask[id / 8] |= 1 << (id % 8);
    }

    // cfg_file.close();
    
    bool send_flag = send_msg_to_serial(serial_fd, buf, buf_size);
    std::cout << "Sent Set_mask config\n" << std::endl;
    
    delete[] buf;
    delete[] buf_disable;

    return send_flag;
}

void enter_binary(std::vector<uint8_t>& data)
{
    // append new log file to global string current_buffer
    current_buffer.append(reinterpret_cast<const char*>(data.data()), data.size());
    
    // output the binary as for now
    // std::ofstream outfile("current_buffer.txt", std::ios::app);
    // if (outfile.is_open()) 
    // {
    //     for (size_t i = 0; i < current_buffer.size(); ++i) {
    //         outfile << std::hex << std::uppercase << std::setw(2) 
    //                 << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(current_buffer[i])) << " ";
            
    //         if ((i + 1) % 64 == 0) {
    //             outfile << std::endl;
    //         }
    //     }
        
    //     if (current_buffer.size() % 64 != 0) {
    //         outfile << std::endl;
    //     }
        
    //     outfile.close();
    //     std::cout << "Write (" << current_buffer.size() << " bytes to current_buffer.txt)" << std::endl;
    // }
}

static void unescape (std::string& frame) 
{
    bool is_esc = false;
    std::string output;
    for (size_t i = 0; i < frame.size(); i++) {
        if (is_esc) {
            output.append(1, char(frame[i] ^ ESCAPE_XOR));
            is_esc = false;
        } else if (frame[i] == '\x7d') {
            is_esc = true;
        } else {
            output.append(1, frame[i]);
        }
    }
    frame = output;
    return;
}

bool get_next_frame(std::string& output_frame, bool& crc_correct) 
{
    size_t delim = current_buffer.find('\x7e');
    if (delim == std::string::npos)
        return false;
    
    output_frame = current_buffer.substr(0, delim);
    current_buffer.erase(0, delim + 1);
    unescape(output_frame);
    
    // output the frame
    // std::ofstream outfile("frame_output.txt", std::ios::app);
    // if (outfile.is_open()) {
    //     outfile << "--- New Frame ---" << std::endl;
    //     outfile << "Raw frame size: " << output_frame.size() << " bytes" << std::endl;
    
    //     for (size_t i = 0; i < output_frame.size(); ++i) {
    //         outfile << std::hex << std::uppercase << std::setw(2) 
    //                 << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(output_frame[i])) << " ";
            
    //         if ((i + 1) % 30 == 0) {
    //             outfile << std::endl;
    //         }
    //     }
        
    //     if (output_frame.size() % 30 != 0) {
    //         outfile << std::endl;
    //     }
    // }
    
    // if (output_frame.size() <= 2) {
    //     crc_correct = false;

    //     if (outfile.is_open()) {
    //         outfile << "CRC FALSE - Frame too short" << std::endl;
    //         outfile << "----------------------------------------" << std::endl;
    //         outfile.close();
    //     }
        
    //     return true;
    // }
    
    // data crc
    UINT16 b1 = output_frame[output_frame.size() - 1] & 0xFF;
    UINT16 b2 = output_frame[output_frame.size() - 2] & 0xFF;
    UINT16 frame_crc16 = (b1 << 8) + b2;
    
    // keep the original crc
    // UINT16 original_crc16 = frame_crc16;
    
    //compute crc
    output_frame.erase(output_frame.size() - 2);
    UINT16 crc16 = calc_crc((UINT8 *) output_frame.c_str(), output_frame.size(), 0);
    crc_correct = (frame_crc16 == crc16);

    if (crc_correct == false) {
        std::cout << "CRC FALSE - Checksum mismatch" << std::endl;
    }
    
    // // if crc fail, output it 
    // if (outfile.is_open()) {
    //     // outfile << "Frame CRC: 0x" << std::hex << std::uppercase << std::setw(4) 
    //     //         << std::setfill('0') << original_crc16 << std::endl;
    //     // outfile << "Calc CRC: 0x" << std::hex << std::uppercase << std::setw(4) 
    //     //         << std::setfill('0') << crc16 << std::endl;
        
    //     if (crc_correct == false) {
    //         outfile << "CRC FALSE - Checksum mismatch" << std::endl;
    //     } else {
    //         // outfile << "CRC CORRECT" << std::endl;
    //     }
        
    //     // outfile << "----------------------------------------" << std::endl;
    //     outfile.close();
    // }
    
    return true;
}

void check_frame_format (std::string& output_frame) 
{
    size_t delim = output_frame.find("\x98\x01\x00\x00\x01\00\x00\x00");
    if (delim == 0) 
    {
        output_frame.erase(0, 8);
    }
    // else
    // {
    //     std::ofstream outfile("frame_output.txt", std::ios::app); 
    //     if (outfile.is_open()) {
    //         outfile << "check format fail" << std::endl;
    //         outfile.close();
    //     }
    // }
}

bool is_log_packet(const char *b, size_t length) {
    return length >= 2 && b[0] == '\x10';
}

std::string compute_timestamp(uint64_t timestamp)
{    
    int seconds = static_cast<int>(static_cast<double>(timestamp) / PER_SECOND);
    int useconds = static_cast<int>(static_cast<double>(timestamp) / PER_USECOND - static_cast<double>(seconds) * 1.0e6);
    
    // Define the epoch (January 5, 1980)
    std::tm epoch_tm = {};
    epoch_tm.tm_year = 80;  // Years since 1900
    epoch_tm.tm_mon = 0;    // Month (0-11)
    epoch_tm.tm_mday = 5;   // Day of month (1-31)
    epoch_tm.tm_hour = 19;
    epoch_tm.tm_min = 0;
    epoch_tm.tm_sec = 0;
    
    // Convert epoch to time_t (seconds since January 1, 1970)
    std::time_t epoch_time = std::mktime(&epoch_tm);
    
    // Add the calculated seconds to get the final time
    std::time_t final_time = epoch_time + seconds;
    
    // Convert to calendar time
    std::tm* calendar_time = std::localtime(&final_time);
    
    // Format the result as a string (more compact format)
    std::stringstream result;
    result << calendar_time->tm_year + 1900 << "-"
           << std::setfill('0') << std::setw(2) << calendar_time->tm_mon + 1 << "-"
           << std::setfill('0') << std::setw(2) << calendar_time->tm_mday << " "
           << std::setfill('0') << std::setw(2) << calendar_time->tm_hour << ":"
           << std::setfill('0') << std::setw(2) << calendar_time->tm_min << ":"
           << std::setfill('0') << std::setw(2) << calendar_time->tm_sec << "."
           << std::setfill('0') << std::setw(6) << useconds;
    
    return result.str();
}


//decode each msg header msg_len, logcode, timestamp and version
void decode_header(const char *output_frame, int start_index, uint64_t *msg_header)
{
    int log_msg_len_start_index = start_index;
    uint16_t msglen_1 = (uint16_t)(output_frame[log_msg_len_start_index] & 0xFF);
    uint16_t msglen_2 = (uint16_t)(output_frame[log_msg_len_start_index+1] & 0xFF);
    uint16_t msg_len = (msglen_2<<8)+msglen_1;

    int logcode_start_index = start_index+2;
    uint16_t logcode_1 = (uint16_t)(output_frame[logcode_start_index] & 0xFF);
    uint16_t logcode_2 = (uint16_t)(output_frame[logcode_start_index+1] & 0xFF);
    uint16_t logcode = (logcode_2<<8)+logcode_1;

    if(msg_len==0)
    {
        logcode=0;
    }

    int timestamp_start_index = start_index+4;
    uint64_t t_1 = (uint64_t)(output_frame[timestamp_start_index] & 0xFF);
    uint64_t t_2 = (uint64_t)(output_frame[timestamp_start_index+1] & 0xFF);
    uint64_t t_3 = (uint64_t)(output_frame[timestamp_start_index+2] & 0xFF);
    uint64_t t_4 = (uint64_t)(output_frame[timestamp_start_index+3] & 0xFF);
    uint64_t t_5 = (uint64_t)(output_frame[timestamp_start_index+4] & 0xFF);
    uint64_t t_6 = (uint64_t)(output_frame[timestamp_start_index+5] & 0xFF);
    uint64_t t_7 = (uint64_t)(output_frame[timestamp_start_index+6] & 0xFF);
    uint64_t t_8 = (uint64_t)(output_frame[timestamp_start_index+7] & 0xFF);

    uint64_t timestamp = ((t_8<<56)|(t_7<<48)|(t_6<<40)|(t_5<<32)|(t_4<<24)|(t_3<<16)|(t_2<<8)|t_1);

    // This version may not be true
    int version_start_index = start_index+12;
    int version = ((output_frame[version_start_index+3] & 0xFF)<<24) | ((output_frame[version_start_index+2] & 0xFF)<<16) | ((output_frame[version_start_index+1] & 0xFF)<<8) | ((output_frame[version_start_index] & 0xFF));

    msg_header[0]=msg_len;
    msg_header[1]=logcode;
    msg_header[2]=timestamp;
    msg_header[3]=version;
}               


void decode()
{
    bool crc_correct=false;
    bool success = true;
    std::string frame;

    while(success)
    {
        success = get_next_frame(frame, crc_correct);
        if (success && crc_correct) 
        {
            check_frame_format(frame);
            if (is_log_packet(frame.c_str(), frame.size())) 
            {
                // std::ofstream outfile("frame_output.txt", std::ios::app); // 
                // if (outfile.is_open()) {
                //     outfile << "check is log pass" << std::endl;
                //     outfile.close();
                // }

                const char *frame_c = frame.c_str();

                int start_index=4;
                uint64_t msg_header[4];
                decode_header(frame_c, start_index, msg_header);
                std::string timestamp_str = compute_timestamp(msg_header[2]);
                start_index+=12;
                
                std::cout << "This Msg is " << std::hex << std::uppercase << std::setw(2) 
                << std::setfill('0') << static_cast<int>(msg_header[1]) 
                << ": Msg_len = " << std::dec << static_cast<int>(msg_header[0]) 
                << ", logcode = " << std::hex << std::uppercase << std::setw(2) 
                << std::setfill('0') << static_cast<int>(msg_header[1]) 
                << ", version_4B = " << std::dec << static_cast<int>(msg_header[3]) 
                << ", timestamp = " << timestamp_str
                << std::endl;
                
                std::cout << "----------------------------------" << std::endl;

                //decode_libarary
                uint16_t logcode_now = (uint16_t) msg_header[1];
                int msg_len = msg_header[0];
                uint64_t time_real = msg_header[2];
                int version_4B = msg_header[3];
                int second = (int)(time_real/PER_SECOND);
                int usecond = (int)((time_real/PER_USECOND)-(second*1.0e6));
                uint64_t time_in_us_total = (uint64_t)second * 1000000 + (uint64_t)usecond;
                
                // FILE* orig_stdout = stdout;
                // stdout = fopen("decode_result.txt", "a");
                int a = S_D_V_decode((uint8_t *)frame_c, 65535, logcode_now, &start_index, time_in_us_total, 1, 1, 1, msg_len, version_4B);
                // fclose(stdout);
                // stdout = orig_stdout;

            }
            else
            {
                // std::ofstream outfile_log_file("frame_output.txt", std::ios::app); 
                // if (outfile_log_file.is_open()) {
                //     outfile_log_file << "check is log fail" << std::endl;
                //     outfile_log_file.close();
                // }
            }
        }
    }
}