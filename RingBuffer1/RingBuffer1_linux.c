#ifdef __linux
#include "RingBuffer1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

static pthread_mutex_t g_mtx[256];
static pthread_cond_t  g_cv[256];
static int g_using[256] = { 0, };  // [0] not using, [1] using

void RingBuffer_init(ST_RingBuffer* rb, const unsigned long long max_buf_size, unsigned char _id)
{
	rb->data = NULL;
	rb->capacity = 1024 * 1024;						// �⺻ 1MB
	rb->head = 0;
	rb->tail = 0;
	rb->id = _id;

	if (max_buf_size > 0) {
		rb->capacity = max_buf_size;				// �ִ� ���� ���� (�Ѿ�� ���)
	}

	if (rb->capacity > 0) {
		rb->data = malloc(rb->capacity);			// �����ۿ� �޸� �Ҵ�
	}

	// id�� ���� ����ȭ
	pthread_mutex_init(&g_mtx[rb->id], NULL);
	pthread_cond_init(&g_cv[rb->id], NULL);
	g_using[rb->id] = 1;				// ��� ������ ����
}

// ���۰� �������� ����� (����� ū ũ�⸦ �Ҵ��ؼ� ����� ��)
void RingBuffer_write(ST_RingBuffer* rb, const unsigned char* data, const unsigned long long data_size)
{
	pthread_mutex_lock(&g_mtx[rb->id]);

	for (unsigned long long idx = 0; idx < data_size; idx++)
	{
		rb->data[rb->tail] = data[idx];
		unsigned long long next_tail = (rb->tail + 1) % rb->capacity;

		// �����: ���� ���� tail�� head�� ������ ���۰� �� �� �����̹Ƿ� head�� �������� ���
		if (next_tail == rb->head)
		{
			rb->head = (rb->head + 1) % rb->capacity;
		}
		rb->tail = next_tail;
	}

	// ������ �߰��Ǿ����Ƿ� ��� ���� �б� �����带 �����.
	pthread_cond_signal(&g_cv[rb->id]);
	pthread_mutex_unlock(&g_mtx[rb->id]);
}

// �������� ���۸� ��� �д´� (return ��������Ʈ��, data*�� ���� ������ ����)
unsigned long long RingBuffer_read(ST_RingBuffer* rb, unsigned char** data)
{
	unsigned long long count = 0;
	pthread_mutex_lock(&g_mtx[rb->id]);

	// �����Ͱ� ������
	if (rb->head != rb->tail)
	{
		// ���� �����ͷ��� ����� ��
		if (rb->tail > rb->head)
		{
			count = rb->tail - rb->head;		// ���� �����ͷ�
		}
		else
		{
			count = rb->capacity - rb->head + rb->tail;	// ���� �����ͷ�
		}

		// data�� �Ҵ��ϰ�

		*data = (unsigned char*)malloc(count * sizeof(unsigned char));

		if (*data)
		{
			// �����͸� ī���Ѵ�
			if (rb->tail > rb->head)
			{
				memcpy(*data, rb->data + rb->head, count);			// ���� �����ͷ��� head�� tail���̿� ���� ��
			}
			else
			{
				unsigned long long first_part = rb->capacity - rb->head;	// ù��° �κ�
				unsigned long long second_part = rb->tail;				// �ι�° �κ�
				memcpy(*data, rb->data + rb->head, first_part);			// ù��° �κ�
				memcpy(*data + first_part, rb->data, second_part);		// �ι�° �κ�
			}
			// ���� �� head�� tail�� ���缭 �� ���� ���·� �����
			rb->head = rb->tail;
		}
		else {
			// �Ҵ� ���� �� count = 0
			count = 0;
		}
	}

	pthread_mutex_unlock(&g_mtx[rb->id]);
	return count;				// ���� ũ��
}

unsigned long long RingBuffer_wait_read(ST_RingBuffer* rb, unsigned char** data)
{
	unsigned long long count = 0;
	pthread_mutex_lock(&g_mtx[rb->id]);

	// �����Ͱ� ������ ���
	while ((rb->head == rb->tail) && (g_using[rb->id] == 1))
	{
		pthread_cond_wait(&g_cv[rb->id], &g_mtx[rb->id]);
	}

	if (g_using[rb->id] == 1)				// ��� ���� ���� (�����÷���)
	{
		// �����Ͱ� ������ ��� �о�ͼ� data�� �Ҵ��Ų��
		if (rb->tail > rb->head)
		{
			count = rb->tail - rb->head;		// ���� �����ͷ�
		}
		else
		{
			count = rb->capacity - rb->head + rb->tail;	// ���� �����ͷ�
		}

		*data = (unsigned char*)malloc(count * sizeof(unsigned char));
		if (*data)
		{
			if (rb->tail > rb->head)
			{
				memcpy(*data, rb->data + rb->head, count);			// ���� �����ͷ��� head�� tail���̿� ���� ��
			}
			else
			{
				unsigned long long first_part = rb->capacity - rb->head;
				unsigned long long second_part = rb->tail;
				memcpy(*data, rb->data + rb->head, first_part);
				memcpy(*data + first_part, rb->data, second_part);
			}
			// ���� �� head�� tail�� ���缭 �����͸� �Һ��� ������ ó���Ѵ�
			rb->head = rb->tail;
		}
		else
		{
			count = 0;			// �Ҵ� ����
		}
	}

	pthread_mutex_unlock(&g_mtx[rb->id]);
	return count;				// ���� ũ��
}

// �������� ������ ������
unsigned long long RingBuffer_cur_size(ST_RingBuffer* rb)
{
	unsigned long long size = 0;
	pthread_mutex_lock(&g_mtx[rb->id]);

	if (rb->head <= rb->tail)
	{
		size = rb->tail - rb->head;
	}
	else
	{
		size = rb->capacity - rb->head + rb->tail;
	}

	pthread_mutex_unlock(&g_mtx[rb->id]);
	return size;
}

// �����۸� �ʱ�ȭ�Ͽ� �� ���·� �����
void RingBuffer_reset(ST_RingBuffer* rb)
{
	pthread_mutex_lock(&g_mtx[rb->id]);
	rb->head = 0;
	rb->tail = 0;
	pthread_mutex_unlock(&g_mtx[rb->id]);
}

// �ش� �������� �Ҵ�� �޸𸮿� ����ȭ ��ü�� �����Ѵ�.
void RingBuffer_destroy(ST_RingBuffer* rb)
{
	if (g_using[rb->id] == 1)
	{
		pthread_mutex_lock(&g_mtx[rb->id]);
		g_using[rb->id] = 0;				// ��� ���� ó��
		pthread_cond_broadcast(&g_cv[rb->id]);               // ��� ���� ������ ��� �����

		rb->head = 0;
		rb->tail = 0;
		if (rb->data != NULL)
		{
			free(rb->data);
			rb->data = NULL;
		}
		pthread_mutex_unlock(&g_mtx[rb->id]);
		
		// ����
		pthread_mutex_destroy(&g_mtx[rb->id]);
		pthread_cond_destroy(&g_cv[rb->id]);
	}
}

#endif		__linux