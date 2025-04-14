#ifndef RING_BUFFER_1_H_
#define RING_BUFFER_1_H_

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct STCircularQueue {
        unsigned char* data;                         // 요소들을 저장할 메모리 영역 (바이트 배열)
        int max_capacity;                       // 저장 가능한 최대 요소 개수
        int head;               // 읽기 인덱스
        int tail;                   // 쓰기 인덱스
        int curSize;                // 현재 저장된 데이터 크기
        unsigned char id;                     // 동기화하는 기준 아이디
    } ST_CircularQueue;

    void init_RingBuffer(ST_CircularQueue* q, int _max_count, int _element_size, unsigned char _id);			// 순환큐 초기화
    int size_RingBuffer(ST_CircularQueue* q);									// 큐의 현재크기
    int isFull_RingBuffer(ST_CircularQueue* q);									// 큐가 가득 찼는지 판단
    int isEmpty_RingBuffer(ST_CircularQueue* q);								// 큐가 비었는지 판단
    void enqueue_RingBuffer(ST_CircularQueue* q, void* new_element);							// 큐에 데이터를 추가
    void* dequeue_RingBuffer(ST_CircularQueue* q);										// 큐에서 데이터를 삭제하면서 반환
    void* front_RingBuffer(ST_CircularQueue* q);										// 삭제하지않고 데이터를 확인만한다
    void pop_RingBuffer(ST_CircularQueue* q);										// 가장 앞부분을 제거만 한다 (깊은복사 안함)
    void reset_RingBuffer(ST_CircularQueue* q);									// 큐를 초기화
    void destroy_RingBuffer(ST_CircularQueue* q);						// 큐를 없앰
    void testDisplay_RingBuffer(ST_CircularQueue* q);			// 테스트용

#ifdef __cplusplus
}
#endif

#endif			RING_BUFFER_1_H_