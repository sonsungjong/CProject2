#ifndef RING_BUFFER_1_H_
#define RING_BUFFER_1_H_

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct STCircularQueue {
        unsigned char* data;                         // ��ҵ��� ������ �޸� ���� (����Ʈ �迭)
        int max_capacity;                       // ���� ������ �ִ� ��� ����
        int head;               // �б� �ε���
        int tail;                   // ���� �ε���
        int curSize;                // ���� ����� ������ ũ��
        unsigned char id;                     // ����ȭ�ϴ� ���� ���̵�
    } ST_CircularQueue;

    void init_RingBuffer(ST_CircularQueue* q, int _max_count, int _element_size, unsigned char _id);			// ��ȯť �ʱ�ȭ
    int size_RingBuffer(ST_CircularQueue* q);									// ť�� ����ũ��
    int isFull_RingBuffer(ST_CircularQueue* q);									// ť�� ���� á���� �Ǵ�
    int isEmpty_RingBuffer(ST_CircularQueue* q);								// ť�� ������� �Ǵ�
    void enqueue_RingBuffer(ST_CircularQueue* q, void* new_element);							// ť�� �����͸� �߰�
    void* dequeue_RingBuffer(ST_CircularQueue* q);										// ť���� �����͸� �����ϸ鼭 ��ȯ
    void* front_RingBuffer(ST_CircularQueue* q);										// ���������ʰ� �����͸� Ȯ�θ��Ѵ�
    void pop_RingBuffer(ST_CircularQueue* q);										// ���� �պκ��� ���Ÿ� �Ѵ� (�������� ����)
    void reset_RingBuffer(ST_CircularQueue* q);									// ť�� �ʱ�ȭ
    void destroy_RingBuffer(ST_CircularQueue* q);						// ť�� ����
    void testDisplay_RingBuffer(ST_CircularQueue* q);			// �׽�Ʈ��

#ifdef __cplusplus
}
#endif

#endif			RING_BUFFER_1_H_