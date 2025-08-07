#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Metadata_Header_cb_dcall.h"
// #include "B16C_cb_dcall.h"
// #include "B083_cb_dcall.h"
// // #include "B0A1_cb_dcall.h"
// // #include "B0B6_cb_dcall.h"
// #include "B0CD_cb_dcall.h"
// #include "B116_cb_dcall.h"
// #include "B12A_cb_dcall.h"
// #include "B167_cb_dcall.h"
// #include "B168_cb_dcall.h"
// #include "B169_cb_dcall.h"
// #include "B16A_cb_dcall.h"
// #include "B16D_cb_dcall.h"
// #include "B17A_cb_dcall.h"
// #include "B17D_cb_dcall.h"
// #include "B17F_cb_dcall.h"
// #include "B18F_cb_dcall.h"
// #include "B197_cb_dcall.h"
// #include "B1B2_cb_dcall.h"
// #include "B181_cb_dcall.h"
// #include "B184_cb_dcall.h"
// #include "B196_cb_dcall.h"
// #include "B144_cb_dcall.h"
// #include "B173_cb_dcall.h"
// #include "B064_cb_dcall.h"
// #include "B16C_v48_cb_dcall.h"
// #include "B16C_v48_no_asn.h"
// #include "B173_no_asn.h"
// #include "B064_no_asn.h"
// #include "B139_v161_no_asn.h"
// #include "B887_no_asn.h"
// #include "B885_no_asn.h"
// #include "B883_no_asn.h"
// #include "B872_no_asn.h"

// #include "B814_cb_dcall.h"
// #include "B815_cb_dcall.h"
// #include "B822_cb_dcall.h"
// #include "B823_cb_dcall.h"
// #include "B825_cb_dcall.h"
// #include "B826_cb_dcall.h"
// #include "B840_cb_dcall.h"
// #include "B842_cb_dcall.h"


int S_D_V_decode(uint8_t *hex_data, size_t file_size, int msg_index, int *index_in, uint64_t time_in_us_total, int crc_check, int check_format, int pkt_type, int msg_len, int version_4B);
long get_file_size(const char *filename);
int get_item_id(int type_id);
int get_equip_id(int type_id);
char* generate_cfg(const char* msgtype_list[], int logcode_num);
void decode_header(const char *output_frame, int start_index, uint64_t *msg_header);
const char* map_index_msgtype(int index);
int map_msgtype_index(const char* msgtype);

//LTE
void decode_B083(uint8_t *hex_data, size_t length, int *index);
void decode_B0A1(uint8_t *hex_data, size_t length, int *index);
void decode_B0B6(uint8_t *hex_data, size_t length, int *index);
void decode_B0CD(uint8_t *hex_data, size_t length, int *index);
void decode_B116(uint8_t *hex_data, size_t length, int *index);
void decode_B12A(uint8_t *hex_data, size_t length, int *index);
void decode_B144(uint8_t *hex_data, size_t length, int *index);
void decode_B167(uint8_t *hex_data, size_t length, int *index);
void decode_B168(uint8_t *hex_data, size_t length, int *index);
void decode_B169(uint8_t *hex_data, size_t length, int *index);
void decode_B16A(uint8_t *hex_data, size_t length, int *index);
void decode_B16C(uint8_t *hex_data, size_t length, int *index);
void decode_B16D(uint8_t *hex_data, size_t length, int *index);
void decode_B17A(uint8_t *hex_data, size_t length, int *index);
void decode_B17D(uint8_t *hex_data, size_t length, int *index);
void decode_B17F(uint8_t *hex_data, size_t length, int *index);
void decode_B181(uint8_t *hex_data, size_t length, int *index);
void decode_B184(uint8_t *hex_data, size_t length, int *index);
void decode_B18F(uint8_t *hex_data, size_t length, int *index);
void decode_B196(uint8_t *hex_data, size_t length, int *index);
void decode_B197(uint8_t *hex_data, size_t length, int *index);
void decode_B1B2(uint8_t *hex_data, size_t length, int *index);
void decode_B173(uint8_t *hex_data, size_t length, int *index, uint64_t time_in_us_total, uint16_t logcode);

//5G
void decode_NR5G_NAS_SM5G_Plain_OTA_Outgoing_Msg_v1(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_NAS_MM5G_State_v1(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_NAS_Plain_Message_Container_v1(uint8_t *hex_data, size_t length, int *index, int msg_len);
void decode_NR5G_NAS_MM5G_NSSAI_Info_v1(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RRC_MIB_Info_v1(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RRC_Serving_Cell_Info_v4(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RRC_Configuration_Info_v8(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RRC_Supported_CA_Combos_v7(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_PDCP_DL_Data_Pdu_v6(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_PDCP_DL_Rbs_Stats_v5(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_DL_Config_v5(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RLC_DL_Stats_v196608(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RLC_DL_Status_PDU_v1(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_DL_DATA_PDU_v6(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_PDCP_UL_Stats_v9(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_PDCP_UL_Control_Pdu_v196609(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RLC_UL_Stats_v4(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_UL_Config_v5(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_UL_TB_v4(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_UL_TB_v196616(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_UL_BSR_v196611(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_UL_TB_Stats_v196609(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_DCI_Info_v196617(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_DCI_Info_v131077(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_DL_TB_Report_v3(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_PDSCH_Status_v5(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_PDSCH_Stats_v196608(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_RACH_Trigger_v196615(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_CDRX_Events_Info_v131073(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_UCI_Information_v2(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_UCI_Information_v196608(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_UCI_Payload_Information_v196608(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_Flow_Control_v131073(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_CSF_Report_v131073(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_ML1_Searcher_Measurement_Database_Update_Ext_v131073(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RRC_OTA_Packet_v1660(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_RACH_Attempt_v196616(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RLC_UL_Status_PDU_v196608(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_ML1_Searcher_Conn_Eval_v131075(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_RLC_UL_Status_PDU_v196609(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_ML1_Antenna_Switch_Diversity_v131074(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_LL1_FW_Serving_FTL_v7(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_ML1_Search_HO_Acq_Confirm_v131079(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_L2_UL_Data_PDU_v196611(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_LL1_FW_RX_Control_AGC_v131072(uint8_t *hex_data, size_t length, int *index);
void decode_NR5G_MAC_UL_Physical_Channel_Schedule_Report_v196623(uint8_t *hex_data, size_t length, int *index);

// waiting
void decode_NAS_800_v1(uint8_t *hex_data, size_t length, int *index, int msg_len, uint64_t time_in_us_total);
void decode_NAS_80A_v1(uint8_t *hex_data, size_t length, int *index, int msg_len, uint64_t time_in_us_total);


#ifdef __cplusplus
}
#endif