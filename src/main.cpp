#include <iostream>
#include <string>
#include <memory>
#include <libserial/SerialPort.h>
#include "decode_preprocess.h"
#include <fstream>

// global serial object define
std::shared_ptr<LibSerial::SerialPort> g_serial_port;

void display_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " -p <portname> -b <baudrate> -s <mode> <logfile>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h          Display this help message" << std::endl;
    std::cout << "  -p <port>   Serial port name (e.g., /dev/ttyUSB0)" << std::endl;
    std::cout << "  -b <baud>   Baud rate (9600, 19200, 38400, 57600, 115200)" << std::endl;
    std::cout << "  -s <mode> <file>  Mode and output file:" << std::endl;
    std::cout << "       off  <file>: Offline mode - save raw binary data to file" << std::endl;
    std::cout << "       on   <file>: Online mode - save decoded output to file but not save raw binary data" << std::endl;
}

enum class RunMode {
    OFFLINE,   // Save raw binary data to file
    ONLINE     // Save decoded output to file
};

int main(int argc, char *argv[])
{
    try
    {
        // Default serial port configuration
        std::string port = "/dev/ttyUSB0";
        LibSerial::BaudRate baud_rate = LibSerial::BaudRate::BAUD_9600;
        std::string output_filename = "";
        RunMode run_mode = RunMode::ONLINE;

        // Parse command line arguments
        for (int i = 1; i < argc; i++) 
        {
            std::string arg = argv[i];
            
            if (arg == "-p" && i + 1 < argc) {
                port = argv[i + 1];
                i++; 
            } else if (arg == "-b" && i + 1 < argc) {
                int baud = std::stoi(argv[i + 1]);
                i++; 
                
                switch (baud) {
                    case 9600:
                        baud_rate = LibSerial::BaudRate::BAUD_9600;
                        break;
                    case 19200:
                        baud_rate = LibSerial::BaudRate::BAUD_19200;
                        break;
                    case 38400:
                        baud_rate = LibSerial::BaudRate::BAUD_38400;
                        break;
                    case 57600:
                        baud_rate = LibSerial::BaudRate::BAUD_57600;
                        break;
                    case 115200:
                        baud_rate = LibSerial::BaudRate::BAUD_115200;
                        break;
                    default:
                        std::cerr << "Unsupported baud rate: " << baud << ", using default 9600" << std::endl;
                        baud_rate = LibSerial::BaudRate::BAUD_9600;
                }
            } else if (arg == "-s" && i + 2 < argc) {
                std::string mode = argv[i + 1];
                output_filename = argv[i + 2];
                
                if (mode == "off") {
                    run_mode = RunMode::OFFLINE;
                } else if (mode == "on") {
                    run_mode = RunMode::ONLINE;
                } else {
                    std::cerr << "Invalid mode: " << mode << ", expected 'on' or 'off'" << std::endl;
                    display_help(argv[0]);
                    return 1;
                }
                
                i += 2; // Skip the next two arguments
            } else if (arg == "-h" || arg == "--help") {
                display_help(argv[0]);
                return 0;
            } else {
                std::cout << "Error arguments." << std::endl;
                display_help(argv[0]);
                return 1;
            }
        }

        g_serial_port = std::make_shared<LibSerial::SerialPort>();

        // open and config
        g_serial_port->Open(port);
        g_serial_port->SetBaudRate(baud_rate);
        g_serial_port->SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        g_serial_port->SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        g_serial_port->SetParity(LibSerial::Parity::PARITY_NONE);
        g_serial_port->SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        std::cout << "Success open " << port << std::endl;

        // Set up the diag msg name you want
        int msgtype_num = 6;
        const char* msgtype_list[6] = {
	    "NR5G MAC DCI Info",
	    "NR5G MAC CSF Report",
	    "NR5G MAC UCI Information",
	    "NR5G L2 UL BSR",
	    "NR5G MAC RACH Attempt",
	    "NR5G RRC Supported CA Combos"
        };
        
        // generate and send cfg
        bool generate_cfg = generate_diag_config(g_serial_port->GetFileDescriptor(), msgtype_list, msgtype_num);

        // Set up file output based on mode
        std::ofstream binary_output_file;
        FILE* original_stdout = stdout;
        
        if (run_mode == RunMode::OFFLINE) {
            // Open binary file for raw data in offline mode
            binary_output_file.open(output_filename, std::ios::binary);
            if (!binary_output_file.is_open()) {
                std::cerr << "Error: Could not open binary output file: " << output_filename << std::endl;
                return 1;
            }
            std::cout << "Binary raw data will be written to: " << output_filename << std::endl;
        } else if (run_mode == RunMode::ONLINE) {
            // Redirect stdout to file for decoded data in online mode
            FILE* output_file = freopen(output_filename.c_str(), "w", stdout);
            if (!output_file) {
                std::cerr << "Error: Could not open output file for decoded data: " << output_filename << std::endl;
                return 1;
            }
            std::cerr << "Decoded output will be written to: " << output_filename << std::endl;
        }

        while (true)
        {
            std::vector<uint8_t> buffer;

            // read from serial
            g_serial_port->Read(buffer, 64, 10000000);

            if (buffer.size() > 0)
            {
                if (run_mode == RunMode::OFFLINE) {
                    // Write raw data directly to binary file
                    binary_output_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
                    binary_output_file.flush();
                } else {
                    // Process data for default and online mode
                    enter_binary(buffer);
                    decode();
                }
            }
        }

        // close port
        if (g_serial_port && g_serial_port->IsOpen())
        {
            g_serial_port->Close();
            std::cout << "close port" << std::endl;
        }
        
        // Clean up files
        if (run_mode == RunMode::OFFLINE && binary_output_file.is_open()) {
            binary_output_file.close();
        } else if (run_mode == RunMode::ONLINE) {
            fclose(stdout);
            stdout = original_stdout;
        }
    }
    catch (const LibSerial::OpenFailed &e)
    {
        std::cerr << "cannot open port: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        if (g_serial_port && g_serial_port->IsOpen())
        {
            g_serial_port->Close();
        }
        return 1;
    }

    return 0;
}
