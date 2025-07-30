<div align="center">

# 🥷 CellNinjia Desktop

[![C++](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Research-orange.svg)](LICENSE)
[![5G](https://img.shields.io/badge/5G-SA%20%7C%20NSA-red.svg)](https://www.3gpp.org/)
[![arXiv](https://img.shields.io/badge/arXiv-2507.00337-b31b1b.svg)](https://arxiv.org/abs/2507.00337)

*A powerful RAN monitoring tool for cellular network research*

[📖 Documentation](#-overview) • [🚀 Quick Start](#-building) • [📱 Device Setup](#-device-setup) • [🔧 Usage](#-usage) • [🐛 Troubleshooting](#-troubleshooting)

</div>

---

## 🎯 What is CellNinjia?

CellNinjia is a cutting-edge **RAN (Radio Access Network) monitoring tool** that captures and decodes 5G/LTE diagnostic messages directly from mobile device modems. This desktop component interfaces with mobile phones via USB serial communication to collect real-time cellular network operational data for research and analysis.

## ✨ Key Features

<table>
<tr>
<td width="50%">

### 📡 **Real-time Monitoring**
- Live diagnostic message capture from mobile modems
- Zero-latency RAN state visibility
- Direct USB serial communication

### 🔍 **Comprehensive Decoding**
- **42+** different 5G message types supported
- Multi-layer protocol analysis (NAS, RRC, PDCP, RLC, MAC)
- ASN.1 message parsing

</td>
<td width="50%">

### 🎛️ **Flexible Operation**
- **Online mode**: Real-time decoding and analysis
- **Offline mode**: Raw data collection for later processing
- Configurable message type filtering

### 🏢 **Production Ready**
- Tested on commercial 5G SA networks
- Samsung Galaxy S22 Ultra validated
- Ubuntu 20.04 LTS support

</td>
</tr>
</table>

## 📋 Prerequisites

<div align="center">

| Component | Requirement | Status |
|-----------|-------------|---------|
| 🐧 **OS** | Linux (Ubuntu 20.04+ recommended) | ✅ Tested |
| 🔌 **Hardware** | USB port for device connection | ✅ Required |
| 📱 **Device** | Android with diagnostic mode | ✅ Samsung validated |
| 🛠️ **Build Tools** | CMake 3.10+, GCC/Clang | ✅ Standard |

</div>

### 📦 Dependencies

```bash
# 🔧 Install libserial development package
sudo apt-get install libserial-dev

# 🔌 For USB device access
sudo apt-get install usbutils
```

## 🚀 Building

```bash
# 📁 Create build directory
mkdir build && cd build

# ⚙️ Configure with CMake
cmake ..

# 🔨 Build the project
make
```

<div align="center">

### 🎉 Build Output

| Executable | Purpose | Icon |
|------------|---------|------|
| `CellNinjia_desktop` | Main data collection tool | 🥷 |
| `decoder` | Offline message decoder | 🔓 |
| `libcellninjia_asn1.so` | ASN.1 decoding library | 📚 |

</div>

## 📱 Device Setup

<details>
<summary><b>🔧 Step 1: Enable Diagnostic Mode</b></summary>

### For Samsung Devices 📞
1. **Dial**: `*#0808#`
2. **Select**: `RNDIS+DM+MODEM+ADB` mode
3. **Reboot**: Your device to apply changes

> 💡 **Note**: This procedure is device-specific. Samsung devices don't require rooting.

</details>

<details>
<summary><b>🔌 Step 2: USB Connection Setup</b></summary>

### Connection Process
1. **Connect** device via USB cable
2. **Install** USB drivers if prompted by your system
3. **Identify** the diagnostic port using commands below:

```bash
# 📋 List USB devices
lsusb

# 📱 Example output for Samsung device:
# Bus 001 Device 125: ID 04e8:6864 Samsung Electronics Co., Ltd GT-I9070

# 🔧 Load USB serial driver
sudo modprobe usbserial vendor=0x04e8 product=0x6864

# 🔍 Check for serial ports (typically /dev/ttyUSB0, /dev/ttyUSB1, etc.)
ls /dev/ttyUSB*
```

</details>

## 🔧 Usage

<div align="center">

### 🎮 Choose Your Mode

</div>

<table>
<tr>
<td width="50%">

### 🔴 **Online Mode** (Real-time)
Perfect for live analysis and monitoring

```bash
# 🚀 Start real-time monitoring
./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s on output.txt
```

**Features:**
- ⚡ Real-time decoding
- 📊 Live data analysis
- 🎯 Immediate insights

</td>
<td width="50%">

### 💾 **Offline Mode** (Batch processing)
Ideal for detailed post-analysis

```bash
# 📦 Collect raw data
./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s off raw.log

# 🔓 Decode later
./decoder -i raw.log -o decoded.txt
```

**Features:**
- 💽 Raw data preservation
- 🔍 Detailed analysis
- 🔄 Reprocessing capability

</td>
</tr>
</table>

### 📝 Command Reference

<div align="center">

| Option | Description | Example |
|--------|-------------|---------|
| `-h` | 📖 Display help message | `./CellNinjia_desktop -h` |
| `-p <port>` | 🔌 Serial port name | `-p /dev/ttyUSB0` |
| `-b <baudrate>` | ⚡ Baud rate (9600, 19200, 38400, 57600, 115200) | `-b 9600` |
| `-s <mode> <file>` | 🎛️ Operating mode and output file | `-s on output.txt` |

</div>

## ⚙️ Message Configuration

<div align="center">

### 🎛️ Customize Your Monitoring

Edit `main.cpp` to select which message types to capture

</div>

```cpp
// 🔧 Set up the diagnostic messages you want (main.cpp:107-112)
int msgtype_num = 3;
const char* msgtype_list[3] = {
    "NR5G L2 UL TB",           // 📤 Uplink Transport Block
    "NR5G L2 UL BSR",          // 📊 Buffer Status Report  
    "NR5G MAC DCI Info"        // 📡 Downlink Control Information
};
```

> 💡 **Tip**: Choose message types based on your research focus. More messages = more data but higher processing load.

## 📊 Supported Message Types

<div align="center">

### 🎯 **42+ Message Types Across All Protocol Layers**

*Comprehensive coverage of the 5G protocol stack*

</div>

<table>
<tr>
<td width="50%">

### 🌐 **Upper Layers**
- **📱 NAS (Non-Access Stratum)**
  - MM5G State, SM5G Plain OTA
  - NSSAI Info, Message Container
- **📡 RRC (Radio Resource Control)**  
  - MIB Info, Serving Cell Info
  - Configuration Info, CA Combos

### 🔗 **Data Link Layers**
- **📦 PDCP (Packet Data Convergence)**
  - DL/UL Data PDU, Stats, Control PDU
- **🔄 RLC (Radio Link Control)**
  - DL/UL Stats, Status PDU

</td>
<td width="50%">

### ⚡ **MAC & Physical**
- **🎛️ MAC (Medium Access Control)**
  - DCI Info, TB Stats, RACH Trigger
  - UCI Information, Flow Control
- **📶 Physical Layer**
  - Searcher Measurements
  - Antenna Diversity, AGC Control

### 🎪 **Network Compatibility**
- ✅ **5G SA** (Standalone) - Full support
- ✅ **5G NSA** (Non-Standalone) - Supported  
- ✅ **4G LTE** - Available (testing disabled)

</td>
</tr>
</table>

<div align="center">

> 📋 **Complete List**: See original documentation for all 42 message types with version numbers

</div>

## 🐛 Troubleshooting

<details>
<summary><b>🚫 Port Access Denied</b></summary>

```bash
# 👥 Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in, or use immediate fix:
sudo chmod 666 /dev/ttyUSB0
```

</details>

<details>
<summary><b>📱 Device Not Detected</b></summary>

**Checklist:**
- ✅ Verify diagnostic mode is enabled (`*#0808#`)
- ✅ Check USB cable and connection quality
- ✅ Ensure proper USB drivers are installed
- ✅ Try different USB ports (USB 2.0 preferred)
- ✅ Restart device after enabling diagnostic mode

</details>

<details>
<summary><b>📡 No Data Received</b></summary>

**Debug Steps:**
1. **Port & Baud Rate**: Verify correct settings
2. **Message Config**: Check message type configuration in `main.cpp`
3. **Network Activity**: Ensure device is actively using cellular network
4. **Signal Strength**: Move to area with good cellular coverage
5. **Carrier Compatibility**: Some carriers may block diagnostic access

</details>

<details>
<summary><b>🔧 Build Issues</b></summary>

```bash
# 🧹 Clean build
rm -rf build && mkdir build && cd build

# 🔍 Verbose build for debugging
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON
make VERBOSE=1
```

</details>

## 🔬 Research Applications

<div align="center">

### 🎓 **Enabling Cutting-Edge Cellular Research**

</div>

<table>
<tr>
<td width="50%">

### 📊 **Network Analysis**
- 🔍 RAN buffer analysis and delay attribution
- 📈 Protocol performance evaluation  
- ⚡ Real-time network behavior monitoring
- 🎯 5G/LTE characterization studies

</td>
<td width="50%">

### 🧪 **Research Areas**
- 🌐 Network optimization studies
- 📱 Mobile protocol analysis
- 🔬 Cellular performance research
- 🎛️ QoS and latency investigations

</td>
</tr>
</table>

<div align="center">

### 📚 **Academic Reference**

[![arXiv](https://img.shields.io/badge/Paper-arXiv%3A2507.00337-red.svg)](https://arxiv.org/abs/2507.00337)

*"Seeing Through the Fog: Empowering Mobile Devices to Expose and Mitigate RAN Buffer Effects on Delay-Sensitive Protocols"*

</div>

---

<div align="center">

## 📄 License

![License](https://img.shields.io/badge/License-Research%20%26%20Educational%20Use-blue.svg)

**This tool is provided for research and educational purposes.**

### 🤝 Contributing
Interested in contributing? We welcome researchers and developers!  
Open an issue or submit a pull request.

### ⭐ Star This Project
If CellNinjia helps your research, please star this repository!

---

**Made with ❤️ by the Cellular Network Research Community**

</div>