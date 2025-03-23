#include <iostream>
#include <string>
#include <memory>
#include <libserial/SerialPort.h>
#include "decode_preprocess.h"
#include <fstream>

// global serial object define
std::shared_ptr<LibSerial::SerialPort> g_serial_port;

int main(int argc, char *argv[])
{
    try
    {
        // Default serial port configuration
        std::string port = "/dev/ttyUSB0";
        LibSerial::BaudRate baud_rate = LibSerial::BaudRate::BAUD_9600;

        if (argc > 1)
            port = argv[1];
        if (argc > 2)
        {
            int baud = std::stoi(argv[2]);
            switch (baud)
            {
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
                std::cerr << "unsupported baud rate: " << baud << "we will use the default 9600" << std::endl;
                baud_rate = LibSerial::BaudRate::BAUD_9600;
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

        // personalized your logcode here
        int logcode_num = 2;
        uint16_t logcode_list[2] = {0xB083, 0xB197};

        // generate and send cfg
        bool generate_cfg = generate_diag_config(g_serial_port->GetFileDescriptor(), logcode_list, logcode_num);
        // std::ofstream output_file("serial_data.txt");
        while (true)
        {
            std::vector<uint8_t> buffer;

            // read from serial
            g_serial_port->Read(buffer, 64, 10000000);

            if (buffer.size() > 0)
            {
                std::cout << "Received " << buffer.size() << " bytes of data" << std::endl;

                // Output the raw hex data to serial_data.txt
                // for (int i = 0; i < buffer.size(); i++)
                // {
                //     output_file << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]) << " ";
                // }
                // output_file << std::endl;
                // output_file.flush(); 

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