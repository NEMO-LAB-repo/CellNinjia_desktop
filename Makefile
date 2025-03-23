# Compiler and options
CXX = g++
CXXFLAGS = -Wall -std=c++11
INCLUDES = -I. -Iasn1_group
LIBS = -L. -lcellninjia_asn1 -lserial  # Add -L. to search for libraries in current directory

# Target files
TARGET = CellNinjia_desktop
OBJS = main.o decode_preprocess.o

# Library path
LIB_PATH = /usr/local/lib
ASN1_LIB = libcellninjia_asn1.so

# Default target
all: $(TARGET)

# Compile and link the main program
$(TARGET): $(OBJS) check_lib
	$(CXX) -o $@ $(OBJS) $(LIBS)

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Ensure library files are copied to system path
check_lib:
	@if [ ! -f $(LIB_PATH)/$(ASN1_LIB) ]; then \
		echo "Installing $(ASN1_LIB) to $(LIB_PATH)"; \
		sudo cp $(ASN1_LIB) $(LIB_PATH)/; \
		sudo ldconfig; \
	fi

# Run the program
run: $(TARGET)
	LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./$(TARGET)

# Clean intermediate files and target files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean check_lib run