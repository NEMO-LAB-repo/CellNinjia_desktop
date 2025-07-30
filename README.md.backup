This is the desktop version of CellNinjia, we send the cfg to the phone's diag module by serial communication and receive the diag messages from the modem.

## Build and Install
Prerequirement package: 
```
libserial
```
Compile the code:
```
mkdir build
cd build
cmake ..
make
```
The executable file `CellNinjia_desktop` and `decoder` will be generated.   
The `libcellninjia_asn1.so` is the library which is used to decode the messages.

## Configure the phone connection to the desktop steps
1. Enable the diagnostic mode on the phone. Note that this procedure is phone-specific. For samsung, you don't need to root it, just dial `*#0808#` to open `RNDIS+DM+MODEM+ADB` mode.
2. Connect the phone to the desktop machine with the USB cable. You may need to install USB drivers.
3. Figure out the COM port for the phone’s diagnostic mode.
For example:
```
➜ lsusb
Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 004: ID 8087:0033 Intel Corp. 
Bus 001 Device 125: ID 04e8:6864 Samsung Electronics Co., Ltd GT-I9070 (network tethering, USB debugging enabled)
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```
Then load the usbserial:
```
modprobe usbserial vendor=0x04e8 product=0x6864
```
Typically the serial port would be listed, typically as /dev/ttyUSBX (X=0,1,2,…).  

We have tested Samsung Galaxy S22 Ultra on UBUNTU 20.04 in both 5G and LTE scenarios.

## Configure the Message Type
Message types can be configured in main.cpp by modifying the `msgtype_num` parameter and the `msgtype_list` array to specify the required message types. The `msgtype_list` array should contain the message names as defined in the '5G Supported Message Type' section below. Note that LTE message types are currently disabled for testing purposes.

## Run Offline
We collect raw binary data transferred by port and record in specific file.
```
./CellNinjia_desktop -p <portname> -b <baudrate> -s off <filename>
example: ./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s off raw_binary.log
```
Then offline decode the raw binary data.
```
./decoder -i <binary data filename> -o <decode result filename>
example: ./decoder -i raw_binary.log -o decode_result.txt
```

## Run Online
We decode the message data and record in specific file.
```
./CellNinjia_desktop -p <portname> -b <baudrate> -s on <filename>
./CellNinjia_desktop -p /dev/ttyUSB0 -b 9600 -s on online_decode.txt
```

## 5G Supported Message Type
We can decode these message types so far, please pay attention to the version of the data you collected, if the version is not the same, it may lead to program errors or the messages can not be correctly decoded. We are still working on decoding more message types.  
|Index|Message Name|Version|
|--|--|--|
|1|NR5G NAS SM5G Plain OTA Outgoing Msg|1|
|2|NR5G NAS MM5G State|1|
|3|NR5G NAS Plain Message Container|1|
|4|NR5G NAS MM5G NSSAI Info|1|
|5|NR5G RRC MIB Info|1|
|6|NR5G RRC Serving Cell Info|4|
|7|NR5G RRC Configuration Info|8|
|8|NR5G RRC Supported CA Combos|7|
|9|NR5G PDCP DL Data Pdu|6|
|10|NR5G PDCP DL Rbs Stats|5|
|11|NR5G L2 DL Config|5|
|12|NR5G RLC DL Stats|196608|
|13|NR5G RLC DL Status PDU|1|
|14|NR5G L2 DL DATA PDU|6|
|15|NR5G PDCP UL Stats|9|
|16|NR5G PDCP UL Control Pdu|196609|
|17|NR5G RLC UL Stats|4|
|18|NR5G L2 UL Config|5|
|19|NR5G L2 UL TB|4|
|20|NR5G L2 UL BSR|196611|
|21|NR5G MAC UL TB Stats|196609|
|22|NR5G MAC DCI Info|131077 & 196617|
|23|NR5G MAC DL TB Report|3|
|24|NR5G MAC PDSCH Status|5|
|25|NR5G MAC PDSCH Stats|196608|
|26|NR5G MAC RACH Trigger|196615|
|27|NR5G MAC CDRX Events Info|131073|
|28|NR5G MAC UCI Information|2 & 196608|
|29|NR5G MAC Flow Control|131073|
|30|NR5G MAC CSF Report|131073|
|31|NR5G ML1 Searcher Measurement Database Update Ext|131081|
|32|NR5G RRC OTA Packet|16.6.0|
|33|NR5G MAC RACH Attempt|196616|
|34|NR5G RLC UL Status PDU|196608|
|35|NR5G ML1 Searcher Conn Eval|131075|
|36|NR5G RLC UL Status PDU|196609|
|37|NR5G ML1 Antenna Switch Diversity|131074|
|38|NR5G LL1 FW Serving FTL|7|
|39|NR5G ML1 Search HO Acq Confirm|131079|
|40|NR5G L2 UL Data PDU|196611|
|41|NR5G LL1 FW RX Control AGC|131072|
|42|NR5G MAC UL Physical Channel Schedule Report|196623|