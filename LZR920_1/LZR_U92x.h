#include "stm32f7xx_hal.h"
#include "main.h"

#define LZR_U92x
#define LZR_U920
//#define MICROSCAN
//#define FLATSCAN

#if defined(LZR_U92x)

#define LZR_1							0
#define LZR_2							1

#define PLANE_0 						0
#define PLANE_1 						1
#define PLANE_2 						2
#define PLANE_3 						3

#define U92x_CNT						1
#define U92x_DATA_CNT					274

#define ENABLED							0x01
#define DISABLED						0x00

#define IMMINITY_1						1
#define IMMINITY_2						2
#define IMMINITY_3						3
#define IMMINITY_4						4

#define SYNC_SIZE						4
#define PACKET_SIZE 					2
#define CMD_SIZE 						2
#define ID_FRAME_SIZE 					6
#define INFORMATION_SIZE				14
#define PLANE_NUM_SIZE 					1
#define PLANE_SIZE 						548
#define CHK_SIZE 						2
#define SET_CONFIG_SIZE 				33

#define CONFIGURATION_MODE				2
#define MEASURE_MODE					1
#define RED_ON							1

#define SYNC_0					    	0xFC
#define SYNC_1		          			0xFD
#define SYNC_2		          			0xFE
#define SYNC_3		          			0xFF
#define SIZE_L		          			0x04
#define SIZE_H		          			0x05
#define CMD_L		          			0x06
#define CMD_H							0x07
#define DIST_DATA						0x08
#define CONFIG_DATA						0x09
#define INFO_DATA						0x0A
#define CHK_0							0x0B
#define CHK_1							0x0C

#define MEASURE_MODE 					1
#define CONFIGURATION_MODE 			    2

#define SET_CONFIG_MODE					0x40

#define SETRAWDATAMODE					0x51
#define SETRAWDATACONFIG				0x53
#define SETRAWDATACONFIGSTORE			0x55
#define SETRAWDATACONFIGRESTORE			0x57
#define SETRAWDATAREDLASER				0x59
#define SETRAWDATAERRORLOGRESET			0x56
#define SETRAWDATAHEARTBEAT				0x5C
#define SETRAWDATAFRAMECOUNTERRESET		0x5E

#define GETRAWDATAMODE 					0x52
#define GETRAWDATACONFIG				0x54
#define GETRAWDATAINFORMATION			0x58
#define GETRAWDATAREDLASER				0x5A
#define GETRAWDATADISTANCEVALUES		0x5B
#define GETRAWDATAHEARTBEAT				0x5D

#define SET_CONFIG_PACKET_SIZE (SYNC_SIZE+PACKET_SIZE+CMD_SIZE+SET_CONFIG_SIZE+CHK_SIZE)

#if defined(LZR_U920)
#define LZR_RECIEVE_PACKET_TOTAL \
	(/*CMD_SIZE*/ + ID_FRAME_SIZE + INFORMATION_SIZE +  (PLANE_NUM_SIZE*4) + (PLANE_SIZE*4) /*+ CHK_SIZE*/)
#else
#define LZR_RECIEVE_PACKET_TOTAL \
	(/*CMD_SIZE*/ + ID_FRAME_SIZE + INFORMATION_SIZE +  PLANE_NUM_SIZE + PLANE_SIZE /*+ CHK_SIZE*/)
#endif

//Config field address part
#define CONFIG_BIT_MASK							0
#define CONFIG_COMMUNICATION_CHARGE				4
#define CONFIG_BAUD_RATE						6
#define CONFIG_LZR_INFORMATION					8
#define CONFIG_RED_LASER_TIMEOUT				9
#define CONFIG_TEST_FRAME						10
#define CONFIG_PLANE_0_OnOff					11
#define CONFIG_PLANE_1_OnOff					12
#define CONFIG_PLANE_2_OnOff					13
#define CONFIG_PLANE_3_OnOff					14
#define CONFIG_PULSE_WIDTH						15
#define CONFIG_NUMBER_OF_DISTANCE_VALUES		16
#define CONFIG_STARTING_SPOT_NUMBER				18
#define CONFIG_JUMP_BETWEEN_2_SUCCESSIVE_SPOTS	20
#define CONFIG_APD_DISTANCE_RANGE				22
#define CONFIG_CAN_ID_FRAME_COUNTER				23
#define CONFIG_DIODE_LIFETIME_MANAGEMENT		24
#define CONFIG_POLARITY_OF_INPUT				25
#define CONFIG_DELAY_OF_HEARTBEAT				26
#define CONFIG_LED_1							27
#define CONFIG_LED_2							28
#define CONFIG_LED_BLUE							29
#define CONFIG_LED_ERROR						30
#define CONFIG_DURATION_OF_LED					31
#define CONFIG_MAXIMUM_DISTANCE_RANGE			32
#define CONFIG_PLAN_NUMBER_OnOff				34
#define	CONFIG_IMMUNITY							35
#define	CONFIG_HOT_RESET_TIMER					36
#define	CONFIG_HOT_RESET_COUNTER				38

#define INFO_ERROR_1			0
#define INFO_ERROR_2			1
#define INFO_ERROR_3			2
#define INFO_ERROR_4			3
#define INFO_ERROR_5			4
#define INFO_ERROR_6			5
#define INFO_ERROR_7			6
#define INFO_ERROR_8			7
#define INFO_ERROR_9			8
#define INFO_HOT_RESET_COUNTER	9
#define INFO_CTN				10
#define INFO_VNR				12
#define INFO_CAN_ID				14
#define INFO_VERSION			18

typedef struct _Node
{
	unsigned char sync;
	unsigned char cmd;
	unsigned char ready;
	unsigned short lzr_packet_length;
	unsigned short check_sum;
	unsigned short check_sum_cmp;
	unsigned short buffer_index;

	unsigned char lzr_recieved_packet[LZR_RECIEVE_PACKET_TOTAL];
	struct _Node *next;
}Node;


int ALLOC_MEMORY(void);
unsigned char rx_function(unsigned char port_num, unsigned char uch);

unsigned char SET_RX_BUFFER_NEXT(unsigned char port_num);
unsigned char GET_RX_BUFFER_READY(unsigned char port_num);
unsigned char GET_RX_BUFFER_CMD(unsigned char port_num);
unsigned char GET_RX_BUFFER_BUSY(unsigned char port_num);
unsigned short GET_RX_BUFFER_LENGTH(unsigned char port_num);
void SET_RX_BUFFER_SHORT_DATA(unsigned char *buff, unsigned int index, unsigned short data);
unsigned short GET_SHORT_DATA(unsigned char *buff, unsigned int index);
int GET_INT_DATA(unsigned char *buff, unsigned int index);
unsigned char *GET_RX_BUFFER(unsigned char port_num, unsigned int index);

unsigned char SEND_COMMAND(unsigned char port_num, unsigned char cmd, unsigned char option);
unsigned char SEND_CONFIG_COMMAND(unsigned char port_num);
unsigned char SET_CONFIG_FIELD(unsigned char port_num, int address, unsigned short data);
unsigned short GET_CONFIG_FIELD(unsigned char port_num, int address);
unsigned char GET_PLANE_NUM(unsigned char num);

void SET_SENSOR_CONFIG_FLAG(unsigned char port_num);
unsigned char CHECK_SENSOR_CONFIG_FLAG(unsigned char port_num);

void SET_BACKGROUND_DATA_FLAG(unsigned char port_num);
unsigned char TEACH_IN(unsigned char port_num, unsigned int loop_cnt, unsigned short off_set);
void SET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index, unsigned short data);
unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index);
unsigned short * GET_BACKGROUND_DATA_ARRAY(unsigned char port_num, int plane_num);
void PRINT_BACKGROUND(unsigned char port_num);

unsigned char SET_CONFIG_SENSOR(unsigned char port_num, unsigned char cmd, unsigned char option);
unsigned char GET_SENSOR_READY(unsigned char port_num);
unsigned int GET_RX_BUFFER_DISTANCE_DATA(
		unsigned port_num, unsigned char *plane0, unsigned char *plane1, unsigned char *plane2, unsigned char *plane3,	unsigned char *frame_data,	unsigned char *info_data
		);

void SET_PLANE_NUM_ENABLED(unsigned char port_num, unsigned short value);
void SET_FRAME_COUNTER_ENABLED(unsigned char port_num, unsigned short value);
void SET_INFORMATION_ENABLED(unsigned char port_num, unsigned short value);

#define SHORT	2
#define INTEGER	4
int GET_LSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit);
int GET_MSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit);
void SET_LSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit);
void SET_MSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit);

#endif
