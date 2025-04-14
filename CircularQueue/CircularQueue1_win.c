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
    q->front = -1;                // ť�� ��������� ��Ÿ���� �ε����� ����
    q->rear = -1;                // ť�� ��������� ��Ÿ���� �ε����� ����
    q->id = _id;                // ť�� ��������� ��Ÿ���� �ε����� ����

    if (_max_count > 0) {
        q->max_count = _max_count;                // �ִ� ���� ���� (�Ѿ�� ���)
    }
    if (_element_size > 0) {
        q->element_size = _element_size;           // ��� �ڷ����� ũ��
    }

    // �޸� �Ҵ�
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

// ť�� ���� á���� 1(true), �ƴϸ� 0(false) ��ȯ
int isFull_CircularQueue(ST_CircularQueue* q)
{
    // 1. front�� 0�̰� rear�� ������ �ε����� ��� 
    // 2. front�� rear�� �ٷ� ���� �ε����� ��� (��ȯ ����)
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
        // ���� ������ ���
        if (isEmpty_CircularQueue(q)) {
            q->front = 0;
            q->rear = 0;
        }
        else {
            // ��ȯ ó��: rear�� �������̸� 0����
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
        printf("ť�� ���� á���ϴ�. ����ڽ��ϴ�\n");

        // ���� ����� �������� else �� �ڵ带 ��� �ּ�ó��
        if (q->front == q->max_count - 1) {
            q->front = 0;

        }
        else {
            q->front++;

        }
        // rear ��ġ�� ����� ����� ��ġ�� �ȴ�.
        if (q->rear == q->max_count - 1) {
            q->rear = 0;

        }
        else {
            q->rear++;

        }
        memcpy((char*)q->data + q->rear * q->element_size, new_element, q->element_size);
    }
}

// ��������� ���� ��ȯ�ϸ鼭 �����Ѵ�
void* dequeue_CircularQueue(ST_CircularQueue* q)
{
    void* deep_copy = NULL;

    if (isEmpty_CircularQueue(q) == 0)
    {
        deep_copy = malloc(q->element_size);

        memcpy(deep_copy,  (char*)q->data + q->front * q->element_size, q->element_size);

        // ť�� ��Ұ� �� �� ���� ���� ���
        if (q->front == q->rear) {
            q->front = -1;
            q->rear = -1;
        }
        else {
            // ��ȯ ó��: front�� ������ �ε����̸� 0����
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

// ���� ����� �տ��� Ȯ���Ѵ�
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
        // ť�� ��Ұ� �� �� ���� ���� ���
        if (q->front == q->rear) {
            q->front = -1;
            q->rear = -1;
        }
        else {
            // ��ȯ ó��: front�� ������ �ε����̸� 0����
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
                // q->data�� void*�̹Ƿ� int*�� ĳ���� �� �� ��� ���
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
                // q->data�� void*�̹Ƿ� int*�� ĳ���� �� �� ��� ���
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
                // q->data�� void*�̹Ƿ� unsgiend char*�� ĳ���� �� �� ��� ���
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