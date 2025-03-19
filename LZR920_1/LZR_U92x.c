#include <main.h>

#if defined(LZR_U92x)


#include "LZR_U92x.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "usart.h"
#include "gpio.h"
#include "cmsis_os.h"
#include <stdio.h>


static Node *rx_buffer[U92x_CNT] = {NULL,};
static Node *read_buffer[U92x_CNT] = {NULL,};

static unsigned char CONFIG_DATA_BUFFER[U92x_CNT][39] ={0, };
static unsigned char INFO_DATA_BUFFER[U92x_CNT][19] = {0, };
static unsigned char rx_busy[U92x_CNT] = {0, };

static unsigned char SET_BACKGROUND_FLAG[U92x_CNT] = {0, };
static unsigned char SET_CONFIG_FLAG[U92x_CNT] = {0, };

#if defined(LZR_U920)
static unsigned short *BACKGROUND_DATA[U92x_CNT][4];
#else
static unsigned short *BACKGROUND_DATA[U92x_CNT][1];
#endif

static unsigned short enabled_plane_num[U92x_CNT] = {0, };
static unsigned short enabled_frame_counter[U92x_CNT] = {0, };
static unsigned short enabled_lzr_information[U92x_CNT] = {0, };


void SET_PLANE_NUM_ENABLED(unsigned char port_num, unsigned short value)
{
	enabled_plane_num[port_num] = value;
}

void SET_FRAME_COUNTER_ENABLED(unsigned char port_num, unsigned short value)
{
	enabled_frame_counter[port_num] = value;
}

void SET_INFORMATION_ENABLED(unsigned char port_num, unsigned short value)
{
	enabled_lzr_information[port_num] = value;
}


unsigned short * GET_BACKGROUND_DATA_ARRAY(unsigned char port_num, int plane_num)
{
	return BACKGROUND_DATA[port_num][plane_num];
}


int ALLOC_MEMORY(void)
{
	//	int i;
#if	U92x_CNT >= 1

	//Main recieve buffer part
	Node *tmp1 = malloc(sizeof(Node));
	Node *tmp2 = malloc(sizeof(Node));
	Node *tmp3 = malloc(sizeof(Node));

	tmp1->sync = SYNC_0;
	tmp2->sync = SYNC_0;
	tmp3->sync = SYNC_0;

	tmp1->next = tmp2;
	tmp2->next = tmp3;
	tmp3->next = tmp1;

	rx_buffer[0] = tmp1;
	read_buffer[0] = rx_buffer[0];

#if defined(LZR_U920)
	BACKGROUND_DATA[0][0] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[0][1] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[0][2] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[0][3] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
#else                                                                                     
	BACKGROUND_DATA[0][0] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
#endif

#if U92x_CNT >= 2
	Node *tmp4 = malloc(sizeof(Node));
	Node *tmp5 = malloc(sizeof(Node));
	Node *tmp6 = malloc(sizeof(Node));

	tmp4->sync = SYNC_0;
	tmp5->sync = SYNC_0;
	tmp6->sync = SYNC_0;

	tmp4->next = tmp5;
	tmp5->next = tmp6;
	tmp6->next = tmp4;

	rx_buffer[1] = tmp4;
	read_buffer[1] = rx_buffer[1];

#if defined(LZR_U920)
	BACKGROUND_DATA[1][0] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[1][1] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[1][2] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
	BACKGROUND_DATA[1][3] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
#else                                                                                     
	BACKGROUND_DATA[1][0] = (unsigned short *)malloc( U92x_DATA_CNT* sizeof(unsigned short));
#endif

#endif
	return 1;

#endif
	return 0;
}

void SET_BACKGROUND_DATA_FLAG(unsigned char port_num)
{
	SET_BACKGROUND_FLAG[port_num] = 1;
}

void SET_SENSOR_CONFIG_FLAG(unsigned char port_num)
{
	SET_CONFIG_FLAG[port_num] = 1;
}

unsigned char CHECK_SENSOR_CONFIG_FLAG(unsigned char port_num)
{
	if(SET_CONFIG_FLAG[port_num] == 1)
	{
		SET_CONFIG_FLAG[port_num] = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void SET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index, unsigned short data)
{
#if defined(LZR_U920)
	BACKGROUND_DATA[port_num][plane_num][index] = data;
#else
	BACKGROUND_DATA[port_num][0][index] = data;
#endif
}

unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int plane_num, int index)
{
#if defined(LZR_U920)
	return BACKGROUND_DATA[port_num][plane_num][index];
#else
	return BACKGROUND_DATA[port_num][0][index];
#endif
}

void PRINT_BACKGROUND(unsigned char port_num)
{
	int i;
#if defined(LZR_U920)
	int j;
#endif
	printf("\r\n");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<LIMIT CHECK   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<TEACH-IN START>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");

#if defined(LZR_U920)
	for(j = 0; j < 4; j += 1)
	{
		for(i = 0; i < U92x_DATA_CNT; i++)
		{
			printf("%5d ", GET_BACKGROUND_DATA(port_num, j, i));
			if( (i % 10) == 9 )
			{
				printf("\r\n");
			}
		}
		printf("\r\n\r\n");
	}
#else
	for(i = 0; i < U92x_DATA_CNT; i++)
	{
		printf("%5d ", GET_BACKGROUND_DATA(port_num, 0, i));
		if( (i % 10) == 9 )
		{
			printf("\r\n");
		}
	}
#endif
	printf("\r\n");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<TEACH-IN END    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<LIMIT CHECK DONE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");

}

unsigned char TEACH_IN(unsigned char port_num, unsigned int loop_cnt, unsigned short off_set)
{
	unsigned char cmd;
	unsigned char *raw_data;
	int i;

#if defined(LZR_U920)
	unsigned char plane_num;
	unsigned char *plane_data[4] = {NULL, };
#else
	unsigned char *plane_data[1] = {NULL, };
#endif


	if(SET_BACKGROUND_FLAG[port_num] == 0)
	{
		return 0;
	}

	SET_BACKGROUND_FLAG[port_num] = 0;
#if defined(LZR_U920)
	for(i =0 ; i < U92x_DATA_CNT; i += 1)
	{
		BACKGROUND_DATA[port_num][0][i] = 0xffff;
		BACKGROUND_DATA[port_num][1][i] = 0xffff;
		BACKGROUND_DATA[port_num][2][i] = 0xffff;
		BACKGROUND_DATA[port_num][3][i] = 0xffff;
	}
#else
	for(i =0 ; i < U92x_DATA_CNT; i += 1)
	{
		BACKGROUND_DATA[port_num][0][i] = 0xffff;
	}
#endif

	while(loop_cnt != 0)
	{

		if(GET_RX_BUFFER_READY(port_num))
		{
			cmd = GET_RX_BUFFER_CMD(port_num);
			raw_data = GET_RX_BUFFER(port_num, 0);

			if(cmd == GETRAWDATADISTANCEVALUES)
			{
				if(enabled_frame_counter[port_num])
				{
					raw_data = raw_data + ID_FRAME_SIZE;
				}

				if(enabled_lzr_information[port_num])
				{
					raw_data = raw_data + INFORMATION_SIZE;
				}

				if(enabled_plane_num[port_num])
				{
#if defined(LZR_U920)
					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
#else
					plane_data[PLANE_0] = raw_data + PLANE_NUM_SIZE;
#endif
				}
				else
				{
#if defined(LZR_U920)
					plane_data[1] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[3] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[0] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[2] = raw_data;
#else
					plane_data[PLANE_0] = raw_data;
#endif
				}



#if defined(LZR_U920)
				for(i = 0; i < U92x_DATA_CNT; i += 1)
				{
					if( GET_SHORT_DATA(plane_data[0], i) < BACKGROUND_DATA[port_num][0][i] )
					{
						BACKGROUND_DATA[port_num][0][i] = GET_SHORT_DATA(plane_data[0], i) - off_set;
					}

					if( GET_SHORT_DATA(plane_data[1], i) < BACKGROUND_DATA[port_num][1][i] )
					{
						BACKGROUND_DATA[port_num][1][i] = GET_SHORT_DATA(plane_data[1], i) - off_set;
					}

					if( GET_SHORT_DATA(plane_data[2], i) < BACKGROUND_DATA[port_num][2][i] )
					{
						BACKGROUND_DATA[port_num][2][i] = GET_SHORT_DATA(plane_data[2], i) - off_set;
					}

					if( GET_SHORT_DATA(plane_data[3], i) < BACKGROUND_DATA[port_num][3][i] )
					{
						BACKGROUND_DATA[port_num][3][i] = GET_SHORT_DATA(plane_data[3], i) - off_set;
					}
				}
#else	
				for(i = 0; i < U92x_DATA_CNT; i += 1)
				{
					if( GET_SHORT_DATA(plane_data[0], i) < BACKGROUND_DATA[port_num][0][i] )
					{
						BACKGROUND_DATA[port_num][0][i] = GET_SHORT_DATA(plane_data[0], i) - off_set;
					}
				}
#endif
				loop_cnt -= 1;
			}//End of if(cmd == GETRAWDATADISTANCEVALUES)

		}//End of if(GET_RX_BUFFER_READY(port_num))

		SET_RX_BUFFER_NEXT(port_num);

	}//End of while(loop_cnt != 0)

	return 1;

}//End of function

unsigned char rx_function(unsigned char port_num, unsigned char uch)
{
	if(rx_buffer[port_num] == NULL)
	{
		return 0;
	}


	switch(rx_buffer[port_num]->sync)
	{
		case SYNC_0:
			rx_buffer[port_num]->buffer_index = 0;
			rx_buffer[port_num]->check_sum = 0;
			rx_buffer[port_num]->check_sum_cmp = 0;

			if(uch == SYNC_0)
			{
				rx_busy[port_num] = 1;
				rx_buffer[port_num]->sync = SYNC_1;
			}
			else
			{
				rx_busy[port_num] = 0;
				rx_buffer[port_num]->sync = SYNC_0;
			}
			break;

		case SYNC_1:
			if(uch == SYNC_1)
			{
				rx_buffer[port_num]->sync = SYNC_2;
			}
			else
			{
				rx_busy[port_num] = 0;
				rx_buffer[port_num]->sync = SYNC_0;
			}
			break;

		case SYNC_2:
			if(uch == SYNC_2)
			{
				rx_buffer[port_num]->sync = SYNC_3;
			}
			else
			{
				rx_busy[port_num] = 0;
				rx_buffer[port_num]->sync = SYNC_0;
			}
			break;

		case SYNC_3:
			if(uch == SYNC_3)
			{
				rx_buffer[port_num]->lzr_packet_length = 0;
				rx_buffer[port_num]->check_sum = 0;
				rx_buffer[port_num]->sync = SIZE_L;
			}
			else
			{
				rx_busy[port_num] = 0;
				rx_buffer[port_num]->sync = SYNC_0;
			}
			break;

		case SIZE_L:
			rx_buffer[port_num]->lzr_packet_length = uch;
			rx_buffer[port_num]->sync = SIZE_H;
			break;

		case SIZE_H:
			rx_buffer[port_num]->lzr_packet_length = ((uch<<8) | rx_buffer[port_num]->lzr_packet_length)-2;
			rx_buffer[port_num]->sync = CMD_L;
			break;

		case CMD_L:
			rx_buffer[port_num]->cmd = uch;
			rx_buffer[port_num]->check_sum = rx_buffer[port_num]->check_sum + uch;
			rx_buffer[port_num]->sync = CMD_H;
			break;

		case CMD_H:
			rx_buffer[port_num]->check_sum = rx_buffer[port_num]->check_sum + uch;

			if(rx_buffer[port_num]->cmd == GETRAWDATACONFIG)
			{
				rx_buffer[port_num]->sync = CONFIG_DATA;
			}
			else if(rx_buffer[port_num]->cmd == GETRAWDATAINFORMATION)
			{
				rx_buffer[port_num]->sync = INFO_DATA;
			}
			else
			{
				rx_buffer[port_num]->sync = DIST_DATA;
			}

			break;

		case DIST_DATA:
			rx_buffer[port_num]->check_sum = rx_buffer[port_num]->check_sum + uch;
			rx_buffer[port_num]->lzr_recieved_packet[rx_buffer[port_num]->buffer_index++] = uch;
			if(rx_buffer[port_num]->buffer_index >= rx_buffer[port_num]->lzr_packet_length)
			{
				rx_buffer[port_num]->sync = CHK_0;
			}
			break;

		case CONFIG_DATA:
			rx_buffer[port_num]->check_sum = rx_buffer[port_num]->check_sum + uch;
			CONFIG_DATA_BUFFER[port_num][rx_buffer[port_num]->buffer_index++] = uch;
			if(rx_buffer[port_num]->buffer_index >= rx_buffer[port_num]->lzr_packet_length)
			{
				rx_buffer[port_num]->sync = CHK_0;
			}
			break;

		case INFO_DATA:
			rx_buffer[port_num]->check_sum = rx_buffer[port_num]->check_sum + uch;
			INFO_DATA_BUFFER[port_num][rx_buffer[port_num]->buffer_index++] = uch;
			if(rx_buffer[port_num]->buffer_index >= rx_buffer[port_num]->lzr_packet_length)
			{
				rx_buffer[port_num]->sync = CHK_0;
			}
			break;

		case CHK_0:
			rx_buffer[port_num]->check_sum_cmp = uch;
			rx_buffer[port_num]->sync = CHK_1;
			break;

		case CHK_1:
			rx_buffer[port_num]->check_sum_cmp = (uch<<8) | rx_buffer[port_num]->check_sum_cmp;
			rx_buffer[port_num]->sync = SYNC_0;

			if(rx_buffer[port_num]->check_sum == rx_buffer[port_num]->check_sum_cmp)
			{
				rx_buffer[port_num]->ready = SET;
			}
			else
			{
				rx_buffer[port_num]->ready = RESET;
			}

			//Move to next rx_buffer and clear ready
			rx_buffer[port_num] = rx_buffer[port_num]->next;
			rx_buffer[port_num]->ready = RESET;
			rx_busy[port_num] = 0;
			break;

		default:
			rx_buffer[port_num]->sync = SYNC_0;
			rx_buffer[port_num]->ready = RESET;
			break;

	}//End of switch(rx_buffer[port_num]->sync)
	return 1;
}

unsigned char GET_RX_BUFFER_READY(unsigned char port_num)
{
	if(read_buffer[port_num]->ready == SET)
	{
		read_buffer[port_num]->ready = RESET;
		return 1;
	}
	return 0;
}

unsigned char SET_RX_BUFFER_NEXT(unsigned char port_num)
{
	if(read_buffer[port_num]->next != NULL)
	{
		read_buffer[port_num] = read_buffer[port_num]->next;
		return 1;
	}
	return 0;
}

unsigned short GET_RX_BUFFER_LENGTH(unsigned char port_num)
{
	return read_buffer[port_num]->lzr_packet_length+2;
}

unsigned char GET_RX_BUFFER_CMD(unsigned char port_num)
{
	return read_buffer[port_num]->cmd;
}

unsigned char GET_RX_BUFFER_BUSY(unsigned char port_num)
{
	return rx_busy[port_num];
}

unsigned char SEND_CONFIG_COMMAND(unsigned char port_num)
{
	unsigned char data[43];
	unsigned short check_sum = 0;
	int data_size = 43;


	int i;

	//Sync 4
	data[0] = SYNC_0;
	data[1] = SYNC_1;
	data[2] = SYNC_2;
	data[3] = SYNC_3;

	//Size 2
	data[4] = 35; 	//SIZE_L
	data[5] = 0x00;		//SIZE_H

	//cmd 2
	data[6] = SETRAWDATACONFIG;		//CMD_L
	data[7] = 0xC3;					//CMD_H

	//Copy from config_data to packet buffer
	for(i = 6; i < 39; i++)
	{
		data[i+2] = CONFIG_DATA_BUFFER[port_num][i];
	}

	//Check sum from cmd to end of data
	for(i = 6; i < 41; i++)
	{
		check_sum = check_sum + data[i];
	}

	data[41] = check_sum & 0xff;
	data[42] = (check_sum>>8) & 0xff;
	RS485_Send_delay(port_num, data, data_size, 20);
	return 1;
}


unsigned char SEND_COMMAND(unsigned char port_num, unsigned char cmd, unsigned char option)
{

	unsigned char data[11];
	unsigned short check_sum = 0;
	int data_size = 0;

	data[0] = SYNC_0;
	data[1] = SYNC_1;
	data[2] = SYNC_2;
	data[3] = SYNC_3;
	//data[4] = size; 	//SIZE_L
	data[5] = 0x00;		//SIZE_H

	data[6] = cmd;		//CMD_L
	data[7] = 0xC3;		//CMD_H
	switch(cmd)
	{
		case SET_CONFIG_MODE:
			data[0] = 0xA5;
			data[1] = 0xA5;
			data[2] = 0xA5;
			RS485_Send_delay(port_num, data, 3, 20);
			return 1;

		case GETRAWDATADISTANCEVALUES:
		case SETRAWDATAERRORLOGRESET:
		case SETRAWDATAFRAMECOUNTERRESET:
		case GETRAWDATAMODE:
		case GETRAWDATACONFIG:
		case GETRAWDATAINFORMATION:
		case GETRAWDATAREDLASER:
		case GETRAWDATAHEARTBEAT:
		case SETRAWDATACONFIGSTORE:
		case SETRAWDATACONFIGRESTORE:

			data[4] = 0x02; //SIZE_L
			check_sum = data[6] + data[7];
			data[8] = check_sum & 0xff;
			data[9] = (check_sum>>8) & 0xff;
			data_size = 10;
			break;

		case SETRAWDATAMODE:
		case SETRAWDATAREDLASER:
		case SETRAWDATAHEARTBEAT:

			data[4] = 0x03;//SIZE_L
			data[8] = option;
			check_sum = data[6] + data[7] + data[8];
			data[9] = check_sum & 0xff;
			data[10] = (check_sum>>8) & 0xff;;
			data_size = 11;
			break;

		case SETRAWDATACONFIG:
			printf("Use SEND_CONFIG_COMMAND() function\r\n");
			return 0;

		default:
			printf("Use Wrong Command 0x%x\r\n", cmd);
			return 0;
	}
	RS485_Send_delay(port_num, data, data_size, 50);
	return 1;
}

unsigned char SET_CONFIG_FIELD(unsigned char port_num, int address, unsigned short data)
{
	switch(address)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			printf("You access wrong configuration address\r\n");
			return 0;
		case 16:
		case 18:
		case 20:
		case 32:
		case 36:
			CONFIG_DATA_BUFFER[port_num][address+0] = data & 0xff;
			CONFIG_DATA_BUFFER[port_num][address+1] = (data >> 8) & 0xff;
			return 1;
		default:
			CONFIG_DATA_BUFFER[port_num][address] = data & 0xff;
			return 1;
	}
}

unsigned short GET_CONFIG_FIELD(unsigned char port_num, int address)
{
	unsigned short return_data = 0;

	switch(address)
	{
		case 4:
		case 16:
		case 18:
		case 20:
		case 32:
		case 36:
			return_data = (CONFIG_DATA_BUFFER[port_num][address+1] << 8) | CONFIG_DATA_BUFFER[port_num][address];
			break;
		default:
			return_data = CONFIG_DATA_BUFFER[port_num][address];
			break;
	}

	return return_data;
}

unsigned char *GET_RX_BUFFER(unsigned char port_num, unsigned int index)
{
	return (read_buffer[port_num]->lzr_recieved_packet)+index;
}

unsigned char GET_PLANE_NUM(unsigned char num)
{
	switch(num)
	{
		case 2: return PLANE_0;
		case 0: return PLANE_1;
		case 3: return PLANE_2;
		default: return PLANE_3;
	}
}

void SET_RX_BUFFER_SHORT_DATA(unsigned char *buff, unsigned int index, unsigned short data)
{
	buff[ (index<<1) + 0] = data & 0xff;
	buff[ (index<<1) + 1] = (data>>8) & 0xff;
}

unsigned short GET_SHORT_DATA(unsigned char *buff, unsigned int index)
{
	return (buff[(index<<1)+1]<<8) | buff[(index<<1)];
}

//int GET_BUFFER_INT_DATA(unsigned char *buff, unsigned int index)
int GET_INT_DATA(unsigned char *buff, unsigned int index)
{
	int result;
	result = (buff[index+3]<<24) | (buff[index+2]<<16) | (buff[index+1]<<8) | buff[index+0];
	return result;
}

unsigned char GET_SENSOR_READY(unsigned char port_num)
{	

	unsigned char return_cmd = 0;

	while(1)
	{
		if(GET_RX_BUFFER_READY(port_num))
		{
			return_cmd = GET_RX_BUFFER_CMD(port_num);

			if(return_cmd == GETRAWDATADISTANCEVALUES)
			{
				return 1;
			}
		}
		SET_RX_BUFFER_NEXT(port_num);
	}

	return 0;
}

unsigned char SET_CONFIG_SENSOR(unsigned char port_num, unsigned char cmd, unsigned char option)
{
	const unsigned int CMD_GAP = 500;
	unsigned char return_cmd = 0;
	unsigned char *raw_data;
	unsigned char send_cmd_flag = 0;

	while(1)
	{
		if(send_cmd_flag == 0)
		{
			send_cmd_flag = 1;

			if(cmd == SET_CONFIG_MODE)
			{
				if (GET_RX_BUFFER_BUSY(port_num) == 0)
				{
					SEND_COMMAND(port_num, cmd, option);
				}
			}
			else if(cmd == SETRAWDATACONFIG)
			{
				SEND_CONFIG_COMMAND(port_num);
				delay(CMD_GAP);
			}
			else
			{
				SEND_COMMAND(port_num, cmd, option);
				delay(CMD_GAP);
			}

		}

		if(GET_RX_BUFFER_READY(port_num))
		{
			send_cmd_flag = 0;
			return_cmd = GET_RX_BUFFER_CMD(port_num);
			raw_data = GET_RX_BUFFER(port_num, 0);

			switch(return_cmd)
			{
				case GETRAWDATAMODE:

					if((raw_data[0] == CONFIGURATION_MODE) && (cmd == SET_CONFIG_MODE))
					{
						printf("<< GET CONFIG MODE\r\n\r\n");
						return 1;
					}
					else if((raw_data[0] == MEASURE_MODE) && (cmd == SETRAWDATAMODE))
					{
						printf("<< GET MEASURE MODE\r\n\r\n");
						return 1;
					}
					else
					{
						break;
					}

				case GETRAWDATACONFIG:	

					if((cmd == SETRAWDATACONFIG) || (cmd == GETRAWDATACONFIG))
					{
						enabled_plane_num[port_num] = GET_CONFIG_FIELD(port_num, CONFIG_PLAN_NUMBER_OnOff);
						enabled_frame_counter[port_num] = GET_CONFIG_FIELD(port_num, CONFIG_CAN_ID_FRAME_COUNTER);
						enabled_lzr_information[port_num] = GET_CONFIG_FIELD(port_num, CONFIG_LZR_INFORMATION);

						//printf("%d %d %d\r\n", enabled_plane_num, enabled_frame_counter, enabled_lzr_information);
						printf("<< GET CONFIG DATA\r\n\r\n");
						//For debug
						return 1;
					}
					else
					{
						break;
					}

				case GETRAWDATAREDLASER:
					if(cmd == SETRAWDATAREDLASER)
					{
						printf("<< GET RED LASER %d\r\n\r\n", raw_data[0]);
						return 1;
					}
					else
					{
						break;
					}

				default:
					break;
			}//End of switch(cmd)
		}//End of if(GET_RX_BUFFER_READY(port_num))

		//MCU need to move for checking every queue buffer
		SET_RX_BUFFER_NEXT(port_num);

	}

	return 0;
}

unsigned int GET_RX_BUFFER_DISTANCE_DATA(
		unsigned port_num, 
		unsigned char *plane0, 
		unsigned char *plane1, 
		unsigned char *plane2, 
		unsigned char *plane3,
		unsigned char *frame_data,
		unsigned char *info_data
		)
{

	unsigned char cmd = 0;
	unsigned char *raw_data;

#if defined(LZR_U920)
	unsigned char plane_num;
	unsigned char *plane_data[4] = {NULL, };
#endif

	if(GET_RX_BUFFER_READY(port_num))
	{
		cmd = GET_RX_BUFFER_CMD(port_num);
		raw_data = GET_RX_BUFFER(port_num, 0);

		switch(cmd)
		{
			case GETRAWDATADISTANCEVALUES:

				if(enabled_frame_counter[port_num])
				{
					/*
					   printf("%x%x%x%x%x%x\r\n",
					   raw_data[0],
					   raw_data[1],
					   raw_data[2],
					   raw_data[3],
					   raw_data[4],
					   raw_data[5]
					   );
					   */
					memcpy(frame_data, raw_data, ID_FRAME_SIZE);
					raw_data = raw_data + ID_FRAME_SIZE;
				}

				if(enabled_lzr_information[port_num])
				{
					memcpy(info_data, raw_data, INFORMATION_SIZE);
					raw_data = raw_data + INFORMATION_SIZE;
				}

				if(enabled_plane_num[port_num])
				{
#if defined(LZR_U920)
					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;
					raw_data = raw_data + PLANE_NUM_SIZE + PLANE_SIZE;

					plane_num = GET_PLANE_NUM(raw_data[0]);
					plane_data[plane_num] = raw_data + PLANE_NUM_SIZE;

					memcpy(plane0, plane_data[PLANE_0], PLANE_SIZE);
					memcpy(plane1, plane_data[PLANE_1], PLANE_SIZE);
					memcpy(plane2, plane_data[PLANE_2], PLANE_SIZE);
					memcpy(plane3, plane_data[PLANE_3], PLANE_SIZE);

#else
					raw_data = raw_data + PLANE_NUM_SIZE;
					memcpy(plane0, raw_data, PLANE_SIZE);
#endif
				}
				else
				{
#if defined(LZR_U920)
					plane_data[1] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[3] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[0] = raw_data;
					raw_data = raw_data + PLANE_SIZE;

					plane_data[2] = raw_data;


					memcpy(plane0, plane_data[PLANE_0], PLANE_SIZE);
					memcpy(plane1, plane_data[PLANE_1], PLANE_SIZE);
					memcpy(plane2, plane_data[PLANE_2], PLANE_SIZE);
					memcpy(plane3, plane_data[PLANE_3], PLANE_SIZE);
#else
					memcpy(plane0, raw_data, PLANE_SIZE);
#endif
				}

				break;
			default:
				break;
		}
	}
	SET_RX_BUFFER_NEXT(port_num);

	if(cmd == 0)
		return 0;
	else
		return GET_RX_BUFFER_LENGTH(port_num);
}


int GET_LSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	int i;
	int result = 0;

	if (((byte_digit % 2) != 0) || byte_digit <= 1)
	{
		printf("This is not even array data, Failed\r\n");
		return 0;
	}

	for(i = byte_digit-1 ; i >= 0; i -= 1)
	{
		result = result | (buff [ (index * byte_digit) + i] << (8 * i) );
		//printf("%x %x \r\n", buff[ (index * byte_digit) + i], result);
	}

	return result;
}

int GET_MSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	int i;
	int j;
	int result = 0;

	if (((byte_digit % 2) != 0) || byte_digit <= 1)
	{
		printf("This is not even array data, Failed\r\n");
		return 0;
	}

	j = byte_digit - 1;
	for(i = 0 ; i < byte_digit; i += 1)
	{
		result = result | (buff [ (index * byte_digit) + i] << (8 * j) );
		j -= 1;
		//printf("%x %x \r\n", buff[ (index * byte_digit) + i], result);
	}

	return result;
}

void SET_LSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	buff[index + 0] = data & 0xff;
	buff[index + 1] = (data >> 8) & 0xff;

	if(byte_digit == INTEGER)
	{
		buff[index + 2] = (data >> 16) & 0xff;
		buff[index + 3] = (data >> 24) & 0xff;
	}
}

void SET_MSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	if(byte_digit == SHORT)
	{
		buff[index + 0] = (data >> 8) & 0xff;
		buff[index + 1] = (data     ) & 0xff;
	}
	else
	{		
		buff[index + 0] = (data >> 24) & 0xff;
		buff[index + 1] = (data >> 16) & 0xff;
		buff[index + 2] = (data >>  8) & 0xff;
		buff[index + 3] = (data      ) & 0xff;
	}
}

#endif
