<div align="center">

# CellNinjia Desktop

[![C++](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)
[![5G](https://img.shields.io/badge/5G-SA%20%7C%20NSA-red.svg)](https://www.3gpp.org/)

*A comprehensive next-generation network monitoring tool for cellular research*

[📖 Documentation](#-overview) • [🚀 Quick Start](#-building) • [📱 Device Setup](#-device-setup) • [🔧 Usage](#-usage) • [🐛 Troubleshooting](#-troubleshooting) • [📝 Citation](#-citation)

</div>

---

## 🎯 What is CellNinjia?

CellNinjia is a cutting-edge **next-generation network monitoring tool** that captures and decodes 5G/LTE diagnostic messages directly from mobile device modems. This comprehensive solution monitors both the **Radio Access Network (RAN)** and **mobile core network communications**, decoding messages exchanged between User Equipment (UE) and network infrastructure. The desktop component interfaces with mobile phones via USB serial communication to collect real-time cellular network operational data for research and analysis.

> 📝 **Citation**: If you use CellNinjia, please cite our SenSys '26 paper. See [Citation](#-citation) for the BibTeX entry.

## ✨ Key Features

<table>
<tr>
<td width="50%">

### 📡 **Real-time Monitoring**
- Live diagnostic message capture from mobile modems
- RAN and core network state visibility
- Direct USB serial communication

### 🔍 **Comprehensive Decoding**
- **42+** different 5G message types supported
- Multi-layer protocol analysis (NAS, RRC, PDCP, RLC, MAC)
- UE-to-core network message decoding
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

### 📁 Project Structure
```
CellNinjia_desktop/
├── src/              # 📝 Source code
├── include/asn1/     # 📄 Header files
├── tools/decoder/    # 🔧 Decoder tool
├── lib/              # 📚 Libraries
├── examples/         # 📃 Offline test files
└── build/            # 🔨 Build output (created during build)
```

### 🔨 Build Commands
```bash
# 📁 Create build directory
mkdir build && cd build

# ⚙️ Configure with CMake
cmake ..

# 🔨 Build the project
make

# 🚀 Return to project root to run executables
cd ..
```

<div align="center">

### 🎉 Build Output

| File | Location | Purpose | Icon |
|------|----------|---------|------|
| `CellNinjia_desktop` | Project root | Main data collection tool | 🥷 |
| `decoder` | Project root | Offline message decoder | 🔓 |
| `lib/libcellninjia_asn1.so` | lib/ directory | ASN.1 decoding library | 📚 |

</div>

> 💡 **Note**: Executables are built in the project root directory for easy access. The organized source structure improves maintainability while keeping usage simple.

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

Edit `src/main.cpp` to select which message types to capture

</div>

```cpp
// 🔧 Set up the diagnostic messages you want (src/main.cpp:107-112)
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

### 🌐 **Upper Layers** (UE ↔ Core Network)
- **📱 NAS (Non-Access Stratum)**
  - MM5G State, SM5G Plain OTA
  - NSSAI Info, Message Container
- **📡 RRC (Radio Resource Control)**  
  - MIB Info, Serving Cell Info
  - Configuration Info, CA Combos

### 🔗 **Data Link Layers** (RAN)
- **📦 PDCP (Packet Data Convergence)**
  - DL/UL Data PDU, Stats, Control PDU
- **🔄 RLC (Radio Link Control)**
  - DL/UL Stats, Status PDU

</td>
<td width="50%">

### ⚡ **MAC & Physical** (RAN)
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

<details>
<summary><b>📋 Complete List of All 42 Supported Message Types</b></summary>

| Index | Message Name | Version |
|-------|--------------|---------|
| 1 | NR5G NAS SM5G Plain OTA Outgoing Msg | 1 |
| 2 | NR5G NAS MM5G State | 1 |
| 3 | NR5G NAS Plain Message Container | 1 |
| 4 | NR5G NAS MM5G NSSAI Info | 1 |
| 5 | NR5G RRC MIB Info | 1 |
| 6 | NR5G RRC Serving Cell Info | 4 |
| 7 | NR5G RRC Configuration Info | 8 |
| 8 | NR5G RRC Supported CA Combos | 7 |
| 9 | NR5G PDCP DL Data Pdu | 6 |
| 10 | NR5G PDCP DL Rbs Stats | 5 |
| 11 | NR5G L2 DL Config | 5 |
| 12 | NR5G RLC DL Stats | 196608 |
| 13 | NR5G RLC DL Status PDU | 1 |
| 14 | NR5G L2 DL DATA PDU | 6 |
| 15 | NR5G PDCP UL Stats | 9 |
| 16 | NR5G PDCP UL Control Pdu | 196609 |
| 17 | NR5G RLC UL Stats | 4 |
| 18 | NR5G L2 UL Config | 5 |
| 19 | NR5G L2 UL TB | 4 & 196616 |
| 20 | NR5G L2 UL BSR | 196611 |
| 21 | NR5G MAC UL TB Stats | 196609 |
| 22 | NR5G MAC DCI Info | 131077 & 196617 |
| 23 | NR5G MAC DL TB Report | 3 |
| 24 | NR5G MAC PDSCH Status | 5 & 196611|
| 25 | NR5G MAC PDSCH Stats | 196608 |
| 26 | NR5G MAC RACH Trigger | 196615 |
| 27 | NR5G MAC CDRX Events Info | 131073 |
| 28 | NR5G MAC UCI Information | 2 & 196608 |
| 29 | NR5G MAC Flow Control | 131073 |
| 30 | NR5G MAC CSF Report | 131073 & 196613 |
| 31 | NR5G ML1 Searcher Measurement Database Update Ext | 131081 |
| 32 | NR5G RRC OTA Packet | 16.6.0 |
| 33 | NR5G MAC RACH Attempt | 196616 |
| 34 | NR5G MAC UCI Payload Information | 196608 |
| 35 | NR5G ML1 Searcher Conn Eval | 131075 |
| 36 | NR5G RLC UL Status PDU | 196609 |
| 37 | NR5G ML1 Antenna Switch Diversity | 131074 |
| 38 | NR5G LL1 FW Serving FTL | 7 |
| 39 | NR5G ML1 Search HO Acq Confirm | 131079 |
| 40 | NR5G L2 UL Data PDU | 196611 |
| 41 | NR5G LL1 FW RX Control AGC | 131072 |
| 42 | NR5G MAC UL Physical Channel Schedule Report | 196623 |

> ⚠️ **Version Notice**: Pay attention to the version of the data you collect. If versions don't match, it may lead to program errors or incorrect decoding.

</details>

### 📘 Message Details

For a concise, per-message description of purpose and key fields, see
[`docs/Message_details.md`](docs/Message_details.md). This document serves as an
 overview for the supported NR5G diagnostic messages.

## 🐛 Troubleshooting

<details>
<summary><b>🚫 Port Access Denied</b></summary>

```bash
# 👥 Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in, or use immediate fix:
sudo chmod 666 /dev/ttyUSB0
# Use adb and CellNinjia together
sudo rmmod usbserial  
- Then setup adb applications
sudo modprobe usbserial vendor=0x04e8 product=0x6864
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

## 📝 Citation

If you use CellNinjia, please cite the following paper:

> Yuxin Liu, Tianyang Zhang, Qiang Wu, Ju Ren, Kyle Jamieson, and Yaxiong Xie. 2026. **RAN-Aware Delay Compensation for Delay-Sensitive Protocols in Cellular Networks.** In *Proceedings of the 2026 ACM/IEEE International Conference on Embedded Artificial Intelligence and Sensing Systems (SenSys '26)*. ACM, 1144–1157. https://doi.org/10.1145/3774906.3802791

```bibtex
@inproceedings{liu2026ranaware,
  author    = {Liu, Yuxin and Zhang, Tianyang and Wu, Qiang and Ren, Ju and Jamieson, Kyle and Xie, Yaxiong},
  title     = {{RAN}-Aware Delay Compensation for Delay-Sensitive Protocols in Cellular Networks},
  booktitle = {Proceedings of the 2026 ACM/IEEE International Conference on Embedded Artificial Intelligence and Sensing Systems},
  series    = {SenSys '26},
  pages     = {1144--1157},
  year      = {2026},
  publisher = {ACM},
  address   = {New York, NY, USA},
  doi       = {10.1145/3774906.3802791},
  url       = {https://doi.org/10.1145/3774906.3802791}
}
```

---

<div align="center">

## 📄 License

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)

**This project is licensed under the [Apache License 2.0](LICENSE).**

### 🤝 Contributing
Interested in contributing? We welcome researchers and developers!  
Open an issue or submit a pull request.

---

</div>

This material is based upon work supported by the National Science Foundation under Award No. CNS-2223556.
