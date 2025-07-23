#ifndef SAFE_RING_BUFFER_H_
#define SAFE_RING_BUFFER_H_

#include <memory>
#include <atomic>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

class CSafeRingBuffer
{
public:
	explicit CSafeRingBuffer(unsigned long long max_buf_size = 1024 * 1024, unsigned char id = 0);
	// 5�� ��Ģ
	virtual ~CSafeRingBuffer();
	CSafeRingBuffer(const CSafeRingBuffer&) = delete;											 // ���� ������ ����
	CSafeRingBuffer& operator=(const CSafeRingBuffer&) = delete;							 // ���� ���� ������ ����
	CSafeRingBuffer(CSafeRingBuffer&&) = delete;													 // �̵� ������ ����
	CSafeRingBuffer& operator=(CSafeRingBuffer&&) = delete;								// �̵� ���� ������ ����

	void write(const unsigned char* data, unsigned long long size);
	unsigned long long read(std::unique_ptr<unsigned char[]>& out);
	unsigned long long wait_read(std::unique_ptr<unsigned char[]>& out);
	unsigned long long read_bytes(std::unique_ptr<unsigned char[]>& out, unsigned long long read_size);
	unsigned char read_byte();

	unsigned long long current_size();
	void reset();

private:
	unsigned long long m_capacity;
	unsigned long long m_head;
	unsigned long long m_tail;
	unsigned char m_id;
	unsigned char* m_buffer;

	std::atomic_bool m_running;

#ifdef _WIN32
	CRITICAL_SECTION m_cs;
	CONDITION_VARIABLE m_cv;
#else
	pthread_mutex_t m_mtx;
	pthread_cond_t m_cv;
#endif

};

#endif