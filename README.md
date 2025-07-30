# CellNinjia Desktop

CellNinjia is a RAN (Radio Access Network) monitoring tool that captures and decodes 5G/LTE diagnostic messages directly from mobile device modems. This desktop component interfaces with mobile phones via USB serial communication to collect real-time cellular network operational data.

## Overview

CellNinjia provides researchers and network engineers with unprecedented visibility into cellular RAN operations by:
- Capturing real-time diagnostic messages from mobile device modems
- Decoding 42+ different 5G message types across protocol layers
- Supporting both real-time analysis and offline data collection
- Enabling research into cellular network behavior and performance

## Prerequisites

### System Requirements
- Linux-based system (tested on Ubuntu 20.04)
- USB port for mobile device connection
- Compatible Android device (tested with Samsung Galaxy S22 Ultra)

### Dependencies
```bash
# Install libserial development package
sudo apt-get install libserial-dev

# For USB device access
sudo apt-get install usbutils
```

## Building

```bash
mkdir build
cd build
cmake ..
make
```

This generates two executables:
- `CellNinjia_desktop` - Main data collection tool
- `decoder` - Offline message decoder

## Device Setup

### 1. Enable Diagnostic Mode
Enable diagnostic mode on your Android device. For Samsung devices:
1. Dial `*#0808#`
2. Select `RNDIS+DM+MODEM+ADB` mode
3. Reboot the device

### 2. USB Connection Setup
1. Connect device via USB cable
2. Install necessary USB drivers if prompted
3. Identify the diagnostic port:

```bash
# List USB devices
lsusb

# Example output for Samsung device:
# Bus 001 Device 125: ID 04e8:6864 Samsung Electronics Co., Ltd GT-I9070

# Load USB serial driver
sudo modprobe usbserial vendor=0x04e8 product=0x6864

# Check for serial ports (typically /dev/ttyUSB0, /dev/ttyUSB1, etc.)
ls /dev/ttyUSB*
```

## Usage

### Real-time Monitoring (Online Mode)
Capture and decode messages in real-time:
```bash
./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s on output.txt
```

### Raw Data Collection (Offline Mode)
Collect raw binary data for later analysis:
```bash
# Collect raw data
./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s off raw_data.log

# Decode offline
./decoder -i raw_data.log -o decoded_output.txt
```

### Command Line Options
```
Usage: CellNinjia_desktop -p <portname> -b <baudrate> -s <mode> <logfile>

Options:
  -h              Display help message
  -p <port>       Serial port name (e.g., /dev/ttyUSB0)
  -b <baudrate>   Baud rate (9600, 19200, 38400, 57600, 115200)
  -s <mode> <file> Operating mode:
      on  <file>  Online mode - real-time decoding to file
      off <file>  Offline mode - raw binary data to file
```

## Message Configuration

Configure which message types to capture by modifying `main.cpp`:

```cpp
// Set up the diag msg name you want (main.cpp:107-112)
int msgtype_num = 3;
const char* msgtype_list[3] = {
    "NR5G L2 UL TB",
    "NR5G L2 UL BSR",
    "NR5G MAC DCI Info"
};
```

## Supported Message Types

CellNinjia can decode 42 different 5G message types across multiple protocol layers:

| Layer | Examples |
|-------|----------|
| **NAS (Non-Access Stratum)** | MM5G State, SM5G Plain OTA, NSSAI Info |
| **RRC (Radio Resource Control)** | MIB Info, Serving Cell Info, Configuration Info |
| **PDCP** | DL/UL Data PDU, DL/UL Stats, Control PDU |
| **RLC** | DL/UL Stats, Status PDU |
| **MAC** | DCI Info, TB Stats, RACH Trigger, UCI Information |
| **Physical Layer** | Searcher Measurements, Antenna Diversity, AGC Control |

For a complete list of supported messages and their versions, see the table in the original documentation.

## Network Compatibility

- **5G SA (Standalone)**: Full support with comprehensive message coverage
- **4G LTE**: Supported (message types currently disabled for testing)
- **Commercial Networks**: Validated across multiple carrier networks

## Troubleshooting

### Common Issues

**Port Access Denied**
```bash
sudo usermod -a -G dialout $USER
# Log out and back in, or:
sudo chmod 666 /dev/ttyUSB0
```

**Device Not Detected**
- Verify diagnostic mode is enabled
- Check USB cable and connection
- Ensure proper USB drivers are installed
- Try different USB ports

**No Data Received**
- Verify correct port and baud rate
- Check message type configuration
- Ensure device is actively using cellular network

## Research Applications

CellNinjia is designed for cellular network research including:
- RAN buffer analysis and delay attribution
- Protocol performance evaluation
- Network optimization studies
- 5G/LTE behavior characterization

For research using CellNinjia, see: [arXiv:2507.00337](https://arxiv.org/abs/2507.00337)

## License

This tool is provided for research and educational purposes.