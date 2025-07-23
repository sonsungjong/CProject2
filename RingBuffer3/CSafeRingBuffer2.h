#ifndef SAFE_RING_BUFFER2_H_
#define SAFE_RING_BUFFER2_H_

#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

class CSafeRingBuffer
{
public:
	explicit CSafeRingBuffer(unsigned long long max_buf_size = 1024 * 1024, unsigned char id = 0);
	// 5의 법칙
	virtual ~CSafeRingBuffer();
	CSafeRingBuffer(const CSafeRingBuffer&) = delete;											 // 복사 생성자 금지
	CSafeRingBuffer& operator=(const CSafeRingBuffer&) = delete;							 // 복사 대입 연산자 금지
	CSafeRingBuffer(CSafeRingBuffer&&) = delete;													 // 이동 생성자 금지
	CSafeRingBuffer& operator=(CSafeRingBuffer&&) = delete;								// 이동 대입 연산자 금지

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
	std::mutex m_mutex;
	std::condition_variable m_cv;

};

#endif			// SAFE_RING_BUFFER2_H_