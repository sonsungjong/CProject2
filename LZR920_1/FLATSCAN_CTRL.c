#include "main.h"
#ifdef FLATSCAN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "usart.h"
#include "FLATSCAN.h"

CONFIG_DATA config_data[U951_CNT];
Node *recieved_buffer[U951_CNT] = {NULL, }; // For RS485 interrupt handler
Node *read_buffer[U951_CNT] = {NULL, };       // For data tracking
unsigned short* baseline_plane[U951_CNT];

#define BEA_POLYNOM 0x90d9

unsigned short CRC16_function(unsigned char *buf_,unsigned short cnt_)
{
	unsigned short crc = 0;
	unsigned short i, j;

	for(i = 0; i < cnt_; i++)
	{
		crc ^= (unsigned short)(buf_[i] << 8);

		for (j = 0; j < 8; j++)
		{
			if ((crc & 0x8000) != 0)
			{
				crc = (unsigned short)((crc << 1) ^ BEA_POLYNOM);
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return crc;
}

unsigned short GET_SHORT_DATA(unsigned char *buff, unsigned int index)
{
	unsigned short result;
	result = (buff[(index<<1)+1]<<8) | buff[(index<<1)];
	return result;
}

void SET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index, unsigned short data)
{
	baseline_plane[port_num][index] = data;
}

unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index)
{
	return baseline_plane[port_num][index];
}


//void ALLOC_FLATSCAN_MEMORY(void)
int ALLOC_MEMORY(void)
{
	int i, j;
	Node *lzr_buffer[U951_CNT];
	printf("FLATSCAN MEMORY ALLOC START\n\r");

	for(i = 0; i < U951_CNT ; i++)
	{
		//LZR_Init(i);
		lzr_buffer[i] 		= Make_buffer(PACKET_BUFFER_COUNT);
		read_buffer[i]		= lzr_buffer[i];       // For data tracking
		recieved_buffer[i] 	= lzr_buffer[i]; // For RS485 interrupt handler
		baseline_plane[i] = (unsigned short*)malloc(sizeof(unsigned short)*MAX_POINT);
	}

	for(i = 0; i < U951_CNT; i++)
	{
		for(j = 0; j < PACKET_BUFFER_COUNT; j++)
		{
			if(read_buffer[i] == NULL)
			{
				printf("MEMORY ALLCO FAILED !!\r\n");
				return 0;
			}
			read_buffer[i] = read_buffer[i]->next;
		}
	}

	for(j = 0; j < U951_CNT; j++)
	{
		for(i = 0; i<MAX_POINT; i++)
		{
			baseline_plane[j][i] = 0xffff;
			//baseline_plane[j][i] = 1500;
		}
		//printf("FLATSCAN MEMORY ALLOC DONE\n\r");
		printf("FLATSCAN MEMORY ALLOC OK !!\n\r");
	}
	return 1;
}

Node* CreateNode(void)
{
	Node *newnode = NULL;
	while(1)
	{
		newnode = (Node *)malloc(sizeof(Node));
		if(newnode != NULL)
		{
			newnode->sync = SYNC_0;
			newnode->length = 0;
			newnode->ready = RESET;
			newnode->cmd = 0;
			newnode->next = NULL;
			break;
		}
	}
	return newnode;
}
Node* Make_buffer(unsigned char buffer_cnt)
{
	unsigned int i = buffer_cnt-1;
	Node *start = NULL;
	Node *current = NULL;
	Node *newnode = NULL;
	start = CreateNode();

	current = start;
	printf("Memory addr : %x\r\n", (unsigned int )current);

	if(buffer_cnt <= 1)
	{
		i = 2;
		printf("Too much few buffer : counter = %d\r\n", buffer_cnt);
	}

	/*
	   if(buffer_cnt == 1)
	   {
	   printf("Only one buffer\n\r");
	   start->next = start;
	   return start;
	   }
	   */

	while(i--)
	{
		newnode = CreateNode();
		printf("Memory addr : %x\r\n", (unsigned int )newnode);
		current->next = newnode;
		current = current->next;
	}
	current->next = start;

	return start;
}

static unsigned int f_index[U951_CNT] = {0,};
static unsigned short crc_result[U951_CNT] = {0};

static unsigned short crc_calculation(unsigned short crc, unsigned char uch)
{
	unsigned char j;
	crc ^= (unsigned short)(uch << 8);
	for(j=0;j<8;j++)
	{
		if((crc & 0x8000) != 0)
			crc = (unsigned short)((crc << 1) ^ BEA_POLYNOM);
		else
			crc <<=1;
	}
	return crc;
}

unsigned char buff_changer(unsigned char port_num, unsigned char uch)
{
	if(recieved_buffer[port_num] == NULL)
		return 0;
	//switch(lzr_info[port_num].sync)
	switch(recieved_buffer[port_num]->sync)
	{
		case SYNC_0 :	f_index[port_num] = 0;
						crc_result[port_num] = 0;
						if(uch == SYNC_0) //0xbe
						{
							recieved_buffer[port_num]->sync = SYNC_1;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_1 : 	if(uch == SYNC_1) //0xa0
						{
							recieved_buffer[port_num]->sync = SYNC_2;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_2 : 	if(uch == SYNC_2) //0x12
						{
							recieved_buffer[port_num]->sync = SYNC_3;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_3 : 	if(uch == SYNC_3) //0x34
						{
							recieved_buffer[port_num]->sync = SYNC_4;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_4 : 	if(uch == 0x02) // Version of the protocol
						{
							recieved_buffer[port_num]->sync = SYNC_5;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
						// Total size of the frame in bytes, low data
		case SYNC_5 : 	recieved_buffer[port_num]->length = uch;
						recieved_buffer[port_num]->sync = SYNC_6;
						recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
						// Total size of the frame in bytes, high data
		case SYNC_6 :	recieved_buffer[port_num]->length = \
						((uch<<8) | recieved_buffer[port_num]->length);
						//Only data size, Except the SYNC, CMD, CHK count
						recieved_buffer[port_num]->sync = SYNC_7;
						recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
		case SYNC_7 : 	if(uch == 0x02) // Verification method
						{
							recieved_buffer[port_num]->sync = SYNC_8;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_8 :	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = SYNC_9;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_9 :	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = SYNC_10;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_10:	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = CMD_L;
							recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case CMD_L	: 	recieved_buffer[port_num]->sync = CMD_H;
						recieved_buffer[port_num]->cmd = uch;
						recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
		case CMD_H	: 	// 0xC3
						recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);

						if((recieved_buffer[port_num]->length-CHK_SIZE) == 0)
							recieved_buffer[port_num]->sync = CHK_0;
						else
							recieved_buffer[port_num]->sync = DATA;
						break;

		case DATA :     recieved_buffer[port_num]->recieved_rawdata[f_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);

						//Test
						recieved_buffer[port_num]->f_index = f_index[port_num];
						//Test

						if((f_index[port_num]+CHK_SIZE) >= recieved_buffer[port_num]->length)
						{
							recieved_buffer[port_num]->sync = CHK_0;
						}

						break;
		case CHK_0 :
						recieved_buffer[port_num]->check_sum = uch;
						recieved_buffer[port_num]->sync = CHK_1;
						break;
		case CHK_1 :
						recieved_buffer[port_num]->check_sum = (uch<<8) | recieved_buffer[port_num]->check_sum;
						if(crc_result[port_num] == recieved_buffer[port_num]->check_sum)
						{
							//Set the currently buffer
							recieved_buffer[port_num]->ready = SET;
							//move to next buffer
							recieved_buffer[port_num] = recieved_buffer[port_num]->next;
							//clear the ready status & sync status
							recieved_buffer[port_num]->ready = RESET;
						}
						recieved_buffer[port_num]->sync = SYNC_0;
						break;
		default : break;
	}

	return 1;
}

void set_area(unsigned char port_num, char left_or_right, double width, double height)
{
	double hypotenuse;
	double py = 3.141592;
	int idx_1;
	double result;
	double degree = (config_data[port_num].angle_last-config_data[port_num].angle_first)/100;

	//	printf("degree[%f] degree/spots[%f]\r\n",degree,degree/config_data[port_num].number_of_spots);
	//unsigned short result;

	hypotenuse = sqrt(pow(width,2) + pow(height,2));
	printf("hypotenuse value is : %f\n", hypotenuse);

	idx_1 = 0;
	while(idx_1 < config_data[port_num].number_of_spots)
	{
		result = width / cos(degree/config_data[port_num].number_of_spots*idx_1*py/180.0);
		//		printf("degree %f    ",cos(degree/config_data[port_num].number_of_spots*idx_1*py/180.0));
		//		printf("degree/config_data[port_num].number_of_spots*idx_1*py/180.0 = %f\r\n", degree/config_data[port_num].number_of_spots*idx_1*py/180.0);
		//		printf("%d:%3f\r\n", idx_1, result);
		if(result > hypotenuse)
			break;
		baseline_plane[port_num][(int)idx_1] = result;
		idx_1++;
	}
	while(idx_1 < config_data[port_num].number_of_spots)
	{
		result = height / sin(degree/config_data[port_num].number_of_spots*idx_1*py/180.0);
		//		printf("%d:%3f\r\n", idx_1, result);
		baseline_plane[port_num][(int)idx_1] = result;
		idx_1++;
	}

}

void teach_in(unsigned char port_num , unsigned int count, unsigned short off_set)
{
	int i,j;
	unsigned char *buff;
	unsigned short distance;

	unsigned int address_offset = 0;

	printf("<<<<<<<<<<<<<<<<<<<<<<<<LIMIT CHECK   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\r");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<TEACH-IN START>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\r");

	for(i = 0; i < MAX_POINT; i++)
	{
		baseline_plane[port_num][i] = 0xffff;
	}

	i = 0;
	j = 0;

	while(j < count)
	{
		if(read_buffer_ready(port_num))
		{
			if(read_buffer_cmd(port_num) == MDI)
			{
				address_offset = 0;

				if(read_config_CAN(port_num))
				{
					address_offset = address_offset+CAN_CNTR_SIZE;
				}

				if(read_config_CTN(port_num))
				{
					address_offset = address_offset+CTN_SIZE;
				}

				if(read_config_FACET(port_num))
				{
					address_offset = address_offset+FACET_SIZE;
				}

				buff = rawdata_copy(port_num, SYNC_CMD_SIZE+address_offset);

				//Jump to begin data pointer, sync,cmd,can, cntr,ctn,facet = 22
				//buff = rawdata_copy(port_num, 22);
				//Count only data, sync,cmd,can, cntr,ctn,facet,chk = 24
				//for(i = 0; i < (read_buffer[port_num]->length-24); i = i+2)

				for(i = 0; i < MAX_POINT; i++)
				{
					distance = GET_SHORT_DATA(buff, i);
					if(distance < baseline_plane[port_num][i])
					{
						baseline_plane[port_num][i] = distance;
					}
				}
				j++;
			}
			//move_next_read_buffer(port_num);
		}
		move_next_read_buffer(port_num);
	}

	for(i = 0; i < MAX_POINT; i++)
	{
		if(baseline_plane[port_num][i] > off_set)
			baseline_plane[port_num][i] =  baseline_plane[port_num][i] - off_set;
		printf("%5d ",baseline_plane[port_num][i]);
		//if((i%20) == 19)
		if((i%10) == 9)
			printf("\n\r");
	}

	printf("\n\r");

	printf("<<<<<<<<<<<<<<<<<<<<<<<<TEACH-IN END    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\r");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<LIMIT CHECK DONE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\r");
}

unsigned char read_buffer_ready(unsigned char port_num)
{
	if(read_buffer[port_num]->ready == SET)
	{
		read_buffer[port_num]->ready = RESET;
		return 1;
	}
	else
	{
		return 0;
	}
}
void move_next_read_buffer(unsigned char port_num)
{
	read_buffer[port_num] = read_buffer[port_num]->next;
}
unsigned char read_buffer_cmd(unsigned char port_num)
{
	return read_buffer[port_num]->cmd;
}
unsigned short read_buffer_length(unsigned char port_num)
{
	return read_buffer[port_num]->length;
}
unsigned char *rawdata_copy(unsigned char port_num, unsigned int index)
{
	return (read_buffer[port_num]->recieved_rawdata) + index;
}

double read_config_angle_first(unsigned char port_num)
{
	double result;
	result = (double)config_data[port_num].angle_first/100.0;
	return result;
}

double read_config_angle_last(unsigned char port_num)
{
	double result;
	result = (double)config_data[port_num].angle_last/100.0;
	return result;
}

void set_baudrate(unsigned char port_num, unsigned char baud_rate)
{

	unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE+1;
	unsigned short crc16;
	packet = (unsigned char *)malloc(sizeof(unsigned char) * length);

	packet[0] = SYNC_0;
	packet[1] = SYNC_1;
	packet[2] = SYNC_2;
	packet[3] = SYNC_3;
	packet[4] = 0x02;
	packet[5] = length & 0xff; //low
	packet[6] = (length>>8) & 0xff; //high
	packet[7] = 0x02;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = SET_BAUDRATE;
	packet[12] = CMD_H;
	packet[13] = baud_rate;
	crc16 = CRC16_function(packet, length-CHK_SIZE);
	packet[14] = crc16 & 0xff;
	packet[15] = (crc16 >> 8) & 0xff;
	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(10); //We need to delay, if no delay, data will break.
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\n\r");
	free(packet);
}

void get_measurements(unsigned char port_num, unsigned char mode)
{
	unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE+1;
	unsigned short crc16;
	packet = (unsigned char *)malloc(sizeof(unsigned char) * length);

	packet[0] = SYNC_0;
	packet[1] = SYNC_1;
	packet[2] = SYNC_2;
	packet[3] = SYNC_3;
	packet[4] = 0x02;
	packet[5] = length & 0xff; //low
	packet[6] = (length>>8) & 0xff; //high
	packet[7] = 0x02;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = GET_MEASUREMENTS;
	packet[12] = CMD_H;
	packet[13] = mode;
	crc16 = CRC16_function(packet, length-CHK_SIZE);
	packet[14] = crc16 & 0xff;
	packet[15] = (crc16 >> 8) & 0xff;

	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(10);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\n\r");
	free(packet);
}

void get_parameter(unsigned char port_num)
{
	unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE;
	unsigned short crc16;
	packet = (unsigned char *)malloc(sizeof(unsigned char) * length);

	packet[0] = SYNC_0;
	packet[1] = SYNC_1;
	packet[2] = SYNC_2;
	packet[3] = SYNC_3;
	packet[4] = 0x02;
	packet[5] = length & 0xff; //low
	packet[6] = (length>>8) & 0xff; //high
	packet[7] = 0x02;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = GET_PARAMETERS;
	packet[12] = CMD_H;
	crc16 = CRC16_function(packet, length-CHK_SIZE);
	packet[13] = crc16 & 0xff;
	packet[14] = (crc16 >> 8) & 0xff;

	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(10);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\n\r");
	free(packet);
}

void set_default_config_data(
		int port_num, 
		unsigned short angle_first, 
		unsigned short angle_last, 
		unsigned short number_of_spots,
		unsigned char immunity
		)
{
	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	printf("Angle, SPOT NUMBERS SETTING START\r\n");
	config_data[port_num].angle_first = angle_first;
	config_data[port_num].angle_last = angle_last;
	config_data[port_num].number_of_spots = number_of_spots;
	config_data[port_num].immunity_level 		= immunity;

	printf("angle first             : %d\r\n",config_data[port_num].angle_first);
	printf("angle last              : %d\r\n",config_data[port_num].angle_last);
	printf("angle number_of_spots   : %d\r\n",config_data[port_num].number_of_spots);
	printf("Angle, SPOT NUMBERS SETTING END\r\n");
	printf("--------------------------------------------\r\n");
}

void set_parameter(unsigned char port_num,unsigned char mode)
{
	unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE+22;
	unsigned short crc16;
	packet = (unsigned char *)malloc(sizeof(unsigned char) * length);

	//SYCN begin
	packet[0] = SYNC_0;
	packet[1] = SYNC_1;
	packet[2] = SYNC_2;
	packet[3] = SYNC_3;
	packet[4] = 0x02;
	packet[5] = length & 0xff; //low
	packet[6] = (length>>8) & 0xff; //high
	packet[7] = 0x02;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	//SYNC end

	//CMD begin
	packet[11] = SET_PARAMETERS;
	packet[12] = CMD_H;
	//CMd end

	//DATA begin
	packet[13] = 0x00;              //Reserved for future use, write 0.
	packet[14] = 0x00;				//CTN(temperature) FIELD Diable
	packet[15] = 0x00;				//ONLY MDI

	packet[16] = mode;              //detection field mode
	packet[17] = config_data[port_num].immunity_level;
	//packet[17] = IMMINITY_1;              //immunity
	//packet[17] = IMMINITY_4;              //immunity

	packet[18] = 0x00;              //reserved
	packet[19] = 0x00;				//reserved
	packet[20] = 0x00;				//reserved

	packet[21] = config_data[port_num].number_of_spots & 0xff;		//spot number low
	packet[22] = (config_data[port_num].number_of_spots>>8) & 0xff;	//spot number high

	packet[23] = 0x00;              //reserved
	packet[24] = 0x00;				//reserved
	packet[25] = 0x00;				//reserved
	packet[26] = 0x00;				//reserved


	packet[27] = config_data[port_num].angle_first & 0xff;			//angle first degree low
	packet[28] = (config_data[port_num].angle_first>>8) & 0xff;		//angle first degree high
	packet[29] = config_data[port_num].angle_last & 0xff;			//angle last degree low
	packet[30] = (config_data[port_num].angle_last>>8) & 0xff;		//angle last degree high

	packet[31] = 0x00;												//CAN,FRAME COUNT, HEARTBEAT, EMERGENCY FIELD ENABLE
	packet[32] = 0x00;												//heartbeat period
	packet[33] = 0x00;												//facet number field enable
	packet[34] = 0x00;												//no averaging.
	//DATA end


	crc16 = CRC16_function(packet, length-CHK_SIZE);
	packet[35] = crc16 & 0xff;
	packet[36] = (crc16 >> 8) & 0xff;


	for(int i = 0; i < length;i++)
	{
		//printf("%02x", packet[i]);
		delay(10);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\r\n");
	free(packet);

}

void set_led_fucntion(unsigned char port_num, unsigned char led,
		unsigned char option1, unsigned char option2, unsigned char frequency)
{
	unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE+4;
	unsigned short crc16;
	packet = (unsigned char *)malloc(sizeof(unsigned char) * length);

	//SYCN begin
	packet[0] = SYNC_0;
	packet[1] = SYNC_1;
	packet[2] = SYNC_2;
	packet[3] = SYNC_3;
	packet[4] = 0x02;
	packet[5] = length & 0xff; //low
	packet[6] = (length>>8) & 0xff; //high
	packet[7] = 0x02;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	//SYNC end
	//CMD begin
	packet[11] = SET_LED;
	packet[12] = CMD_H;
	//CMd end
	//DATA begin
	packet[13] = led;
	packet[14] = option1;
	packet[15] = option2;
	packet[16] = frequency;
	//DATA end

	crc16 = CRC16_function(packet, length-CHK_SIZE);
	packet[17] = crc16 & 0xff;
	packet[18] = (crc16 >> 8) & 0xff;

	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(10);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\n\r");
	free(packet);
}


unsigned short read_packet_length(unsigned char port_num)
{
	return recieved_buffer[port_num]->length;
}

unsigned char read_config_CTN(unsigned char port_num)
{
	return config_data[port_num].ctn_field_status;
}

unsigned char read_config_CAN(unsigned char port_num)
{
	return config_data[port_num].can_n_frame_fields;
}

unsigned char read_config_FACET(unsigned char port_num)
{
	return config_data[port_num].facet_number_field;
}


//CONFIG_DATA config_data[U951_CNT];
void read_config_data(unsigned char *buff, unsigned char port_num)
{
	config_data[port_num].bit_mask_d0 = buff[0];
	config_data[port_num].bit_mask_d1 = buff[1];
	config_data[port_num].bit_mask_d2 = buff[2];
	config_data[port_num].bit_mask_d3 = buff[3];

	config_data[port_num].communication_charge 	= (buff[5]<<8) | buff[4];
	config_data[port_num].ctn_field_status 		= buff[7];
	config_data[port_num].info_in_mdi 			= buff[8];
	config_data[port_num].detection_field_mode 	= buff[9];
	config_data[port_num].immunity_level 		= buff[10];
	config_data[port_num].number_of_spots 		= (buff[15]<<8)|buff[14];
	config_data[port_num].angle_first 			= (buff[21]<<8)|buff[20];
	config_data[port_num].angle_last 			= (buff[23]<<8)|buff[22];
	config_data[port_num].can_n_frame_fields 	= buff[24];
	config_data[port_num].heartbeat_period 		= buff[25];
	config_data[port_num].facet_number_field 	= buff[26];
	config_data[port_num].averaging_setting 	= buff[27];


	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	if( (config_data[port_num].bit_mask_d0 & 0xff) ||
			(config_data[port_num].bit_mask_d1 & 0xff) ||
			(config_data[port_num].bit_mask_d2 & 0xff) ||
			(config_data[port_num].bit_mask_d3 & 0xff)
	  )
	{
		printf("SET parameter error\n\r");
	}

	printf("communication_charge %d\n\r", config_data[port_num].communication_charge);
	printf("ctn_field_status     %d\n\r", config_data[port_num].ctn_field_status 	  );
	printf("info_in_mdi          %d\n\r", config_data[port_num].info_in_mdi 		  );
	printf("detection_field_mode %d\n\r", config_data[port_num].detection_field_mode);
	printf("immunity_level       %d\n\r", config_data[port_num].immunity_level 	  );
	printf("number_of_spots      %d\n\r", config_data[port_num].number_of_spots 	  );
	printf("angle_first          %d\n\r", config_data[port_num].angle_first 		  );
	printf("angle_last           %d\n\r", config_data[port_num].angle_last 		  );
	printf("can_n_frame_fields   %d\n\r", config_data[port_num].can_n_frame_fields  );
	printf("heartbeat_period     %d\n\r", config_data[port_num].heartbeat_period 	  );
	printf("facet_number_field   %d\n\r", config_data[port_num].facet_number_field  );
	printf("averaging_setting    %d\n\r", config_data[port_num].averaging_setting   );
	printf("--------------------------------------------\r\n");
}

#endif
