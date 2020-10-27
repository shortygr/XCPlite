/*----------------------------------------------------------------------------
| File:
|   A2L.c
|
| Description:
|   Generate A2L file
|   Linux (Raspberry Pi) Version
 ----------------------------------------------------------------------------*/


#include "A2L.h"

#define MAX_EVENT 256

FILE* gA2lFile = 0;
unsigned int gA2lEvent = 0;
unsigned int gA2lEventCount = 0;
char* gA2lEventList[MAX_EVENT];

char* gA2lHeader =
"/* generated by XCPlite */\n"
"ASAP2_VERSION 1 71\n"
"/begin PROJECT XCPlite \"\"\n"
"/begin HEADER \"\" VERSION \"1.0\" /end HEADER\n"
"/begin MODULE XCPlite \"\"\n"
"/include \"XCP.AML\"\n"
"/begin MOD_COMMON \"\"\n"
"BYTE_ORDER MSB_LAST\n"
"ALIGNMENT_BYTE 1\n"
"ALIGNMENT_WORD 1\n"
"ALIGNMENT_LONG 1\n"
"ALIGNMENT_FLOAT16_IEEE 1\n"
"ALIGNMENT_FLOAT32_IEEE 1\n"
"ALIGNMENT_FLOAT64_IEEE 1\n"
"ALIGNMENT_INT64 1\n"
"/end MOD_COMMON\n";

char* gA2lIfData1 =
"/begin IF_DATA XCP\n"
"/begin PROTOCOL_LAYER\n"
"0x0103 0x03E8 0x2710 0x00 0x00 0x00 0x00 0x00 0xFA 0x0574 BYTE_ORDER_MSB_LAST ADDRESS_GRANULARITY_BYTE\n"
"OPTIONAL_CMD GET_COMM_MODE_INFO\n"
"/end PROTOCOL_LAYER\n"
"/begin DAQ\n"
"DYNAMIC 0x00 0x03 0x00 OPTIMISATION_TYPE_DEFAULT ADDRESS_EXTENSION_FREE IDENTIFICATION_FIELD_TYPE_RELATIVE_BYTE GRANULARITY_ODT_ENTRY_SIZE_DAQ_BYTE 0xF8 OVERLOAD_INDICATION_PID\n"
"/begin TIMESTAMP_SUPPORTED\n"
"0x01 SIZE_DWORD UNIT_1US TIMESTAMP_FIXED\n"
"/end TIMESTAMP_SUPPORTED\n";

char* gA2lIfData2 = 
"/end DAQ\n"
"/begin PAG\n"
"0x00\n"
"/end PAG\n"
"/begin PGM\n"
"PGM_MODE_ABSOLUTE 0x00 0x00\n"
"/end PGM\n"
"/begin XCP_ON_TCP_IP 0x0100 0x15B3 ADDRESS \"172.31.31.194\" /end XCP_ON_TCP_IP\n"
"/begin XCP_ON_UDP_IP 0x0103 0x15B3 ADDRESS \"172.31.31.194\" /end XCP_ON_UDP_IP\n"
"/end IF_DATA\n"
;

char* gA2lFooter =
"/end MODULE\n"
"/end PROJECT\n"
;

void A2lInit(void) {
	gA2lFile = 0;
	gA2lEvent = 0;
	gA2lEventCount = 0;
}

void A2lHeader(void) {

  gA2lFile = fopen(kXcpA2LFilenameString, "w");
  fprintf(gA2lFile, gA2lHeader);
  fprintf(gA2lFile, gA2lIfData1);
  for (int i =0; i<gA2lEventCount; i++) fprintf(gA2lFile, "/begin EVENT \"%s\" \"%s\" 0x%X DAQ 0xFF 0x01 0x06 0x00 /end EVENT\n", gA2lEventList[i], gA2lEventList[i], i+1);
  fprintf(gA2lFile, gA2lIfData2);

}


void A2lCreateEvent(const char* name) {
	if (gA2lEventCount >= MAX_EVENT) return;
	gA2lEventList[gA2lEventCount] = name;
	gA2lEventCount++;
}

void A2lSetEvent(unsigned int event) {
	gA2lEvent = event;
}

void A2lCreateMeasurement_(char* name, int size, unsigned long addr) {

	char* type, *min, *max;
	
	switch (size) {
	case -1: type = "SBYTE"; min = "-128", max = "127";  break;
	case -2: type = "SWORD"; min = "-32768", max = "32767";  break;
	case -4: type = "SLONG"; min = "-2147483648", max = "2147483647";  break;
	case -8: type = "A_SINT64"; min = "-1E12", max = "1E12"; break;
	case 1: type = "UBYTE"; min = "0", max = "255";  break;
	case 2: type = "UWORD"; min = "0", max = "65535";  break;
	case 4: type = "ULONG"; min = "0", max = "4294967295";  break;
	case 8: type = "A_UINT64"; min = "0", max = "1E12"; break;
	default: return;
	}
	fprintf(gA2lFile,
		"/begin MEASUREMENT %s \"\" %s NO_COMPU_METHOD 0 0 %s %s ECU_ADDRESS 0x%X",
		name, type, min, max, addr); 
	if (gA2lEvent > 0) {
		fprintf(gA2lFile,
			" /begin IF_DATA XCP /begin DAQ_EVENT FIXED_EVENT_LIST EVENT 0x%X /end DAQ_EVENT /end IF_DATA",
			gA2lEvent);
	}
	fprintf(gA2lFile," /end MEASUREMENT\n");
}

void A2lClose(void) {

	fprintf(gA2lFile, gA2lFooter);
	fclose(gA2lFile);
}





#if 0


/* generated by XCPlite */
ASAP2_VERSION 1 71
/ begin PROJECT XCPlite ""
/ begin HEADER "" VERSION "1.0" / end HEADER
/ begin MODULE XCPlite ""
/ begin MOD_COMMON ""
BYTE_ORDER MSB_LAST
ALIGNMENT_BYTE 1
ALIGNMENT_WORD 1
ALIGNMENT_LONG 1
ALIGNMENT_FLOAT16_IEEE 1
ALIGNMENT_FLOAT32_IEEE 1
ALIGNMENT_FLOAT64_IEEE 1
ALIGNMENT_INT64 1
/ end MOD_COMMON
/ begin IF_DATA XCP
/ begin PROTOCOL_LAYER
0x0103 0x03E8 0x2710 0x00 0x00 0x00 0x00 0x00 0xFA 0x0574 BYTE_ORDER_MSB_LAST ADDRESS_GRANULARITY_BYTE
OPTIONAL_CMD GET_COMM_MODE_INFO
/ end PROTOCOL_LAYER
/ begin DAQ
DYNAMIC 0x00 0x03 0x00 OPTIMISATION_TYPE_DEFAULT ADDRESS_EXTENSION_FREE IDENTIFICATION_FIELD_TYPE_RELATIVE_BYTE GRANULARITY_ODT_ENTRY_SIZE_DAQ_BYTE 0xF8 OVERLOAD_INDICATION_PID
/ begin TIMESTAMP_SUPPORTED
0x01 SIZE_DWORD UNIT_1US TIMESTAMP_FIXED 
/ end TIMESTAMP_SUPPORTED
/ begin EVENT "Event1" "Event1" 0x01 DAQ 0xFF 0x01 0x06 0x00 / end EVENT
/ end DAQ
/ begin PAG
0x00
/ end PAG
/ begin PGM
PGM_MODE_ABSOLUTE 0x00 0x00
/ end PGM
/ begin XCP_ON_TCP_IP
0x0100 0x15B3 ADDRESS "172.31.31.194"
/ end XCP_ON_TCP_IP
/ begin XCP_ON_UDP_IP
0x0103 0x15B3 ADDRESS "172.31.31.194" / end XCP_ON_UDP_IP
/ end IF_DATA






/ begin CHARACTERISTIC byteArray1 ""
VAL_BLK 0x32290 __UByte_Value 0 NO_COMPU_METHOD 0 255
ECU_ADDRESS_EXTENSION 0x0
EXTENDED_LIMITS 0 255
FORMAT "%.15"
MATRIX_DIM 1400
SYMBOL_LINK "byteArray1" 0
/ begin IF_DATA CANAPE_EXT
100
LINK_MAP "byteArray1" 0x32290 0 0 0 1 0x87 0
DISPLAY 0x0 0 255
/ end IF_DATA
/ end CHARACTERISTIC






/ begin CHARACTERISTIC gCmdCycle "Command handler cycle time"
VALUE 0x2519C __ULong_Value 0 NO_COMPU_METHOD 0 4294967295
PHYS_UNIT "us"
SYMBOL_LINK "gCmdCycle" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gExit "Terminate application"
VALUE 0x251CC __UByte_Value 0 NO_COMPU_METHOD 0 255
SYMBOL_LINK "gExit" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gFlushCycle "Flush cycle time"
VALUE 0x251A0 __ULong_Value 0 NO_COMPU_METHOD 0 4294967295
PHYS_UNIT "us"
SYMBOL_LINK "gFlushCycle" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gTaskCycleTimerECU "ECU cycle time (ns delay)"
VALUE 0x251A4 __ULong_Value 0 NO_COMPU_METHOD 0 4294967295
EXTENDED_LIMITS 0 4294967295
PHYS_UNIT "ns"
SYMBOL_LINK "gTaskCycleTimerECU" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gTaskCycleTimerECUpp "ECU cycle time (ns delay)"
VALUE 0x251A8 __ULong_Value 0 NO_COMPU_METHOD 0 4294967295
EXTENDED_LIMITS 0 4294967295
PHYS_UNIT "ns"
SYMBOL_LINK "gTaskCycleTimerECUpp" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gTaskCycleTimerServer "Server loop cycle time (ns delay)"
VALUE 0x25198 __ULong_Value 0 NO_COMPU_METHOD 0 4294967295
PHYS_UNIT "ns"
SYMBOL_LINK "gTaskCycleTimerServer" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gXcpDebugLevel "Debug verbosity"
VALUE 0x251AC __UByte_Value 0 NO_COMPU_METHOD 0 255
SYMBOL_LINK "gXcpDebugLevel" 0
/ end CHARACTERISTIC

/ begin CHARACTERISTIC gXcpStationId "A2L filename"
VALUE 0x14108 __UByte_Value 0 NO_COMPU_METHOD 0 255
SYMBOL_LINK "gXcpStationId" 0
/ end CHARACTERISTIC


/ begin CHARACTERISTIC map1 "8*8 BYTE"
MAP 0x25108 MapV88ub 0 NO_COMPU_METHOD 0 2550
/ begin AXIS_DESCR
FIX_AXIS NO_INPUT_QUANTITY map1Input_Conversion 8 - 12.8 12.7
EXTENDED_LIMITS - 12.8 12.7
FIX_AXIS_PAR_DIST 0 1 8
FORMAT "%.3"
/ end AXIS_DESCR
/ begin AXIS_DESCR
FIX_AXIS NO_INPUT_QUANTITY map1Input_Conversion 8 - 12.8 12.7
EXTENDED_LIMITS - 12.8 12.7
FIX_AXIS_PAR_DIST 0 1 8
FORMAT "%.3"
/ end AXIS_DESCR
BYTE_ORDER MSB_LAST
ECU_ADDRESS_EXTENSION 0x0
EXTENDED_LIMITS 0 2550
FORMAT "%.3"
SYMBOL_LINK "map1_8_8_uc" 0
/ begin IF_DATA CANAPE_EXT
100
LINK_MAP "map1_8_8_uc" 0x25108 0 0 0 1 0x87 0
DISPLAY 0x0 0 255
/ end IF_DATA
/ end CHARACTERISTIC



/ begin MEASUREMENT sbytePWMLevel ""
SBYTE sbytePWMLevel.CONVERSION 0 0 0 255
ECU_ADDRESS 0x251C0
ECU_ADDRESS_EXTENSION 0x0
FORMAT "%.15"
SYMBOL_LINK "sbytePWMLevel" 0
/ begin IF_DATA CANAPE_EXT
100
LINK_MAP "sbytePWMLevel" 0x251C0 0 0 0 1 0xC7 0
DISPLAY 0x0 0 255
/ end IF_DATA
/ end MEASUREMENT




/ begin COMPU_METHOD Factor100 ""
LINEAR "%3.1" ""
COEFFS_LINEAR 0.01 0
/ end COMPU_METHOD




/ begin COMPU_VTAB TestStatus.CONVERSION "" TAB_VERB 4
0 "Off"
1 "Silent"
2 "Pending"
3 "Running"
DEFAULT_VALUE ""
/ end COMPU_VTAB




/ begin GROUP Test_Parameters "Test Parameters, BYTE"
/ begin REF_CHARACTERISTIC
CALRAM_LAST CALRAM_SIGN CALRAM_START gCmdCycle gDebugLevel gExit gFlushCycle
gSocketTimeout gTaskCycle gTaskCycleTimerCMD gTaskCycleTimerDAQ gTaskCycleTimerECU gTaskCycleTimerECUpp gTaskCycleTimerServer gXcpDebugLevel
/ end REF_CHARACTERISTIC
/ end GROUP






#endif

