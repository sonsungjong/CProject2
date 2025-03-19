#ifndef FLATSCAN_H_
#define FLATSCAN_H_
#include "stm32f7xx_hal.h"
#include "main.h"

#ifdef FLATSCAN

#define FULL_DUPLEX
//#define FLATSCAN

#define U951_1						0
#define U951_2						1
#define U951_CNT					1
#define PACKET_BUFFER_COUNT			3

//#define FLATSCAN_HW_ERROR


#define MODE_HD						1
#define MODE_HS						0

#ifdef MODE_HD
	#define MAX_POINT					400
#else
	#define MAX_POINT					100
#endif

#define ENABLED						0x01
#define DISABLED					0x00

#define IMMUNITY_0					0
#define IMMUNITY_1					1
#define IMMUNITY_2					2
#define IMMUNITY_3					3
#define IMMUNITY_4					4

#define SYNC_SIZE					11
#define CMD_SIZE 					2
#define SYNC_CMD_SIZE				SYNC_SIZE+CMD_SIZE

#define CAN_SIZE 					4
#define CNTR_SIZE					2
#define CAN_CNTR_SIZE				6
#define CTN_SIZE					2
#define FACET_SIZE					1
#define DISTANCE_SIZE				800
#define REMISSIONS_SIZE				800
#define CHK_SIZE 					2

#define SYNC_CMD_CHK_SIZE		   	SYNC_SIZE+CMD_SIZE+CHK_SIZE

#define BAUD_RATE_57600				0
#define BAUD_RATE_115200			1
#define BAUD_RATE_230400			2
#define BAUD_RATE_460800			3
#define BAUD_RATE_921600			4
#define SINGLE_SHOT					0
#define CONTINUOUS					1

#define SINGLE_LED					1
#define BLINK_LED					2
#define LED_OFF						0
#define RED							1
#define GREEN						2
#define ORANGE						3

#define SYNC_0						0xbe
#define SYNC_1		        		0xa0
#define SYNC_2		        		0x12
#define SYNC_3		        		0x34
#define SYNC_4		        		0x05
#define SYNC_5		        		0x06
#define SYNC_6		        		0x07
#define SYNC_7		        		0x08
#define SYNC_8		        		0x09
#define SYNC_9		        		0x0a
#define SYNC_10		        		0x0b
#define CMD_L		        		0x0c
#define CMD_H						0xc3
#define DATA						0x10
#define CHK_0						0x21
#define CHK_1						0x22



#define SET_BAUDRATE				0x51
#define SET_PARAMETERS				0x53
#define GET_PARAMETERS				0x54
#define SEND_PARAMETERS				0x54
#define STORE_PARAMETERS			0x55
#define GET_IDENTITY				0x5a
#define GET_MEASUREMENTS			0x5b
#define MDI							0x5b
#define GET_CAN_NUMBER				0x5d
#define SEND_CAN_NUMBER				0x5d
#define RESET_MDI_COUNTER			0x5e
#define RESET_HEARBEAT_COUNTER		0x5f
#define RESERT_ENERGENCY_COUNTER	0x61
#define HEARTBEAT					0x64
#define GET_EMERGENCY				0x6e
#define EMERGENCY					0x6e
#define SET_LED						0x78


#define LZR_RECIEVE_PACKET_TOTAL 	1624

typedef struct{
	unsigned char bit_mask_d0;
	unsigned char bit_mask_d1;
	unsigned char bit_mask_d2;
	unsigned char bit_mask_d3;

	//IDENTITY
	unsigned int product_part_number;
	unsigned char sw_version;
	unsigned char sw_revision;
	unsigned char sw_prototype;
	unsigned int can_number_of_the_detector;

	//parameters
	unsigned short communication_charge;
	unsigned char ctn_field_status;
	unsigned char info_in_mdi;
	unsigned char detection_field_mode;
	unsigned char immunity_level;
	unsigned short number_of_spots;
	unsigned short angle_first;
	unsigned short angle_last;
	unsigned char can_n_frame_fields;
	unsigned char heartbeat_period;
	unsigned char facet_number_field;
	unsigned char averaging_setting;

}CONFIG_DATA;


typedef struct Node
{
	__IO ITStatus ready;
	unsigned short check_sum;
	unsigned short length;
	unsigned char sync;
	unsigned char cmd;

	int f_index;

	unsigned char recieved_rawdata[LZR_RECIEVE_PACKET_TOTAL];
	struct Node *next;
}Node;

extern CONFIG_DATA config_data[U951_CNT];
extern Node *recieved_buffer[U951_CNT];
extern Node *read_buffer[U951_CNT];
extern unsigned short* baseline_plane[U951_CNT];

Node* Make_buffer(unsigned char buffer_cnt);
Node* CreateNode(void);
unsigned char buff_changer(unsigned char port_num, unsigned char uch);
unsigned short GET_SHORT_DATA(unsigned char *buff, unsigned int index);
void SET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index, unsigned short data);
unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index);
int ALLOC_MEMORY(void);
void CONFIG_LZR(unsigned char port_num);
void teach_in(unsigned char port_num , unsigned int count, unsigned short off_set);
unsigned short CRC16_function(unsigned char *buf_,unsigned short cnt_);
unsigned char read_buffer_ready(unsigned char port_num);
void move_next_read_buffer(unsigned char port_num);
unsigned char read_buffer_cmd(unsigned char port_num);
unsigned short read_buffer_length(unsigned char port_num);
unsigned char *rawdata_copy(unsigned char port_num, unsigned int index);
double read_config_angle_first(unsigned char port_num);
double read_config_angle_last(unsigned char port_num);
void set_baudrate(unsigned char port_num, unsigned char baud_rate);
void get_measurements(unsigned char port_num, unsigned char mode);
void set_default_config_data(int port_num, unsigned short angle_first, unsigned short angle_last, unsigned short number_of_spots, unsigned char immunity);
void get_parameter(unsigned char port_num);
void set_parameter(unsigned char port_num,unsigned char mode);
void set_led_fucntion(unsigned char port_num, unsigned char led,
		unsigned char option1, unsigned char option2, unsigned char frequency);
void read_config_data(unsigned char *buff, unsigned char port_num);
unsigned char read_config_CTN(unsigned char port_num);
unsigned char read_config_CAN(unsigned char port_num);
unsigned char read_config_FACET(unsigned char port_num);
void set_area(unsigned char port_num, char left_or_right, double width, double height);
#endif
#endif
