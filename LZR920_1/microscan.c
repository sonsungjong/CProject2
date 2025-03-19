
#include "main.h"
#ifdef MICROSCAN

#include "microscan.h"
#include "stdlib.h"
#include "usart.h"

#define DELAY_TIME 40
#define BEA_POLYNOM 0x90d9

CONFIG_DATA config_data[uSCAN_CNT];
Node *recieved_buffer[uSCAN_CNT] = {NULL, }; // For RS485 interrupt handler
Node *read_buffer[uSCAN_CNT] = {NULL, };       // For data tracking
unsigned int buff_index[uSCAN_CNT] = {0,};
unsigned short crc_result[uSCAN_CNT] = {0, };
BACKGROUND_FACET back_ground[uSCAN_CNT];

unsigned short CRC16(unsigned char *buf_,unsigned short cnt_)
{
	unsigned short crc = 0; /* CRC value is 16bit */
	unsigned short i, j;
	for(i = 0; i < cnt_; i++)
	{
		crc ^= (unsigned short)(buf_[i] << 8); /* move unsigned char into MSB of 16bit
												  CRC */
		for (j = 0; j < 8; j++)
		{
			if ((crc & 0x8000) != 0) /* test for MSB = bit 15 */
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

unsigned short GET_BACKGROUND_DATA(unsigned char port_num, int facet_num, int index)
{
	switch(facet_num)
	{
		case FACET1:
			return back_ground[port_num].facet1[index];
		case FACET2:
			return back_ground[port_num].facet2[index];
		case FACET3:
			return back_ground[port_num].facet3[index];
		default:
			return back_ground[port_num].facet4[index];
	}
}

void SET_BACKGROUND_DATA(unsigned char port_num, int facet_num, int index, unsigned short data)
{
	switch(facet_num)
	{
		case FACET1:
			back_ground[port_num].facet1[index] = data;
			break;
		case FACET2:
			back_ground[port_num].facet2[index] = data;
			break;
		case FACET3:
			back_ground[port_num].facet3[index] = data;
			break;
		default:
			back_ground[port_num].facet4[index] = data;
			break;
	}
}

//void ALLOC_MICROSCAN_MEMORY(void)
int ALLOC_MEMORY(void)
{
	int i, j;
	Node *lzr_buffer[uSCAN_CNT];
	printf("\r\nLZR MEMORY ALLOC START\r\n");

	for(i = 0; i < uSCAN_CNT ; i++)
	{
		//LZR_Init(i);
		lzr_buffer[i] 		= Make_buffer(PACKET_BUFFER_COUNT);
		read_buffer[i]		= lzr_buffer[i];       // For data tracking
		recieved_buffer[i] 	= lzr_buffer[i]; // For RS485 interrupt handler
	}


	for(i = 0; i < uSCAN_CNT ; i++)
	{

		back_ground[i].facet1 = (unsigned short *)malloc(sizeof(unsigned short)* 100);
		back_ground[i].facet2 = (unsigned short *)malloc(sizeof(unsigned short)* 100);
		back_ground[i].facet3 = (unsigned short *)malloc(sizeof(unsigned short)* 100);
		back_ground[i].facet4 = (unsigned short *)malloc(sizeof(unsigned short)* 100);

		for(j = 0; j < 250; j++)
		{
			back_ground[i].facet1[j] = 0xffff;
			back_ground[i].facet2[j] = 0xffff;
			back_ground[i].facet3[j] = 0xffff;
			back_ground[i].facet4[j] = 0xffff;
		}
	}

	printf("LZR MEMORY ALLOC DONE\r\n");
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
	unsigned int i =buffer_cnt- 1;
	Node *start = NULL;
	Node *current = NULL;
	Node *newnode = NULL;
	start = CreateNode();

	current = start;

	if(buffer_cnt == 0)
	{
		printf("Only one buffer\r\n");
		start->next = start;
		return start;
	}

	while(i--)
	{
		newnode = CreateNode();
		current->next = newnode;
		current = current->next;
	}
	current->next = start;

	return start;
}


unsigned short crc_calculation(unsigned short crc, unsigned char uch)
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
		//led_on(0);
		return 0;

	switch(recieved_buffer[port_num]->sync)
	{
		case SYNC_0 :	buff_index[port_num] = 0;
						crc_result[port_num] = 0;
						if(uch == SYNC_0) //0xbe	
						{
							recieved_buffer[port_num]->sync = SYNC_1;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_1 : 	if(uch == SYNC_1) //0xa0
						{
							recieved_buffer[port_num]->sync = SYNC_2;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else 			
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_2 : 	if(uch == SYNC_2) //0x12
						{
							recieved_buffer[port_num]->sync = SYNC_3;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;	
		case SYNC_3 : 	if(uch == SYNC_3) //0x34
						{
							recieved_buffer[port_num]->sync = SYNC_4;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else 			
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_4 : 	if(uch == SYNC_4) // Version of the protocol
						{
							recieved_buffer[port_num]->sync = SYNC_5;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
						// Total size of the frame in bytes, low data
		case SYNC_5 : 	recieved_buffer[port_num]->length = uch;
						recieved_buffer[port_num]->sync = SYNC_6;
						recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
						// Total size of the frame in bytes, high data
		case SYNC_6 :	recieved_buffer[port_num]->length = ((uch<<8) | recieved_buffer[port_num]->length); 
						//Only data size, Except the SYNC, CMD, CHK count
						recieved_buffer[port_num]->sync = SYNC_7;
						recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
		case SYNC_7 : 	if(uch == 0x02) // Verification method
						{
							recieved_buffer[port_num]->sync = SYNC_8;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_8 :	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = SYNC_9;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_9 :	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = SYNC_10;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case SYNC_10:	if(uch == 0)
						{
							recieved_buffer[port_num]->sync = CMD_L;
							recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
							crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						}
						else
							recieved_buffer[port_num]->sync = SYNC_0;
						break;
		case CMD_L	: 	recieved_buffer[port_num]->sync = CMD_H;
						recieved_buffer[port_num]->cmd = uch;
						recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						break;
		case CMD_H	: 	// 0xC3
						recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						if((recieved_buffer[port_num]->length-CHK_SIZE) == 0)
							recieved_buffer[port_num]->sync = CHK_0;
						else
							recieved_buffer[port_num]->sync = DATA;
						break;
		case DATA :     recieved_buffer[port_num]->recieved_rawdata[buff_index[port_num]++] = uch;
						crc_result[port_num] = crc_calculation(crc_result[port_num], uch);
						if(buff_index[port_num] >= (recieved_buffer[port_num]->length-CHK_SIZE))
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
							//led_toggle(3);
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


//config_data[port_num].spot_number_facet[FACET1]
//back_ground[uSCAN_CNT].facet1[j] = 0xffff;
unsigned short Get_spot_number_facet(unsigned char port_num, int facet_num)
{
	return config_data[port_num].spot_number_facet[facet_num];
}

void teach_in(unsigned char port_num , unsigned int count, unsigned short off_set)
{

	int i;
	int cnt = 0;
	int data_size = 0;
	unsigned char cmd = 0;
	unsigned char *frame_buff,*buff1, *buff2, *buff3, *buff4;
	volatile unsigned short distance;

	int address_offset = 0;

	if(get_can_status(port_num))
	{
		address_offset = address_offset + CAN_SIZE;
	}
	if(get_counter_status(port_num))
	{
		address_offset = address_offset + COUNT_ER_SIZE;
	}
	if(get_ctn_status(port_num))
	{
		address_offset = address_offset + CTN_SIZE;
	}
	

	for (i = 0; i < config_data[port_num].spot_number_facet[FACET1]; i++)
	{
		back_ground[port_num].facet1[i] = 0xffff;
	}

	for (i = 0; i < config_data[port_num].spot_number_facet[FACET2]; i++)
	{
		back_ground[port_num].facet2[i] = 0xffff;
	}

	for (i = 0; i < config_data[port_num].spot_number_facet[FACET3]; i++)
	{
		back_ground[port_num].facet3[i] = 0xffff;
	}

	for (i = 0; i < config_data[port_num].spot_number_facet[FACET4]; i++)
	{
		back_ground[port_num].facet4[i] = 0xffff;
	}

	while( cnt < count )
	{
		//frame_buff = analyze_message(port_num, &data_size, &cmd) + (CAN_SIZE+COUNT_ER_SIZE+CTN_SIZE);
		frame_buff = analyze_message(port_num, &data_size, &cmd) + address_offset;
		if(cmd == MDI)
		{
			if (config_data[port_num].spot_number_facet[FACET1] != 0)
			{
				buff1 = assigned_buffer(port_num, frame_buff, FACET1);
				for (i = 0; i < config_data[port_num].spot_number_facet[FACET1]; i++)
				{
					distance = GET_SHORT_DATA(buff1, i);
					if (distance < back_ground[port_num].facet1[i])
						back_ground[port_num].facet1[i] = distance - off_set;
				}
			}

			if (config_data[port_num].spot_number_facet[FACET2] != 0)
			{
				buff2 = assigned_buffer(port_num, frame_buff, FACET2);
				for (i = 0; i < config_data[port_num].spot_number_facet[FACET2]; i++)
				{
					distance = GET_SHORT_DATA(buff2, i);
					if (distance < back_ground[port_num].facet2[i])
						back_ground[port_num].facet2[i] = distance - off_set;
				}
			}

			if (config_data[port_num].spot_number_facet[FACET3] != 0)
			{
				buff3 = assigned_buffer(port_num, frame_buff, FACET3);

				for (i = 0; i < config_data[port_num].spot_number_facet[FACET3]; i++)
				{
					distance = GET_SHORT_DATA(buff3, i);
					if (distance < back_ground[port_num].facet3[i])
						back_ground[port_num].facet3[i] = distance - off_set;
				}
			}

			if (config_data[port_num].spot_number_facet[FACET4] != 0)
			{
				buff4 = assigned_buffer(port_num, frame_buff, FACET4);

				for (i = 0; i < config_data[port_num].spot_number_facet[FACET4]; i++)
				{
					distance = GET_SHORT_DATA(buff4, i);
					if (distance < back_ground[port_num].facet4[i])
						back_ground[port_num].facet4[i] = distance - off_set;
				}
			}
			cnt++;
		}
	}
}


unsigned char read_buffer_ready(unsigned char port_num)
{
	if(read_buffer[port_num]->ready)
	{
		read_buffer[port_num]->ready = RESET;		
		return 1;
	}
	else
		return 0;
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
unsigned char *rawdata_copy(unsigned char port_num, int jump_address)
{
	return (read_buffer[port_num]->recieved_rawdata) + jump_address;
}

double read_config_angle_first(unsigned char port_num, int facet_num)
{
	double result;
	result = (double)config_data[port_num].angle_first_facet[facet_num]/100.0;
	return result;
}
double read_config_angle_last(unsigned char port_num, int facet_num)
{	
	double result;
	result = (double)config_data[port_num].angle_last_facet[facet_num]/100.0;
	return result;
}

int get_ctn_status(unsigned char port_num)
{
	return config_data[port_num].ctn_field_in_measurement;
}
int get_can_status(unsigned char port_num)
{
	return config_data[port_num].can_enable;
}
int get_counter_status(unsigned char port_num)
{
	return config_data[port_num].frame_counter_enable;
}

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
		)
{
	config_data[port_num].ctn_field_in_measurement = ctn_enable;
	config_data[port_num].measurement_information = mri_enable;

	config_data[port_num].angle_first_facet[FACET1] = facet1_angle_first;
	config_data[port_num].angle_last_facet[FACET1] = facet1_angle_last;
	config_data[port_num].spot_number_facet[FACET1] = facet1_spot_numner;

	config_data[port_num].angle_first_facet[FACET2] = facet2_angle_first;
	config_data[port_num].angle_last_facet[FACET2] = facet2_angle_last;
	config_data[port_num].spot_number_facet[FACET2] = facet2_spot_numner;

	config_data[port_num].angle_first_facet[FACET3] = facet3_angle_first;
	config_data[port_num].angle_last_facet[FACET3] = facet3_angle_last;
	config_data[port_num].spot_number_facet[FACET3] = facet3_spot_numner;

	config_data[port_num].angle_first_facet[FACET4] = facet4_angle_first;
	config_data[port_num].angle_last_facet[FACET4] = facet4_angle_last;
	config_data[port_num].spot_number_facet[FACET4] = facet4_spot_numner;

	config_data[port_num].frame_counter_enable = frame_counter_enable;
	config_data[port_num].can_enable = can_enable;
	config_data[port_num].heartbeat_period = heartbeat_period;

	config_data[port_num].facet_Num_enable = 0x01;	//Fixed value
	config_data[port_num].baudrate = baudrate;
}

void set_parameter(unsigned char port_num)
{
	//unsigned char *packet;
	const unsigned short length = SYNC_SIZE+CMD_SIZE+PARAMETER_SIZE+CHK_SIZE;
	unsigned short crc16;
	//packet = (unsigned char *)malloc(sizeof(unsigned char) * length);
	unsigned char packet[SYNC_SIZE+CMD_SIZE+PARAMETER_SIZE+CHK_SIZE] = {0, };

	// SYNC part //
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
	// SYNC part //

	// CMD part //
	packet[11] = SET_PARAMETERS;
	packet[12] = CMD_H;
	// CMD part //

	// DATA part //
	packet[13] = 0;
	packet[14] = config_data[port_num].ctn_field_in_measurement;
	packet[15] = config_data[port_num].measurement_information;
	packet[16] = 0;
	packet[17] = 0;
	packet[18] = 0;
	packet[19] = 0;
	packet[20] = 0;
	packet[21] = 0;
	packet[22] = 0;
	packet[23] = 0;
	packet[24] = 0;
	packet[25] = 0;
	packet[26] = 0;
	
	packet[27] = config_data[port_num].angle_first_facet[FACET1] & 0xff;
	packet[28] = (config_data[port_num].angle_first_facet[FACET1]>>8) & 0xff;
	packet[29] = config_data[port_num].angle_last_facet[FACET1] & 0xff;
	packet[30] = (config_data[port_num].angle_last_facet[FACET1]>>8) & 0xff;
	packet[31] = config_data[port_num].spot_number_facet[FACET1] & 0xff;
	packet[32] = (config_data[port_num].spot_number_facet[FACET1]>>8) & 0xff;

	packet[33] = config_data[port_num].angle_first_facet[FACET2] & 0xff;
	packet[34] = (config_data[port_num].angle_first_facet[FACET2]>>8) & 0xff;
	packet[35] = config_data[port_num].angle_last_facet[FACET2] & 0xff;
	packet[36] = (config_data[port_num].angle_last_facet[FACET2]>>8) & 0xff;
	packet[37] = config_data[port_num].spot_number_facet[FACET2] & 0xff;
	packet[38] = (config_data[port_num].spot_number_facet[FACET2]>>8) & 0xff;

	packet[39] = config_data[port_num].angle_first_facet[FACET3] & 0xff;
	packet[40] = (config_data[port_num].angle_first_facet[FACET3]>>8) & 0xff;
	packet[41] = config_data[port_num].angle_last_facet[FACET3] & 0xff;
	packet[42] = (config_data[port_num].angle_last_facet[FACET3]>>8) & 0xff;
	packet[43] = config_data[port_num].spot_number_facet[FACET3] & 0xff;
	packet[44] = (config_data[port_num].spot_number_facet[FACET3]>>8) & 0xff;
	
	packet[45] = config_data[port_num].angle_first_facet[FACET4] & 0xff;
	packet[46] = (config_data[port_num].angle_first_facet[FACET4]>>8) & 0xff;
	packet[47] = config_data[port_num].angle_last_facet[FACET4] & 0xff;
	packet[48] = (config_data[port_num].angle_last_facet[FACET4]>>8) & 0xff;
	packet[49] = config_data[port_num].spot_number_facet[FACET4] & 0xff;
	packet[50] = (config_data[port_num].spot_number_facet[FACET4]>>8) & 0xff;

	packet[51] = config_data[port_num].frame_counter_enable;
	packet[52] = config_data[port_num].can_enable;
	packet[53] = config_data[port_num].heartbeat_period;
	packet[54] = config_data[port_num].facet_Num_enable;
	packet[55] = config_data[port_num].baudrate;
	// DATA part //

	crc16 = CRC16(packet, length-CHK_SIZE);
	packet[56] = crc16 & 0xff;
	packet[57] = (crc16 >> 8) & 0xff;

	printf("Set Parameter\r\n");
	for(int i = 0; i < length;i++)
	{
		//printf("%02x ", packet[i]);
		delay(DELAY_TIME);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\r\n");
	//free(packet);

}
//get_parameter();
//store_parameter();
//get_identity();
void send_command(unsigned char port_num, unsigned char cmd)
{
	//unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE;
	unsigned short crc16;
	//packet = (unsigned char *)malloc(sizeof(unsigned char) * length);
	unsigned char packet[SYNC_SIZE+CMD_SIZE+CHK_SIZE] ={0, };

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
	packet[11] = cmd;
	packet[12] = CMD_H;
	crc16 = CRC16(packet, length-CHK_SIZE);
	packet[13] = crc16 & 0xff;
	packet[14] = (crc16 >> 8) & 0xff;

	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(DELAY_TIME);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\r\n");
	//free(packet);

}
void get_measurements(unsigned char port_num, unsigned char mode)
{
	//unsigned char *packet;
	unsigned short length = SYNC_SIZE+CMD_SIZE+CHK_SIZE+1;
	unsigned short crc16;
	//packet = (unsigned char *)malloc(sizeof(unsigned char) * length);
	unsigned char packet[SYNC_SIZE+CMD_SIZE+CHK_SIZE+1] = {0, };

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
	crc16 = CRC16(packet, length-CHK_SIZE);
	packet[14] = crc16 & 0xff;
	packet[15] = (crc16 >> 8) & 0xff;

	for(int i = 0; i < length;i++)
	{
		//printf("%2x", packet[i]);
		delay(DELAY_TIME);
		RS485_Send(port_num, &(packet[i]), 1);
	}
	//printf("\r\n");
	//free(packet);
}

//CONFIG_DATA config_data[uSCAN_CNT];
void input_config_data(unsigned char *buff, unsigned char port_num)
{
	config_data[port_num].ctn_field_in_measurement = buff[7];
	config_data[port_num].measurement_information = buff[8];

	config_data[port_num].angle_first_facet[FACET1] = (buff[15] << 8) | buff[14];
	config_data[port_num].angle_last_facet[FACET1]  = (buff[17] << 8) | buff[16];
	config_data[port_num].spot_number_facet[FACET1] = (buff[19] << 8) | buff[18];

	config_data[port_num].angle_first_facet[FACET2] = (buff[21] << 8) | buff[20];
	config_data[port_num].angle_last_facet[FACET2]  = (buff[23] << 8) | buff[22];
	config_data[port_num].spot_number_facet[FACET2] = (buff[25] << 8) | buff[24];
	
	config_data[port_num].angle_first_facet[FACET3] = (buff[27] << 8) | buff[26];
	config_data[port_num].angle_last_facet[FACET3]  = (buff[29] << 8) | buff[28];
	config_data[port_num].spot_number_facet[FACET3] = (buff[31] << 8) | buff[30];

	config_data[port_num].angle_first_facet[FACET4] = (buff[33] << 8) | buff[32];
	config_data[port_num].angle_last_facet[FACET4]  = (buff[35] << 8) | buff[34];
	config_data[port_num].spot_number_facet[FACET4] = (buff[37] << 8) | buff[36];
	
	config_data[port_num].frame_counter_enable = buff[38];
	config_data[port_num].can_enable = buff[39];
	config_data[port_num].heartbeat_period = buff[40];
	config_data[port_num].facet_Num_enable = buff[41];
	config_data[port_num].baudrate = buff[42];


	printf("CTN field in measurement frames : %d\r\n", config_data[port_num].ctn_field_in_measurement);
	printf("Measurement information			: %d\r\n", config_data[port_num].measurement_information);

	printf("Angle_first: limit of the detection facet1 :%d\r\n", config_data[port_num].angle_first_facet[FACET1]);
	printf("Angle_last : limit of the detection facet1 :%d\r\n", config_data[port_num].angle_last_facet[FACET1]);
	printf("Spot_number: limit of the detection facet1 :%d\r\n", config_data[port_num].spot_number_facet[FACET1]);

	printf("Angle_first: limit of the detection facet2 :%d\r\n", config_data[port_num].angle_first_facet[FACET2]);
	printf("Angle_last : limit of the detection facet2 :%d\r\n", config_data[port_num].angle_last_facet[FACET2]);
	printf("Spot_number: limit of the detection facet2 :%d\r\n", config_data[port_num].spot_number_facet[FACET2]);

	printf("Angle_first: limit of the detection facet3 :%d\r\n", config_data[port_num].angle_first_facet[FACET3]);
	printf("Angle_last : limit of the detection facet3 :%d\r\n", config_data[port_num].angle_last_facet[FACET3]);
	printf("Spot_number: limit of the detection facet3 :%d\r\n", config_data[port_num].spot_number_facet[FACET3]);

	printf("Angle_first: limit of the detection facet4 :%d\r\n", config_data[port_num].angle_first_facet[FACET4]);
	printf("Angle_last : limit of the detection facet4 :%d\r\n", config_data[port_num].angle_last_facet[FACET4]);
	printf("Spot_number: limit of the detection facet4 :%d\r\n", config_data[port_num].spot_number_facet[FACET4]);

	printf("frame counter Enable: %d\r\n", config_data[port_num].frame_counter_enable);
	printf("CAN Enable: %d\r\n", config_data[port_num].can_enable);
	printf("heartbeat period : %d\r\n", config_data[port_num].heartbeat_period);
	printf("Facet number filed in MDI: %d\r\n", config_data[port_num].facet_Num_enable);
	printf("Baudrate: %d\r\n", config_data[port_num].baudrate);

}

void input_identity_data(unsigned char *buff, unsigned char port_num)
{
	config_data[port_num].product_part_number = (buff[3] << 24) | (buff[2] << 16) | (buff[1] << 8) | buff[0];
	config_data[port_num].sw_version = buff[4];
	config_data[port_num].sw_revision = buff[5];
	config_data[port_num].sw_prototype = buff[6];
	config_data[port_num].can_number_of_the_detector = (buff[10] << 24) | (buff[9] << 16) | (buff[8] << 8) | buff[7];
	config_data[port_num].can_number_of_the_detector2 = (buff[14] << 24) | (buff[13] << 16) | (buff[12] << 8) | buff[11];

	printf("Product part number (BEA TOF) : %d\r\n", config_data[port_num].product_part_number);
	printf("Software version %d\r\n", config_data[port_num].sw_version);
	printf("Software revision %d\r\n", config_data[port_num].sw_revision);
	printf("Software prototype %d\r\n", config_data[port_num].sw_prototype);
	printf("CAN number of the detector (Laser Head serial number) %d\r\n", config_data[port_num].can_number_of_the_detector);
	printf("CAN number of the detector (RPU BEA serial number) %d\r\n", config_data[port_num].can_number_of_the_detector2);
}
unsigned char *analyze_message(unsigned char port_num, int *data_size, unsigned char *cmd)
{

	unsigned char *temp;
	unsigned char *data_buffer = NULL;
	unsigned char command;

	*cmd = 0;
	if(read_buffer_ready(port_num))
	{						
		command = read_buffer_cmd(port_num);
		*cmd = command;
		switch(command)
		{
			case SEND_PARAMETERS : 
				input_config_data(rawdata_copy(port_num, (SYNC_SIZE+CMD_SIZE)), port_num);
				break;
			case STORE_PARAMETERS :
				// ?? There are no answer from the scanner.
				printf("STORE PARAMETERS is done\r\n");
				break;
			case SEND_IDENTITY :
				input_identity_data(rawdata_copy(port_num, (SYNC_SIZE+CMD_SIZE)), port_num);
				break;
			case MDI :
				*data_size = read_buffer_length(port_num);//-23;
				data_buffer = rawdata_copy(port_num, (SYNC_SIZE+CMD_SIZE));
				break;
			default :

				temp = rawdata_copy(port_num, 0);
				for(int i=0;i<27;i++)
					printf("%2x ", temp[i]);
				printf("\r\n");

				break;
		}
		//move_next_read_buffer(port_num);
	}
	move_next_read_buffer(port_num);
	//else
		//printf("Wait buffer Ready\r\n");

	return data_buffer;
}

unsigned char* assigned_buffer(unsigned char port_num, unsigned char *measured_data, int facet_num)
{
	switch(facet_num)
	{	//Facet_Num(Plant_Num) is 1byte
		case FACET1 :
			if (config_data[port_num].spot_number_facet[FACET1] != 0)
			{
				return measured_data + 1;//Facet_Num(Plant_Num) is 1byte
			}
			else
				return NULL;
			break;
		case FACET2 :
			if (config_data[port_num].spot_number_facet[FACET2] != 0)
			{
				return measured_data + (config_data[port_num].spot_number_facet[FACET1] << 1) + 2;
			}
			else
				return NULL;
			break;
		case FACET3 :
			if (config_data[port_num].spot_number_facet[FACET3] != 0)
			{
				return measured_data + (config_data[port_num].spot_number_facet[FACET1] << 1) + (config_data[port_num].spot_number_facet[FACET2] << 1) + 3;
			}
			else
				return NULL;
			break;
		case FACET4 :
			if (config_data[port_num].spot_number_facet[FACET4] != 0)
			{
				return measured_data + (config_data[port_num].spot_number_facet[FACET1] << 1) + (config_data[port_num].spot_number_facet[FACET2] << 1) + (config_data[port_num].spot_number_facet[FACET3] << 1) + 4 ;
			}
			else
				return NULL;
			break;
		default :
			return NULL;
			break;
	}
	/*
	if (config_data[port_num].spot_number_facet[FACET1] != 0)
		{
			facet[FACET1] = measured_data;
		}
		if (config_data[port_num].spot_number_facet[FACET2] != 0)
		{
			facet[FACET2] = measured_data + (config_data[port_num].spot_number_facet[FACET1] << 1);
		}
		if (config_data[port_num].spot_number_facet[FACET3] != 0)
		{
			facet[FACET3] = measured_data +
								(config_data[port_num].spot_number_facet[FACET1] << 1) +
								(config_data[port_num].spot_number_facet[FACET2] << 1);
		}
		if (config_data[port_num].spot_number_facet[FACET4] != 0)
		{
			facet[FACET4] = measured_data +
								(config_data[port_num].spot_number_facet[FACET1] << 1) +
								(config_data[port_num].spot_number_facet[FACET2] << 1) +
								(config_data[port_num].spot_number_facet[FACET3] << 1) ;
		}
		*/
}

#endif
