/*
 * struct_data.c
 *
 *  Created on: 2020. 4. 17.
 *      Author: Tommy
 */

#include <struct_data.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG

#ifdef DEBUG
#define DEBUG_printf	printf
#else
#define DEBUG_printf(...)
#endif

//QUEUE
static QUEUE_BUFFER *frame_buffer;
static int QUEUE_SIZE;
static int queue_front;
static int queue_rear;
//QUEUE

int init_queue(int size)
{
	frame_buffer = NULL;
	QUEUE_SIZE = size;
	queue_front = queue_rear = -1;
	frame_buffer = (QUEUE_BUFFER *)malloc(QUEUE_SIZE * sizeof(QUEUE_BUFFER));
	if(frame_buffer == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int push_queue(unsigned char *data, int data_size)
{
	if((queue_rear+1)%QUEUE_SIZE == queue_front)
	{
		return 0;
	}

	queue_rear = (queue_rear+1)%QUEUE_SIZE;

	DEBUG_printf("queue push %d \n\r", queue_rear);
	frame_buffer[queue_rear].data_size = data_size;
	memcpy(frame_buffer[queue_rear].data, data, data_size);
	return 1;
}

QUEUE_BUFFER* pop_queue(void)
{
	if(queue_front == queue_rear)
	{
		return NULL;
	}
	queue_front = (queue_front+1)%QUEUE_SIZE;
	DEBUG_printf("queue pop %d \n\r", queue_front);
	//return frame_buffer[queue_front].data;
	return &frame_buffer[queue_front];
}

// STACK
static int STACK_SIZE = 0;
static STACK_BUFFER *stack = NULL;
static int top = -1;
// STACK

int init_stack(int size)
{
	STACK_SIZE = size;
	top = -1;
	stack = (STACK_BUFFER *)malloc(STACK_SIZE * sizeof(STACK_BUFFER));
	if(stack == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


int push_stack(unsigned char *data, int data_size)
{
	if(top < STACK_SIZE-1)
	{
		top = top + 1;
		stack[top].data_size = data_size;
		memcpy(stack[top].data, data, data_size);
//		DEBUG_printf("Push top : %d %d\n", top, x);
		return 1;
	}

	return 0;
}

STACK_BUFFER* pop_stack(void)
{
	STACK_BUFFER *temp;

	if(top != -1)
	{
		temp = &stack[top];
//		DEBUG_printf("Pop  top : %d %d\n", top, temp);
		top = top - 1;
		return temp;
	}
	return NULL;
}
