We provide an overview of the NR5G diagnostic messages supported by CellNinjia desktop, summarizing the purpose of each log type and the key fields it contains.

## 1. NR5G NAS SM5G Plain OTA Outgoing Msg
### Purpose
This msg contains the NAS msg from UE to network.

### Content
- version
- Rel number
- Rel_version_major
- Rel_version_minor
- Message ID

## 2. NR5G NAS MM5G State
### Purpose
The current Mobility Management (MM) state of the UE in the 5G NAS layer. It is used to monitor the UE’s registration state, location information, and identifiers used to track the UE within the core network.

### Content
- MM5G State
- Identity
- AMF Region ID
- AMF Set ID
- AMF Pointer
- 5G TMSI
- NR5G TAC

## 3. NR5G NAS Plain Message Container
### Purpose
UE request the network to re-establish or resume NAS signaling and access so it can continue sending or receiving services/data under an existing 5G registration context.

### Content
- version
- release version

## 4. NR5G NAS MM5G NSSAI Info
### Purpose
This message reports the S-NSSAI (Single Network Slice Selection Assistance Information) context used or received by the UE during the 5G NAS Mobility Management (MM) procedure.

### Content
- Versions -> Reason
- PLMN ID
- NSSAI List Type
- Num NSSAI
- SD Val
- Mapping Exists

## 5. NR5G RRC MIB Info
### Purpose
This message records the decoded Master Information Block (MIB) received over the PBCH (Physical Broadcast Channel) during the initial cell acquisition process in NR5G.

### Content
- Physical Cell ID
- DL Frequency
- Block Index
- Half Number
- Intra Freq Reselection
- SFN
- PDCCH Config SIB1
- DMRS TypeA Position
- SSB Subcarrier Offset
- Subcarrier Spacing Common

## 6. NR5G RRC Serving Cell Info
### Purpose
This message provides key information about the current serving cell to which the UE is connected at the RRC layer.

### Content
- Physical Cell ID
- DL Frequency
- UL Frequency
- DL Bandwidth
- UL Bandwidth
- Cell Id
- Selected PLMN MCC
- Num MNC Digits
- Selected PLMN MNC
- Allowed Access
- TAC
- Freq Band Indicator

## 7. NR5G RRC Configuration Info
### Purpose
This message reflects the current RRC connection configuration on the UE. It is triggered when the UE is in RRC_CONNECTED state.

### Content
- State
- Config Status
- Connectivity Mode
- Num Active SRB
- Num Active DRB
- MN MCG DRB IDs
- Num Contiguous CC Groups
- Num Active CC
- Num Active RB
- Band Number
- DL BW Class
- UL BW Class
- CC Id
- Cell Id
- DL Arfcn
- UL Arfcn
- Band
- Band Type
- DL Carrier Bandwidth
- UL Carrier Bandwidth
- DL Max MIMO
- UL Max MIMO
- RB ID
- Termination Point
- DL RB Type
- DL RB Path
- DL ROHC Enabled
- DL Cipher Algo
- DL Integrity Algo
- UL RB Type
- UL RB Path
- UL ROHC Enabled
- UL Cipher Algo
- UL Integrity Algo

## 8. NR5G RRC Supported CA Combos
### Purpose
This message records the UE’s supported NR Carrier Aggregation (CA) combinations, detailing frequency bands, bandwidth classes, MIMO capabilities, and SCS support. It reflects device capabilities, not current network configuration.

### Content
- Total Num Combos From RF
- Starting Index
- num_combos
- Source
- Envelope Mode Pruned Out
- Num Bands
- Env Mask
- BC_ID
- Muting State
- Band
- DL BW Class
- Tech
- DL Max Antennas Index
- UL BW Class
- UL Type
- UL Max Antennas Index
- UL QAM Cap Index
- SRS TX Switch Type
- Max SCS
- DL BW Per CC
- UL BW Per CC

## 9. NR5G PDCP DL Data Pdu
### Purpose
This message captures the behavior of the PDCP layer in downlink, showing how IP packets are reconstructed from PDCP PDUs.

### Content
- Number Of Meta
- Number Of RB
- Log Count
- RB Cfg Index
- RX Deliv
- Rx Next
- Next Count
- Slot
- Frame
- RX Timetick Raw
- Key Index
- RLC Path
- Route Status
- IP Packet Header
- Start Count
- End Count
- RLC end SN
- Number IP Pkts
- Number IP bytes

## 10. NR5G PDCP DL Rbs Stats
### Purpose
This log provides per-RB downlink statistics at PDCP layer.

### Content
- Num RB
- RB Config Index
- RB Mode
- T Reordering
- Pdcp Hdr length
- Num Data PDU Received
- Data PDU Bytes Received
- Num Miss PDU To Upper Layer
- Num Reorder Timer Expire
- Num Reorder Timer Start
- RX Reorder
- RX Next
- RX Deliv

## 11. NR5G L2 DL Config
### Purpose
This msg records L2 downlink configuration information.

### Content
- Config Mask
- Num RB Logged
- DRB ID Bmask
- SRB ID Bitmask
- SRB Cipher Algo
- SRB Cipher Key Index
- srb_cipher_key1
- srb_cipher_key2
- srb_cipher_key3
- srb_cipher_key4
- SRB Integrity Algo
- SRB Integrity Key Index
- srb_integ_key1
- srb_integ_key2
- srb_integ_key3
- srb_integ_key4
- DRB Cipher Algo
- DRB Cipher Key Index
- drb_cipher_key1
- drb_cipher_key2
- drb_cipher_key3
- drb_cipher_key4
- DRB Integrity Algo
- DRB Integrity Key Index
- drb_integ_key1
- drb_integ_key2
- drb_integ_key3
- drb_integ_key4
- DRB ID Bmask
- SRB ID Bmask
- PDCP DRB Rel Mask
- RLC DRB Rel Mask
- PDCP SRB Rel Mask
- RLC SRB Rel Mask
- PDCP DRB Changed Mask
- RLC DRB Changed Mask
- PDCP SRB Changed Mask
- RLC SRB Changed Mask
- RB Mode
- RB Index
- EPSID
- RB Type
- Config Action
- RLC Path
- PDCP Path
- Config Bmask
- Sn Size
- Integrity Protection
- Status Report Required
- Out Of Order Delivery
- T Reordering
- LCID
- Action
- Config Bitmask
- Reestablish RLC
- T Status Prohibit
- T Reassembly
- SN Length

## 12. NR5G RLC DL Stats
### Purpose
This log captures RLC Layer statistics in the Downlink direction in NR 5G. It logs detailed per-RB information on PDUs, retransmissions, reassembly, and RLC status feedback (ACK/NACK).

### Content
- Num RB
- Reason
- Num FC Down Commands
- Num FC Up Commands
- Num RLF Issued
- Num Scell TA Start
- Num Scell TA Stop
- Num Scell FC Start
- Num Scell FC Stop
- Num RF Available
- Last RLF
- Num FC Level1 History Index
- Num FC Shutdown Inc Segs Dropped
- RB Config Index
- RB Mode
- Num RLC Meta
- Num Data PDU
- Num Status Rxed
- Num Invalid PDUs
- Num ReTx PDU
- Num Duplicate PDU
- Num Dropped PDU
- Num Segment Join
- Num Segment Collapse
- Num Status Trigger
- Num Status Update
- Num Status OTA
- Num Status PDU Drop
- Num Reassembly Expired
- Num Reassembly Start
- Num Missed UM PDU
- Num Meta Overlap
- Num Status Trigger Ack SN Only
- Num Status Trigger Nack List
- Num Status Update Ack SN Only
- Num Status Update Nack List
- Num Status Ackd
- Num Status Nack Lists
- RLC Data Bytes

## 13. NR5G RLC DL Status PDU
### Purpose
This message captures downlink RLC STATUS PDUs that are transmitted in 5G NR using RLC Acknowledged Mode (AM).

### Content
- Num Status PDU
- Slot
- Frame
- RB Config Index
- LCID
- SN Length Value
- Status PDU Size
- ACK_SN
- Header E1
- ACK_SN 18 Bits
- NACK_SN Part 1 Value
- NACK_SN 18 Bits Part 2 Value
- NACK_SN Part 3
- E1
- E2
- E3
- NACK_SN 18 Bits
- SO Start
- SO End
- NACK Range

## 14. NR5G L2 DL DATA PDU
### Purpose
This message records state variables/config for each RB (Radio Bearer) and downlink TBs (Transport Blocks) received by the UE from the gNB, and details their L2 protocol headers (MAC, RLC, PDCP).

### Content
- Num TB
- Last Processed TS
- LCID
- RLC Mode Value
- RLC SN Bits
- RLC SN Length Value
- RLC RB Index
- RB Type
- Rx Next
- Next IPA
- Rx Next Status Trigger
- Rx Highest Status
- Rx Next Highest
- TB Size
- Slot
- Frame
- Num Subheaders
- CA ID
- Num Logged Subheaders
- Fill
- LCID
- R
- L8
- SN 18 Bits
- L16
- RLC PDU Header
- First Byte PDCP Header

## 15. NR5G PDCP UL Stats
### Purpose
This log provides a comprehensive report of the PDCP Uplink layer behavior in 5G NR, combining both meta/system-level and per-Radio Bearer (RB) statistics.

### Content
- Reason
- Num RB Logged
- Cumulative SRB BMask
- Configured SRB BMask
- Cumulative DRB Bmask
- Configured DRB BMask
- Num PDCP Control PDUs
- Num Discard Packets
- Num Discard Bytes
- Total Count
- Count Low
- Count High
- Count Empty
- Count Non Empty
- RB ID
- Current PDCP Count HFN
- Current PDCP Count SN
- Total Num Discard Packets
- Total Num Discard Bytes
- Current WM Drop Packets
- Num RX Packets
- Total Num RX Bytes
- Current WM Drop Bytes
- DNE Bytes
- High WM Threshold Bytes
- Low WM Threshold Bytes
- PDCP Flow State

## 16. NR5G PDCP UL Control Pdu
### Purpose
This log captures the uplink transmission of PDCP control PDUs in 5G NR specifically, the STATUS_REPORT type PDUs sent from UE to gNB. These control PDUs report the PDCP reception status for downlink packets and are essential for retransmission mechanisms and window management.

### Content
- Num Control PDU
- RB ID
- PDCP SN Length
- RB Mode
- RB Type
- Size
- Pad Size
- Slot Number
- FN
- FMC

## 17. NR5G RLC UL Stats
### Purpose
This log captures uplink RLC statistics for each Logical Channel (LC) in 5G NR. It provides insight into the RLC Acknowledged Mode (AM) operation by exposing transmission progress, retransmissions, polling events, and STATUS PDU feedback.

### Content
- Reason
- Num LC Logged
- Cumulative LC BMask
- Configured LC BMask
- Total RLC Invalid Status PDUs
- Total Poll Expiry
- Total ReTx PDUs
- Total ReTx Bytes
- LCID
- Tx Next ACK
- Tx Next
- Current Tx Win PDUs
- Current Tx Win Bytes
- Total Tx PDUs
- Total Tx Bytes
- Total Poll PDUs
- Total High Prio PDUs Tx
- Total High Prio Bytes Tx
- Total Txed Status PDUs
- Total RLC Nack Status PDUs
- Last Valid Status PDU Received Time
- Total Rxed Status PDU


## 18. NR5G L2 UL Config
### Purpose
This log captures the uplink (UL) L2 Radio Bearer (RB) configuration updates applied in the UE, typically as a result of an RRC Request, like handover or initial setup. It includes SRB/DRB security config, PDCP & RLC bearer parameters, and flags for re-establishment.

### Content
- Num RB Logged
- Configured SRB Bitmask
- Configured DRB Bitmask
- SRB ID Bitmask
- DRB ID Bitmask
- SRB Cipher Algo
- SRB Cipher Key Index
- SRB Integrity Algo
- SRB Integrity Key Index
- DRB Cipher Algo
- DRB Cipher Key Index
- DRB Integrity Algo
- DRB Integrity Key Index
- PDCP SRB Bitmask
- RLC SRB Bitmask
- PDCP DRB Bitmask
- RLC DRB Bitmask
- RLC2 DRB Bitmask
- RB Id
- LCID
- EPS Id
- DRB IP
- T Pdcp Discard
- Poll Byte
- Poll PDU
- T Poll Retx
- Max Retx Threshold

## 19. NR5G L2 UL TB
### Purpose
The NR5G L2 UL TB message records actual UL Transport Block (TB) transmissions by the UE across multiple TTIs.

### Content
- Num TTI
- Slot Number
- FN
- Num TB
- Numerology
- Harq ID
- Start PDU Seg
- Num Complete PDU
- End PDU Seg
- Grant Size
- Bytes Built
- MCE Built Bitmask
- PHR Reason
- BSR Reason
- BSR Type
- MCE Length
- MCE Type
- BSR Index
- LCG

## 20. NR5G L2 UL BSR
### Purpose
This message logs the Buffer Status Reports (BSRs) generated by the UE and sent to the gNB over the UL MAC layer in 5G NR. Each TTI entry corresponds to one BSR MAC CE triggered and transmitted by the UE.

### Content
- Num TTI
- LCID Prio Bitmask
- Slot Number
- FN
- Harq ID
- Carrier ID
- BSR Trigger Reason
- BSR Type
- BSR Length
- LCG Bitmask
- Num LCIDs
- MCE Type
- R
- R1
- BSR Index
- LCG

## 21. NR5G MAC UL TB Stats
### Purpose
This message is to provide a periodic summary of NR uplink MAC transport block transmission activity.

### Content
- Num record
- Tb new tx bytes
- Tb retx bytes
- Num mcs
- Num prb
- Phr
- Max power
- Num new tx tbs
- Num retx tb
- Ti
- Cqi
- Num phr
- Num ulsch sched
- Num no ulsch sched
- Pcmax
- Flush gap cnt

## 22. NR5G MAC DCI Info
### Purpose
This message logs decoded Downlink Control Information (DCI) received by the UE in 5G NR. This log captures both uplink (UL DCI) and downlink (DL DCI) types, showing complete parsing results for every DCI command.

### Content
- Num Records
- Numerology
- Frame
- Num DCI
- Raw DCI Included
- Agg Level
- DCI Format
- RNTI Type
- Raw DCI Payload
- NDI
- MCS
- RV
- HARQ ID
- PUSCH TPC
- Symbol Alloc Index
- RB Assignment
- Precoding Layer Info
- DAI 1
- Antenna Ports
- DL Assignment Index
- TB New Data Indicator
- TB MCS
- Time Resource Assignment 
- PDSCH Harq Feedback Timing
- PUCCH Resource Indicator
- TPC Command For Sched PUCCH

## 23. NR5G MAC DL TB Report
### Purpose
This message is generated by the UE and serves as a report on downlink control message reception. It indicates whether the UE has successfully received and applied certain control-level MAC elements.

### Content
- Num Records
- slot
- numerology
- frame
- num_ca
- MCE Bitmask
- TAG Id
- TA Command

## 24. NR5G MAC PDSCH Status
### Purpose
This msg captures downlink PDSCH transport block (TB) status information at the MAC layer on the UE side.

### Content
- Num Records
- Slot
- Numerology
- Frame
- Num PDSCH Status
- SFN
- Bandwidth
- Physical Cell ID
- EARFCN
- TB Size
- MCS
- Num Rbs
- RV
- NDI
- HARQ Or MBSFN Area Id
- Num Layers Raw
- K1 Or PMCH ID
- RNTI Type
- Num ReTx
- Num RX
- Mod Type

## 25. NR5G MAC PDSCH Stats
### Purpose
This message provides cumulative statistics of PDSCH downlink decoding on the UE over time. 

### Content
- Num Records
- Num Total Slots
- Num CA
- Carrier ID
- Num Slots Elapsed
- Num PDSCH Decode
- Num CRC Pass TB
- Num CRC Fail TB
- Num ReTx
- ACK As NACK
- HARQ Failure
- CRC Pass TB Bytes
- CRC Fail TB Bytes
- TB Bytes
- Padding Bytes
- ReTx Bytes

## 26. NR5G MAC RACH Trigger
### Purpose
This msg records the triggering of a Random Access (RACH) procedure on the UE. 

### Content
- num_records
- Frame
- SubFrame
- Slot
- SCS
- C-RNTI
- Rach Reason
- CA ID
- First Active UL BWP
- Active DL BWP
- UL RACH Rsrc. Present
- DL RACH Rsrc. Present

## 27. NR5G MAC CDRX Events Info
### Purpose
This msg provides insights into the behavior and configuration of Connected Mode Discontinuous Reception (CDRX) at the MAC layer. CDRX helps reduce UE power consumption by allowing it to periodically turn off RF and baseband processing when inactive.

### Content
- Num Records
- DRX Enable
- On Duration Time
- Inactivity Timer
- DL HARQ RTT Timer
- UL HARQ RTT Timer
- DL Retransmission Timer
- UL Retransmission Timer
- Long DRX Cycle Start Offset
- Long DRX Cycle
- Short DRX Cycle Enable
- Short DRX Cycle Timer
- Short DRX Cycle
- DRX Slot Offset
- System Frame Number
- Slot Number
- reason
- Current State
- Current Ref Count
- Short Cycle Timer
- Last Grant timer
- Active Procedure

## 28. NR5G MAC UCI Information
### Purpose
This msg captures Uplink Control Information (UCI). 

### Content
- Num Records
- Slot
- Numerology
- Frame
- Num UCI Tx
- Channel Type
- Total Harq Ack Bits
- UCI Request Bitmask
- Num Symbols
- Num SR
- Num CSF
- Num HARQ
- HARQ Id
- Bit Offset
- Num Bits
- CRC Status
- SR Request
- Report Id

## 29. NR5G MAC Flow Control
### Purpose
This msg captures uplink transmission throttling mechanisms used by the UE. It reflects why and how uplink flow control is being applied to adjust data/control transmission rates in response to network or radio conditions.

### Content
- Num Records
- FC Reason
- Initial P Backoff
- P Min
- T Up
- T Down
- Step Timer
- PUCCH Period
- Configured Throttle
- Current Throttle
- Cool Down Time
- Step Timer

## 30. NR5G MAC CSF Report
### Purpose
This msg represents CSI (Channel State Information) feedback reports sent from the UE to the gNB. These are essential for downlink scheduling decisions and link adaptation.

### Content
- Num Records
- Slot
- Numerology
- Frame
- Num CSF Reports
- Report ID
- Report Quantity Bitmask
- Num CSI P1 Bits
- Cri Ssbri Bit Width
- Resource Set ID
- RSRP Bit Width
- RSRP
- Ssbri Cri
- CSI Metrics
- CSI Bit Width

## 31. NR5G ML1 Searcher Measurement DB Update
### Purpose
This msg records raw RF measurement data collected by the ML1 layer's Searcher module. 

### Content
- Slot Number
- SubFrame Number
- System Frame Number
- Num Layers
- Frequency Offset Raw
- Timing Offset
- CC_ID
- Num Cells
- Serving Cell PCI
- Serving Cell Index
- Servingsubarray Id
- Serving SRB
- Serving RFIC ID
- Serving RX BEAM
- SubArray ID
- PCI
- PBCH SFN
- Num Beams
- SSB Index
- SSB Ref Timing 1
- SSB Ref Timing 2

## 32. NR5G RRC OTA Packet
### Purpose
The purpose of this message is to record the actual 3GPP-defined NR RRC signaling content transmitted over the air between the UE and the network.

### Content
Follow the content defined in 3gpp NR RRC standard.

## 33. NR5G MAC RACH Attempt
### Purpose
This msg records the details of a single RACH attempt by the UE in NR5G, including Msg1/Msg2 timestamps, SSB/RA-RNTI identifiers, contention type, TA, and result.

### Content
- num_records
- Num Attempts
- SSB ID
- Frame
- SubFrame
- Slot
- PRACH Config
- Uroot
- RAID
- FDM
- Cyclic Shift V
- N_CS
- RA_RNTI
- Regular Pathloss
- Pmin Pathloss
- Max Backoff Duration
- T_RNTI
- TA Value

## 34. NR5G MAC UCI Payload Information
### Purpose
This message captures the actual payload of Uplink Control Information (UCI) sent by the UE over PUSCH.

### Content
- Num Records
- Slot
- Frame
- Num Channels
- Channel
- Start Symbol
- Num Symbols
- Num HARQ Bits
- Num SR Bits
- Num CSF P1 Bits
- Num CSF P2 Bits
- Num HARQ Payload
- Num SR Payload
- Num CSI P1 Payload
- Num CSI P2 Payload

## 35. NR5G ML1 Searcher Conn Eval
### Purpose
This msg represents the connection evaluation status performed by ML1 layer. It shows which NR cells are currently under evaluation for possible mobility actions, like reselection or handover.

### Content
- Slot Number
- SubFrame Number
- System Frame Number
- Num Cells
- State
- Meas Id
- Num Reports Sent
- Cell Id
- TTT Remaining

## 36. NR5G RLC UL Status PDU
### Purpose
This message captures uplink RLC STATUS PDUs that are transmitted in 5G NR using RLC Acknowledged Mode (AM).

### Content
- Num Status PDU
- Num Status PDU Dropped
- Slot Number
- FN
- LCID
- SN Length
- Size
- Pad Size
- Action Type
- PDU Type

## 37. NR5G ML1 Antenna Switch Diversity
### Purpose
This message records NR ML1 antenna-switch diversity attempts for the serving carrier, including the target switching mode, per-chain old/new antenna mapping, and the reason why the switch was not performed.

### Content
- Subframe
- System frame
- Slot
- Scs
- Switch Reason
- Switch Mode
- Switch Denial Reason
- Old Antenna Id
- Switch Type
- New Antenna Id

## 38. NR5G LL1 FW Serving FTL
### Purpose
This message records how the UE’s NR L1 firmware tracks the serving cell in time and frequency.

### Content
- SSB or TRS
- Cell Id
- Subfn
- Starting Sfn
- Snr1 Raw
- Snr2 Raw
- Snr3 Raw
- Snr4 Raw
- NRarfcn
- Dl frequency
- Dl frequency low
- Frequency Error
- Frame Offset Vstm Ref

## 39. NR5G ML1 Search HO Acq Confirm
### Purpose
This message confirms the outcome of ML1 handover acquisition toward a candidate target cell and to provide the radio measurements and beam-detection results used to support mobility/handover processing.

### Content
- Slot Number
- Scs
- System Frame Number
- Subframe Number
- Arfcn
- Cell id
- Num ssb detected
- Best tx beam id
- SSS rsrp
- SSS rsrq
- SSS sinr
- Tx beam pair
- Arfcn center frequency
- Frequency offset


## 40. NR5G L2 UL Data Pdu
### Purpose
This message logs the UE’s uplink Layer 2 data transmission behavior in 5G NR, covering each Transmission Time Interval (TTI). 

### Content
- Num TTI
- Num LCID Configured
- LCID
- RLC SN Length
- PDCP SN Length
- Tx Next Ack
- Tx Next
- Slot Number
- FN
- Num TB
- Num TB Entry
- MCE Built
- BSR Type
- Numerology
- Num LC Built
- Num LC Packet Info Entries
- PDU info count
- Num First Pkt PDCP Hdr Bytes
- Num Last Pkt PDCP Hdr Bytes
- Last Poll SN
- rlc_start_sn
- PDCP Start Count
- Num RLC Status PDUs
- Num PDCP Control PDUs
- DC Bit Outer
- PDCP RB Type
- SN First 4 Bits
- SN Next 8 Bits
- SN Last 8 Bits
- SN 18 Bits
- Num Packets
- Offset

## 41. NR5G LL1 FW RX Control AGC
### Purpose
This message reports the status of the Automatic Gain Control (AGC) on the RX path, including the gain state of the LNA (Low Noise Amplifier) and the RSSI (Received Signal Strength Indicator) values for each receiver chain.

### Content
- Variant ID
- Log2 Slot To Unit Slot
- Fractional Unit Slot Number
- System Frame Number
- Log Size


## 42. NR5G MAC UL Physical Channel Schedule Report
### Purpose
This message represents the 5G NR MAC Uplink Physical Channel Schedule Report, generated by the MAC layer (L2). It captures detailed information about scheduled UL resources in specific slots.

### Content
- Num Records
- Slot
- SCS
- Frame
- Num Carrier
- Phychan Bit Mask
- UL Slot fstmr
- Total Timing Adj
- MAC NTA
- Timing Adj Adv Ret
- Timing Adj TC
- Num SRS
- Resource Id
- Config Index
- Num Ports
- Starting Symbol
- Num Symbols
- HARQ ID
- RV Index
- MCS
- RB Start
- Num RBs
- Start Symbol
- TX Slot Offset
- UCI Request Mask
- TB Size
- DMRS Symbol Bitmask
- RNTI Value
- RB Start Hop
- Num HARQ ACK Bits
- Num CSF P1 Bits
- Num CSF P2 Bits
- SRS Res Ind
- Modulation Order
- Transform Precoding
- Code Rate
- CB Size
- K Prime Value
- ZC Value
- Num CBs
- RBG Bitmap
- Num Gap RBs
- Num PUCCH
- PUCCH Format
- UCI Request BMask
- Start symbol
- Starting RB
- Num RB
- Second Hop RB
- Num UCI P1 Bits
- M0
- Time OCC Index
- DFT OCC Length
- DFT OCC Index
