#ifndef __MICROSCAN_H
#define __MICROSCAN_H

#include "main.h"

#ifdef MICROSCAN

#define FULL_DUPLEX

#define uSCAN_1						0
#define uSCAN_2						1

#define uSCAN_CNT					2
#define PACKET_BUFFER_COUNT			3

#define MAX_POINT					100

#define ENABLED						0x01
#define DISABLED					0x00

//#define IMMINITY_1					1
//#define IMMINITY_2					2
//#define IMMINITY_3					3
//#define IMMINITY_4					4

#define FACET1						0
#define FACET2						1
#define FACET3						2
#define FACET4						3

#define SYNC_SIZE					11
#define CMD_SIZE 					2
#define CAN_SIZE 					8
#define COUNT_ER_SIZE				2
#define CTN_SIZE					2
#define FACET_SIZE					1
#define FACET1_SIZE					200
#define FACET2_SIZE					200
#define FACET3_SIZE					200
#define FACET4_SIZE					200
#define CHK_SIZE 					2
#define PARAMETER_SIZE				43

#define BAUD_RATE_921600			4

#define SINGLE_SHOT_MODE			0x00
#define CONTINUOUS_MODE				0x01

#define SYNC_0						0xbe
#define SYNC_1		        		0xa0
#define SYNC_2		        		0x12
#define SYNC_3		        		0x34
#define SYNC_4		        		0x02
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
#define STORE_PARAMETERS			0x55
#define GET_IDENTITY				0x5a
#define GET_MEASUREMENTS			0x5b
//#define RESET_MDI_COUNTER			0x5e
//#define RESET_HEARTBEAT_COUNTER	0x5f
//#define GET_EMERGENCY				0x6e
//#define SET_LED					0x78
#define SEND_PARAMETERS				0x54
#define SEND_IDENTITY				0x5a
#define MDI							0x5b

#define LZR_RECIEVE_PACKET_TOTAL 	1100

typedef struct{
	//IDENTITY
	unsigned int product_part_number;
	unsigned char sw_version;
	unsigned char sw_revision;
	unsigned char sw_prototype;
	unsigned int can_number_of_the_detector;
	unsigned int can_number_of_the_detector2;

	//parameters
	unsigned char ctn_field_in_measurement;
	unsigned char measurement_information;

	unsigned short angle_first_facet[4];
	unsigned short angle_last_facet[4];
	unsigned short spot_number_facet[4];

	unsigned char frame_counter_enable;
	unsigned char can_enable;
	unsigned char heartbeat_period;
	unsigned char facet_Num_enable;
	unsigned char baudrate;

}CONFIG_DATA;


typedef struct Node
{
	char ready;
	unsigned short check_sum;
	unsigned short length;
	unsigned char sync;
	unsigned char cmd;
	unsigned char recieved_rawdata[LZR_RECIEVE_PACKET_TOTAL];
	struct Node *next;
}Node;

typedef struct _BACKGROUND_FACET
{
	unsigned short *facet1;
	unsigned short *facet2;
	unsigned short *facet3;
	unsigned short *facet4;
}BACKGROUND_FACET;

extern CONFIG_DATA config_data[uSCAN_CNT];
extern Node *recieved_buffer[uSCAN_CNT];
extern Node *read_buffer[uSCAN_CNT];
extern BACKGROUND_FACET back_ground[uSCAN_CNT];
//extern unsigned short *baseline_facet[uSCAN_CNT];

Node* Make_buffer(unsigned char buffer_cnt);
Node* CreateNode(void);
unsigned char buff_changer(unsigned char port_num, unsigned char uch);
unsigned short GET_SHORT_DATA(unsigned char *buff, unsigned int index);

int ALLOC_MEMORY(void);
void CONFIG_LZR(unsigned char port_num);
unsigned short CRC16(unsigned char *buf_,unsigned short cnt_);
unsigned char read_buffer_ready(unsigned char port_num);
void move_next_read_buffer(unsigned char port_num);
unsigned char read_buffer_cmd(unsigned char port_num);
unsigned short read_buffer_length(unsigned char port_num);
unsigned char *rawdata_copy(unsigned char port_num, int jump_address);
double read_config_angle_first(unsigned char port_num, int facet_num);
double read_config_angle_last(unsigned char port_num, int facet_num);
void input_config_data(unsigned char *buff, unsigned char port_num);
void input_identity_data(unsigned char *buff, unsigned char port_num);

void set_parameter(unsigned char port_num);
void send_command(unsigned char port_num, unsigned char cmd);
void get_measurements(unsigned char port_num, unsigned char mode);

//unsigned char *analyze_message(unsigned char port_num, int *data_size);
unsigned char *analyze_message(unsigned char port_num, int *data_size, unsigned char *cmd);
void teach_in(unsigned char port_num , unsigned int count, unsigned short off_set);

void input_uSCAN_parameter(
		unsigned char port_num,
		unsigned short facet1_angle_first, unsigned short facet1_angle_last, unsigned short facet1_spot_numner,
		unsigned short facet2_angle_first, unsigned short facet2_angle_last, unsigned short facet2_spot_numner,
		unsigned short facet3_angle_first, unsigned short facet3_angle_last, unsigned short facet3_spot_numner,
		unsigned short facet4_angle_first, unsigned short facet4_angle_last, unsigned short facet4_spot_numner,
		char ctn_enable,
		char mri_enable,
		char frame_counter_enable,
		char can_enable,
		char heartbeat_period,
		char baudrate
		);

unsigned char* assigned_buffer(unsigned char port_num, unsigned char *measured_data, int facet_num);

void SET_BACKGROUND_DATA(unsigned char port_num, int facet_num, int index, unsigned short data);

unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int facet_num, int index);

int get_ctn_status(unsigned char port_num);
int get_can_status(unsigned char port_num);
int get_counter_status(unsigned char port_num);
unsigned short Get_spot_number_facet(unsigned char port_num, int facet_num);

#endif
#endif
