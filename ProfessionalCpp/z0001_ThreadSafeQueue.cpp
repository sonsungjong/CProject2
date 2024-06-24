#include <iostream>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

template<typename T>
class ThreadSafeQueue
{
	std::mutex m_mtx;
	std::condition_variable m_cv;
	std::queue<std::shared_ptr<T>> m_queue;

public:
	ThreadSafeQueue();
	ThreadSafeQueue(ThreadSafeQueue const& other_queue);
	ThreadSafeQueue& operator=(ThreadSafeQueue const& other_queue) = delete;
	ThreadSafeQueue(ThreadSafeQueue&& other_queue) noexcept;
	ThreadSafeQueue& operator=(ThreadSafeQueue&& other_queue) noexcept;

	void push(T const& value);
	std::shared_ptr<T> pop();
	bool empty();

	std::shared_ptr<T> wait_pop();
	size_t size();
	bool wait_pop(T& ref);
	bool pop(T& ref);
};

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue()
{
}

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue const& other_queue)
{
	std::lock_guard<std::mutex> lg(other_queue.m_mtx);
	m_queue = other_queue.m_queue;
}

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue&& other_queue) noexcept
{
	std::lock_guard<std::mutex> lg(other_queue.m_mtx);
	m_queue = std::move(other_queue.m_queue);
}

template<typename T>
ThreadSafeQueue<T>& ThreadSafeQueue<T>::operator=(ThreadSafeQueue&& other_queue) noexcept
{
	if (this != &other_queue) {
		std::lock_guard<std::mutex> lg(m_mtx);
		std::lock_guard<std::mutex> lg_other(other_queue.m_mtx);
		m_queue = std::move(other_queue.m_queue);
	}
	return *this;
}

template<typename T>
void ThreadSafeQueue<T>::push(T const& value)
{
	std::lock_guard<std::mutex> lg(m_mtx);
	m_queue.push(std::make_shared<T>(value));
	m_cv.notify_one();
}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	if (m_queue.empty())
	{
		return std::shared_ptr<T>();
	}
	else
	{
		std::shared_ptr<T> ref(m_queue.front());
		m_queue.pop();
		return ref;
	}
}

template<typename T>
bool ThreadSafeQueue<T>::empty()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	return m_queue.empty();
}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::wait_pop()
{
	std::unique_lock<std::mutex> lg(m_mtx);			// condition_variable을 사용할 땐 unique_lock을 사용한다
	m_cv.wait(lg, [this] { return !m_queue.empty(); });

	std::shared_ptr<T> ref = m_queue.front();
	m_queue.pop();
	return ref;
}

template<typename T>
size_t ThreadSafeQueue<T>::size()
{
	std::lock_guard<std::mutex> lg(m_mtx);
	return m_queue.size();
}

template<typename T>
bool ThreadSafeQueue<T>::wait_pop(T& ref)
{
	std::unique_lock<std::mutex> lg(m_mtx);
	m_cv.wait(lg, [this] { return !m_queue.empty(); });

	ref = *(m_queue.front());
	m_queue.pop();
	return true;
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& ref)
{
	std::lock_guard<std::mutex> lg(m_mtx);
	if (m_queue.empty())
	{
		return false;
	}
	else 
	{
		ref = *(m_queue.front());
		m_queue.pop();
		return true;
	}
}

void func_1(ThreadSafeQueue<int>& nQueue)
{
	int value;
	nQueue.wait_pop(value);
	printf("%d\n", value);
}

void func_2(ThreadSafeQueue<int>& nQueue)
{
	int x = 10;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	nQueue.push(x);			// wait_pop에 뽑아가라고 notify_one 한다
}

template<typename T>
void func_3(ThreadSafeQueue<T>& nQueue)
{
	T value;
	nQueue.wait_pop(value);
	std::cout << value << std::endl;
}

template<typename T>
void func_4(ThreadSafeQueue<T>& nQueue)
{
	T x = 20;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	nQueue.push(x);
}

int main()
{
	ThreadSafeQueue<int> nQueue;

	std::thread thread1([&nQueue] {func_1(nQueue); });
	std::thread thread2([&nQueue] {func_2(nQueue); });

	thread1.join();
	thread2.join();

	std::thread thread3(func_3<int>, std::ref(nQueue));
	std::thread thread4(func_4<int>, std::ref(nQueue));
	
	thread3.join();
	thread4.join();

	return 0;
}