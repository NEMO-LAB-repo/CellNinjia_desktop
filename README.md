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
We can configure the message type you want in main.cpp. You just need to modify the `logcode_num` and the `logcode_list` in main.cpp according to the types of message you need. You can find messages information in the following `Supported Message Type` part.

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

## Supported Message Type
We can decode these message types so far, please pay attention to the version of the data you collected, if the version is not the same, it may lead to program errors or the messages can not be correctly decoded. We are still working on decoding more message types.  
|Index|Message Name|Logcode|Version|
|--|--|--|--|
|1|LTE_RLC_DL_AM_Control_PDU|0xB083|48|
|2|LTE_PDCP_DL_Data_PDU|0xB0A1|53|
|3|LTE_PDCP_UL_Delay_Statistics|0xB0B6|56|
|4|LTE_RRC_Supported_CA_Combos|0xB0CD|41|
|5|LTE_LL1_Serving_Cell_Measurement_Results|0xB116|21|
|6|LTE_LL1_PCFICH_Decoding_Results|0xB12A|161|
|7|LTE_LL1_RACH_TX_Report|0xB144|161|
|8|LTE_ML1_Random_Access_Request(MSG1)_Report|0xB167|40|
|9|LTE_ML1_Random_Access_Response(MSG2)_Report|0xB168|24|
|10|LTE_UE_Identification_Message_MSG3_Report|0xB169|40|
|11|LTE_Contention_Resulution_Message_MSG4_Report|0xB16A|1|
|12|LTE_ML1_DCI_Information_Report|0xB16C|49|
|13|LTE_ML1_GM_TX_Report|0xB16D|49|
|14|LTE_ML1_S_Criteria_Check_Procedure|0xB17A|1|
|15|LTE_ML1_Idle_Measurement_Request|0xB17D|1|
|16|LTE_ML1_Serving_Cell_Measurements_And_Evaluation|0xB17F|5|
|17|LTE_ML1_Intra_Frequency_Cell_Reselection_Payload|0xB181|1|
|18|LTE_ML1_CA_Metrics_Log_Packet|0xB184|50|
|19|LTE_ML1_AdvRx_IC_Cell_List|0xB18F|54|
|20|LTE_ML1_Cell_Measurement_Results|0xB196|41|
|21|LTE_ML1_Serving_Cell_Information|0xB197|2|
|22|LTE_ML1_Common_DC_Offset|0xB1B2|49|1|
|23|LTE_ML1_PDSCH_Stat_Indication|0xB173|48|
|24|NR5G NAS SM5G Plain OTA Outgoing Msg|0xB801|1|
|25|NR5G NAS MM5G State|0xB80C|1|
|26|NR5G_NAS_Plain_Message_Container|0xB814|1|
|27|NR5G_NAS_MM5G_NSSAI_Info|0xB815|1|
|28|NR5G_RRC_MIB_Info|0xB822|1|
|29|NR5G_RRC_Serving_Cell_Info|0xB823|4|
|30|NR5G_RRC_Configuration_Info|0xB825|8|
|31|NR5G_RRC_Supported_CA_Combos|0xB826|7|
|32|NR5G_PDCP_DL_Data_Pdu|0xB840|6|
|33|NR5G_PDCP_DL_Rbs_Stats|0xB842|5|
|34|NR5G_L2_DL_Config|0xB84B|5|
|35|NR5G RLC DL Stats|0xB84D|196608|
|36|NR5G_RLC_DL_Status_PDU|0xB84E|1|
|37|NR5G L2 DL DATA PDU|0xB857|6|
|38|NR5G PDCP UL Stats|0xB860|9|
|39|NR5G PDCP UL Control Pdu|0xB861|196609|
|40|NR5G RLC UL Stats|0xB868|4|
|41|NR5G L2 UL Config|0xB871|5|
|42|NR5G L2 UL TB|0xB872|4|
|43|NR5G L2 UL BSR|0xB873|196611|
|44|NR5G MAC UL TB Stats|0xB881|196609|
|45|NR5G MAC DCI Info|0xB885|131077 & 196617|
|46|NR5G MAC DL TB Report|0xB886|3|
|47|NR5G MAC PDSCH Status|0xB887|5|
|48|NR5G MAC PDSCH Stats|0xB888|196608|
|49|NR5G MAC RACH Trigger|0xB889|196615|
|50|NR5G MAC CDRX Events Info|0xB890|131073|
|51|NR5G MAC UCI Information|0xB89B|2|
|52|NR5G MAC Flow Control|0xB89C|131073|
|53|NR5G MAC CSF Report|0xB8A7|131073|
|54|NR5G ML1 Searcher Measurement Database Update Ext|0xB97F|131081|
|55|NR5G RRC OTA Packet|0xB821|16.6.0|
|56|NR5G MAC RACH Attempt|0xB88A|196616|
|57|NR5G RLC UL Status PDU|0xB896|196608|
|58|NR5G ML1 Searcher Conn Eval|0xB96F|131075|
|59|NR5G RLC UL Status PDU|0xB869|196609|
|60|NR5G ML1 Antenna Switch Diversity|0xB98F|131074|
|61|NR5G LL1 FW Serving FTL|0xB8DD|7|