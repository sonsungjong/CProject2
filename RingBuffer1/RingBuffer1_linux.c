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
	rb->capacity = 1024 * 1024;						// 기본 1MB
	rb->head = 0;
	rb->tail = 0;
	rb->id = _id;

	if (max_buf_size > 0) {
		rb->capacity = max_buf_size;				// 최대 저장 개수 (넘어가면 덮어씀)
	}

	if (rb->capacity > 0) {
		rb->data = malloc(rb->capacity);			// 링버퍼에 메모리 할당
	}

	// id를 통해 동기화
	pthread_mutex_init(&g_mtx[rb->id], NULL);
	pthread_cond_init(&g_cv[rb->id], NULL);
	g_using[rb->id] = 1;				// 사용 중으로 변경
}

// 버퍼가 가득차면 덮어쓴다 (충분히 큰 크기를 할당해서 사용할 것)
void RingBuffer_write(ST_RingBuffer* rb, const unsigned char* data, const unsigned long long data_size)
{
	pthread_mutex_lock(&g_mtx[rb->id]);

	for (unsigned long long idx = 0; idx < data_size; idx++)
	{
		rb->data[rb->tail] = data[idx];
		unsigned long long next_tail = (rb->tail + 1) % rb->capacity;

		// 덮어쓰기: 만약 다음 tail이 head와 같으면 버퍼가 꽉 찬 상태이므로 head를 전진시켜 덮어씀
		if (next_tail == rb->head)
		{
			rb->head = (rb->head + 1) % rb->capacity;
		}
		rb->tail = next_tail;
	}

	// 데이터 추가되었으므로 대기 중인 읽기 스레드를 깨운다.
	pthread_cond_signal(&g_cv[rb->id]);
	pthread_mutex_unlock(&g_mtx[rb->id]);
}

// 읽지않은 버퍼를 모두 읽는다 (return 읽은바이트수, data*에 읽은 데이터 저장)
unsigned long long RingBuffer_read(ST_RingBuffer* rb, unsigned char** data)
{
	unsigned long long count = 0;
	pthread_mutex_lock(&g_mtx[rb->id]);

	// 데이터가 있으면
	if (rb->head != rb->tail)
	{
		// 읽을 데이터량을 계산한 후
		if (rb->tail > rb->head)
		{
			count = rb->tail - rb->head;		// 읽을 데이터량
		}
		else
		{
			count = rb->capacity - rb->head + rb->tail;	// 읽을 데이터량
		}

		// data에 할당하고

		*data = (unsigned char*)malloc(count * sizeof(unsigned char));

		if (*data)
		{
			// 데이터를 카피한다
			if (rb->tail > rb->head)
			{
				memcpy(*data, rb->data + rb->head, count);			// 읽을 데이터량이 head와 tail사이에 있을 때
			}
			else
			{
				unsigned long long first_part = rb->capacity - rb->head;	// 첫번째 부분
				unsigned long long second_part = rb->tail;				// 두번째 부분
				memcpy(*data, rb->data + rb->head, first_part);			// 첫번째 부분
				memcpy(*data + first_part, rb->data, second_part);		// 두번째 부분
			}
			// 읽은 후 head를 tail로 맞춰서 빈 버퍼 상태로 만든다
			rb->head = rb->tail;
		}
		else {
			// 할당 실패 시 count = 0
			count = 0;
		}
	}

	pthread_mutex_unlock(&g_mtx[rb->id]);
	return count;				// 읽은 크기
}

unsigned long long RingBuffer_wait_read(ST_RingBuffer* rb, unsigned char** data)
{
	unsigned long long count = 0;
	pthread_mutex_lock(&g_mtx[rb->id]);

	// 데이터가 없으면 대기
	while ((rb->head == rb->tail) && (g_using[rb->id] == 1))
	{
		pthread_cond_wait(&g_cv[rb->id], &g_mtx[rb->id]);
	}

	if (g_using[rb->id] == 1)				// 사용 중일 때만 (종료플래그)
	{
		// 데이터가 있으면 모두 읽어와서 data에 할당시킨다
		if (rb->tail > rb->head)
		{
			count = rb->tail - rb->head;		// 읽을 데이터량
		}
		else
		{
			count = rb->capacity - rb->head + rb->tail;	// 읽을 데이터량
		}

		*data = (unsigned char*)malloc(count * sizeof(unsigned char));
		if (*data)
		{
			if (rb->tail > rb->head)
			{
				memcpy(*data, rb->data + rb->head, count);			// 읽을 데이터량이 head와 tail사이에 있을 때
			}
			else
			{
				unsigned long long first_part = rb->capacity - rb->head;
				unsigned long long second_part = rb->tail;
				memcpy(*data, rb->data + rb->head, first_part);
				memcpy(*data + first_part, rb->data, second_part);
			}
			// 읽은 후 head를 tail로 맞춰서 데이터를 소비한 것으로 처리한다
			rb->head = rb->tail;
		}
		else
		{
			count = 0;			// 할당 실패
		}
	}

	pthread_mutex_unlock(&g_mtx[rb->id]);
	return count;				// 읽은 크기
}

// 읽지않은 데이터 사이즈
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

// 링버퍼를 초기화하여 빈 상태로 만든다
void RingBuffer_reset(ST_RingBuffer* rb)
{
	pthread_mutex_lock(&g_mtx[rb->id]);
	rb->head = 0;
	rb->tail = 0;
	pthread_mutex_unlock(&g_mtx[rb->id]);
}

// 해당 링버퍼의 할당된 메모리와 동기화 객체를 정리한다.
void RingBuffer_destroy(ST_RingBuffer* rb)
{
	if (g_using[rb->id] == 1)
	{
		pthread_mutex_lock(&g_mtx[rb->id]);
		g_using[rb->id] = 0;				// 사용 안함 처리
		pthread_cond_broadcast(&g_cv[rb->id]);               // 대기 중인 쓰레드 모두 깨우기

		rb->head = 0;
		rb->tail = 0;
		if (rb->data != NULL)
		{
			free(rb->data);
			rb->data = NULL;
		}
		pthread_mutex_unlock(&g_mtx[rb->id]);
		
		// 해제
		pthread_mutex_destroy(&g_mtx[rb->id]);
		pthread_cond_destroy(&g_cv[rb->id]);
	}
}

#endif		__linux