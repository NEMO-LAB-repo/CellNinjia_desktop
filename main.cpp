#include <iostream>
#include <string>
#include <memory>
#include <libserial/SerialPort.h>
#include "decode_preprocess.h"
#include <fstream>

// global serial object define
std::shared_ptr<LibSerial::SerialPort> g_serial_port;

void display_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " [-o <outputfile>] -p <portname> -b <baudrate>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h          Display this help message" << std::endl;
    std::cout << "  -o <file>   Optional: Redirect output to specified file" << std::endl;
    std::cout << "  -p <port>   Required: Serial port name (e.g., /dev/ttyUSB0)" << std::endl;
    std::cout << "  -b <baud>   Required: Baud rate (9600, 19200, 38400, 57600, 115200)" << std::endl;
}


int main(int argc, char *argv[])
{
    try
    {
        // Default serial port configuration
        std::string port = "/dev/ttyUSB0";
        LibSerial::BaudRate baud_rate = LibSerial::BaudRate::BAUD_9600;
        std::string output_filename = "";

        // Parse command line arguments
        for (int i = 1; i < argc; i++) 
        {
            std::string arg = argv[i];
            
            if (arg == "-o" && i + 1 < argc) {
                output_filename = argv[i + 1];
                i++; 
            } else if (arg == "-p" && i + 1 < argc) {
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
            } else if (arg == "-h" || arg == "--help") {
                display_help(argv[0]);
                return 0;
            }else{
                std::cout << "Error arguments." << std::endl;
                std::cout << "Usage: " << argv[0] << " -o <outputfile> -p <portname> -b <baudrate>" << std::endl;
                return 0;
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

        std::cout << "success open" << port << std::endl;

        int logcode_num = 1;
        uint16_t logcode_list[1] = {0xB083};

        // generate and send cfg
        bool generate_cfg = generate_diag_config(g_serial_port->GetFileDescriptor(), logcode_list, logcode_num);

        // Redirect stdout if output file is specified
        FILE* original_stdout = stdout;
        FILE* output_file = nullptr;

        if (!output_filename.empty()) {
            output_file = freopen(output_filename.c_str(), "w", stdout);
            if (!output_file) {
                std::cerr << "Error: Could not open output file: " << output_filename << std::endl;
                return 1;
            }
            // std::cout << "Output redirected to: " << output_filename << std::endl;
        }

        while (true)
        {
            std::vector<uint8_t> buffer;

            // read from serial
            g_serial_port->Read(buffer, 64, 10000000);

            if (buffer.size() > 0)
            {
                // std::cout << "Received " << buffer.size() << " bytes of data" << std::endl;

                // Output the raw hex data 
                // for (int i = 0; i < buffer.size(); i++)
                // {
                //     std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]) << " ";
                // }
                // std::cout << std::endl;

                // There is something different between the USB mode and the memory mode, there would be some message leave head in the previous log and leave tail in the current log
                // So we need to use a string to append and delete it easily
                enter_binary(buffer);
                decode();
            }
        }

        // close port
        if (g_serial_port && g_serial_port->IsOpen())
        {
            g_serial_port->Close();
            std::cout << "close port" << std::endl;
        }
        
        // Restore stdout if it was redirected
        if (output_file) {
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