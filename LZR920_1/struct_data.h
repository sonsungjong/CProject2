/*
 * struct_data.h
 *
 *  Created on: 2020. 4. 17.
 *      Author: Tommy
 */

#ifndef INC_STRUCT_DATA_H_
#define INC_STRUCT_DATA_H_

#define QUEUE_MAX_DATA_SIZE 600 // 274*2
#define STACK_MAX_DATA_SIZE 50

typedef struct _QUEUE_BUFFER
{
	int data_size;
	unsigned char data[QUEUE_MAX_DATA_SIZE];
}QUEUE_BUFFER;

typedef struct _STACK_BUFFER
{
	int data_size;
	unsigned char data[STACK_MAX_DATA_SIZE];
}STACK_BUFFER;


int init_stack(int size);
int push_stack(unsigned char *data, int data_size);
STACK_BUFFER* pop_stack(void);

int init_queue(int size);
int push_queue(unsigned char *data, int data_size);
QUEUE_BUFFER* pop_queue(void);


#endif /* INC_STRUCT_DATA_H_ */
