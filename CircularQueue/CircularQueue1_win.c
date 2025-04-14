#ifdef _WIN32
#include "CircularQueue1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <Windows.h>

void init_CircularQueue(ST_CircularQueue* q, int _max_count, int _element_size, unsigned char _id)
{
    q->data = NULL;
    q->max_count = 100;
    q->element_size = (int)(sizeof(unsigned char));
    q->front = -1;                // 큐가 비어있음을 나타내는 인덱스로 설정
    q->rear = -1;                // 큐가 비어있음을 나타내는 인덱스로 설정
    q->id = _id;                // 큐가 비어있음을 나타내는 인덱스로 설정

    if (_max_count > 0) {
        q->max_count = _max_count;                // 최대 저장 개수 (넘어가면 덮어씀)
    }
    if (_element_size > 0) {
        q->element_size = _element_size;           // 대상 자료형의 크기
    }

    // 메모리 할당
    q->data = malloc(q->max_count * q->element_size);
}

int size_CircularQueue(ST_CircularQueue* q) {
    int nResult = 0;

    if (isEmpty_CircularQueue(q) == 0)
    {
        if (q->rear >= q->front) {
            nResult = q->rear - q->front + 1;
        }
        else {
            nResult = q->max_count - q->front + q->rear + 1;

        }
    }
    return nResult;
}

// 큐가 가득 찼으면 1(true), 아니면 0(false) 반환
int isFull_CircularQueue(ST_CircularQueue* q)
{
    // 1. front가 0이고 rear가 마지막 인덱스인 경우 
    // 2. front가 rear의 바로 다음 인덱스인 경우 (순환 구조)
    int nResult = 0;
    if ((q->front == 0 && q->rear == q->max_count - 1) || (q->front == q->rear + 1))
    {
        nResult = 1;
    }
    return nResult;
}
int isEmpty_CircularQueue(ST_CircularQueue* q)
{
    int nResult = 0;
    if (q->front == -1) {
        nResult = 1;
    }

    return nResult;
}

void enqueue_CircularQueue(ST_CircularQueue* q, const void* new_element)
{
    if (isFull_CircularQueue(q) == 0) {
        // 최초 삽입인 경우
        if (isEmpty_CircularQueue(q)) {
            q->front = 0;
            q->rear = 0;
        }
        else {
            // 순환 처리: rear가 마지막이면 0으로
            if (q->rear == q->max_count - 1) {
                q->rear = 0;
            }
            else {
                q->rear = q->rear + 1;
            }
        }
        memcpy((char*)q->data + q->rear * q->element_size, new_element, q->element_size);
    }
    else {
        printf("큐가 가득 찼습니다. 덮어쓰겠습니다\n");

        // 만약 덮어쓰지 않으려면 else 의 코드를 모두 주석처리
        if (q->front == q->max_count - 1) {
            q->front = 0;

        }
        else {
            q->front++;

        }
        // rear 위치는 덮어쓰기 대상의 위치가 된다.
        if (q->rear == q->max_count - 1) {
            q->rear = 0;

        }
        else {
            q->rear++;

        }
        memcpy((char*)q->data + q->rear * q->element_size, new_element, q->element_size);
    }
}

// 깊은복사로 값을 반환하면서 제거한다
void* dequeue_CircularQueue(ST_CircularQueue* q)
{
    void* deep_copy = NULL;

    if (isEmpty_CircularQueue(q) == 0)
    {
        deep_copy = malloc(q->element_size);

        memcpy(deep_copy,  (char*)q->data + q->front * q->element_size, q->element_size);

        // 큐에 요소가 단 한 개만 있을 경우
        if (q->front == q->rear) {
            q->front = -1;
            q->rear = -1;
        }
        else {
            // 순환 처리: front가 마지막 인덱스이면 0으로
            if (q->front == q->max_count - 1) {
                q->front = 0;
            }
            else {
                q->front = q->front + 1;
            }
        }
    }

    return deep_copy;
}

// 얕은 복사로 앞에만 확인한다
void* front_CircularQueue(ST_CircularQueue* q)
{
    void* leak_copy = NULL;
    if (isEmpty_CircularQueue(q) == 0)
    {
        leak_copy = (char*)q->data + q->front * q->element_size;
    }
    return leak_copy;
}

void pop_CircularQueue(ST_CircularQueue* q)
{
    if (isEmpty_CircularQueue(q) == 0)
    {
        // 큐에 요소가 단 한 개만 있을 경우
        if (q->front == q->rear) {
            q->front = -1;
            q->rear = -1;
        }
        else {
            // 순환 처리: front가 마지막 인덱스이면 0으로
            if (q->front == q->max_count - 1) {
                q->front = 0;
            }
            else {
                q->front = q->front + 1;
            }
        }
    }
}

void reset_CircularQueue(ST_CircularQueue* q)
{
    q->front = -1;
    q->rear = -1;
}

void destroy_CircularQueue(ST_CircularQueue* q)
{
    q->front = -1;
    q->rear = -1;
    if (q->data != NULL) {
        free(q->data);
        q->data = NULL;
    }
}

void testDisplay_CircularQueue(ST_CircularQueue* q)
{
    int i;
    printf("front = %d, rear = %d\n", q->front, q->rear);

    if (isEmpty_CircularQueue(q) == 1)
    {
        printf("Queue is empty\n");
        return;
    }

    if (q->element_size == sizeof(int))
    {
        printf("Queue<int> is :");

        i = q->front;
        if (q->front <= q->rear)
        {
            while (i <= q->rear)
            {
                // q->data는 void*이므로 int*로 캐스팅 후 각 요소 출력
                printf("%d ", ((int*)q->data)[i]);
                i++;
            }
        }
        else
        {
            while (i < q->max_count)
            {
                printf("%d ", ((int*)q->data)[i]);
                i++;
            }
            i = 0;
            while (i <= q->rear)
            {
                printf("%d ", ((int*)q->data)[i]);
                i++;
            }
        }
        printf("\n");
    }
    else if (q->element_size == sizeof(double))
    {
        printf("Queue<double> is :");

        i = q->front;
        if (q->front <= q->rear)
        {
            while (i <= q->rear)
            {
                // q->data는 void*이므로 int*로 캐스팅 후 각 요소 출력
                printf("%lf ", ((double*)q->data)[i]);
                i++;
            }
        }
        else
        {
            while (i < q->max_count)
            {
                printf("%lf ", ((double*)q->data)[i]);
                i++;
            }
            i = 0;
            while (i <= q->rear)
            {
                printf("%lf ", ((double*)q->data)[i]);
                i++;
            }
        }
        printf("\n");
    }
    else if (q->element_size == sizeof(unsigned char))
    {
        printf("Queue<unsigned char> is :");

        i = q->front;
        if (q->front <= q->rear)
        {
            while (i <= q->rear)
            {
                // q->data는 void*이므로 unsgiend char*로 캐스팅 후 각 요소 출력
                printf("%c ", ((unsigned char*)q->data)[i]);
                i++;
            }
        }
        else
        {
            while (i < q->max_count)
            {
                printf("%c ", ((unsigned char*)q->data)[i]);
                i++;
            }
            i = 0;
            while (i <= q->rear)
            {
                printf("%c ", ((unsigned char*)q->data)[i]);
                i++;
            }
        }
        printf("\n");
    }
}


#endif          _WIN32