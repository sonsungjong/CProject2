#ifndef COMMON_THREAD_SAFE_QUEUE_H_
#define COMMON_THREAD_SAFE_QUEUE_H_

#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <queue>

template<typename T>
class CCommonThreadSafeQueue
{
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::queue<std::shared_ptr<T>> m_queue;

public:
	CCommonThreadSafeQueue();
	CCommonThreadSafeQueue(CCommonThreadSafeQueue const& other_queue);
	CCommonThreadSafeQueue& operator=(CCommonThreadSafeQueue const& other_queue) = delete;
	CCommonThreadSafeQueue(CCommonThreadSafeQueue&& other_queue) noexcept;
	CCommonThreadSafeQueue& operator=(CCommonThreadSafeQueue&& other_queue) noexcept;

	void push(T const& value);
	std::shared_ptr<T> pop();
	bool empty();
	std::shared_ptr<T> wait_pop();
	size_t size();
	bool wait_pop(T& ref);
	bool pop(T& ref);
};


template<typename T>
CCommonThreadSafeQueue<T>::CCommonThreadSafeQueue()
{
}

template<typename T>
CCommonThreadSafeQueue<T>::CCommonThreadSafeQueue(CCommonThreadSafeQueue const& other_queue)
{
	std::lock_guard<std::mutex> lg(other_queue.m_mtx);
	m_queue = other_queue.m_queue;
}

template<typename T>
CCommonThreadSafeQueue<T>::CCommonThreadSafeQueue(CCommonThreadSafeQueue&& other_queue) noexcept
{
	std::lock_guard<std::mutex> lg(other_queue.m_mtx);
	m_queue = std::move(other_queue.m_queue);
}

template<typename T>
CCommonThreadSafeQueue<T>& CCommonThreadSafeQueue<T>::operator=(CCommonThreadSafeQueue&& other_queue) noexcept
{
	if (this != &other_queue) {
		std::lock_guard<std::mutex> lg(m_mtx);
		std::lock_guard<std::mutex> lg_other(other_queue.m_mtx);
		m_queue = std::move(other_queue.m_queue);
	}
	return *this;
}

template<typename T>
void CCommonThreadSafeQueue<T>::push(T const& value)
{
	std::lock_guard<std::mutex> lg(m_mtx);
	m_queue.push(std::make_shared<T>(value));
	m_cv.notify_one();
}

template<typename T>
std::shared_ptr<T> CCommonThreadSafeQueue<T>::pop()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	if (!m_queue.empty()) {
		std::shared_ptr<T> ref(m_queue.front());
		m_queue.pop();
		return ref;
	}
	else {
		return std::shared_ptr<T>();
	}
}

template<typename T>
bool CCommonThreadSafeQueue<T>::empty()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	return m_queue.empty();
}

template<typename T>
std::shared_ptr<T> CCommonThreadSafeQueue<T>::wait_pop()
{
	std::unique_lock<std::mutex> lg(m_mtx);
	m_cv.wait(lg, [this] {
		return !m_queue.empty();
		});
	std::shared_ptr<T> ref = m_queue.front();
	m_queue.pop();
	return ref;
}

template<typename T>
size_t CCommonThreadSafeQueue<T>::size()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	return m_queue.size();
}

template<typename T>
bool CCommonThreadSafeQueue<T>::wait_pop(T& ref)
{
	std::unique_lock<std::mutex> lg(m_mtx);
	m_cv.wait(lg, [this] {
		return !m_queue.empty();
		});
	ref = *(m_queue.front());
	m_queue.pop();
	return true;
}

template<typename T>
bool CCommonThreadSafeQueue<T>::pop(T& ref)
{
	std::lock_guard<std::mutex> lg(m_mtx);
	if (!m_queue.empty()) {
		ref = *(m_queue.front());
		m_queue.pop();
		return true;
	}
	else {
		return false;
	}
}



#endif		COMMON_THREAD_SAFE_QUEUE_H_


