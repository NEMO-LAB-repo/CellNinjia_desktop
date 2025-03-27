#include <iostream>
#include <string>
#include <memory>
#include <libserial/SerialPort.h>
#include "../decode_preprocess.h"
#include <fstream>
#include <vector>

void display_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " -i <binary_file> [-o <output_file>]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i <file>   Input binary file (required)" << std::endl;
    std::cout << "  -o <file>   Output file (optional, defaults to console)" << std::endl;
    std::cout << "  -h          Display this help message" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string input_filename;
    std::string output_filename;
    
    // Parse command line arguments using for loop
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-i" && i + 1 < argc) {
            input_filename = argv[i + 1];
            i++; // Skip the next argument since we've used it
        } else if (arg == "-o" && i + 1 < argc) {
            output_filename = argv[i + 1];
            i++; // Skip the next argument
        } else if (arg == "-h") {
            display_help(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            display_help(argv[0]);
            return 1;
        }
    }
    
    // Check if input file was specified
    if (input_filename.empty()) {
        std::cerr << "Error: Input file must be specified with -i option" << std::endl;
        display_help(argv[0]);
        return 1;
    }
    
    // Open input file
    std::ifstream file(input_filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file '" << input_filename << "'" << std::endl;
        return 1;
    }
    
    try {
        // Get file size
        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        // Read file content
        std::vector<uint8_t> buffer(file_size);
        if (file.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
            enter_binary(buffer);
            
            std::cout << "Successfully read " << file_size << " bytes from '" 
                      << input_filename << "' into current_buffer" << std::endl;
        } else {
            std::cerr << "Error: Failed to read the entire file" << std::endl;
            return 1;
        }
        
        file.close();
        
        // Redirect stdout to output file if specified
        FILE* original_stdout = stdout;
        FILE* output_file = nullptr;

        if (!output_filename.empty()) {
            output_file = freopen(output_filename.c_str(), "w", stdout);
            if (!output_file) {
                std::cerr << "Error: Could not open output file: " << output_filename << std::endl;
                return 1;
            }
            std::cerr << "Output will be written to '" << output_filename << "'" << std::endl;
        }
        
        // Decode the binary data
        decode();
        
        // Restore stdout if it was redirected
        if (output_file) {
            fclose(stdout);
            stdout = original_stdout;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}