#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <chrono>
#include <queue>
#include <functional>
#include <vector>
#include <future>
#include <numeric>
#include <type_traits>

template<typename T>
class CSafeQueue
{
	std::mutex m_mtx;
	std::condition_variable m_cv;
	std::queue<std::shared_ptr<T>> m_queue;

public:
	CSafeQueue() {}
	CSafeQueue(CSafeQueue const& other_queue) {
		std::lock_guard<std::mutex> lg(other_queue.m_mtx);
		m_queue = other_queue.m_queue;
	}
	CSafeQueue& operator=(CSafeQueue const& other_queue) = delete;
	CSafeQueue(CSafeQueue&& other_queue) noexcept {
		std::lock_guard<std::mutex> lg(other_queue.m_mtx);
		m_queue = std::move(other_queue.m_queue);
	}
	CSafeQueue& operator=(CSafeQueue&& other_queue) noexcept {
		if (this != &other_queue) {
			std::lock_guard<std::mutex> lg(m_mtx);
			std::lock_guard<std::mutex> lg_other(other_queue.m_mtx);
			m_queue = std::move(other_queue.m_queue);
		}
		return *this;
	}

	void push(const T& value) {
		std::lock_guard<std::mutex> lg(m_mtx);
		m_queue.push(std::make_shared<T>(value));
		m_cv.notify_one();
	}

	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lg(m_mtx);
		if (!m_queue.empty()) {
			std::shared_ptr<T> ref(m_queue.front());
			m_queue.pop();
			return ref;
		} else {
			return std::shared_ptr<T>();
		}
	}

	bool empty() {
		std::lock_guard<std::mutex> lg(m_mtx);
		return m_queue.empty();
	}

	std::shared_ptr<T> wait_pop() {
		std::unique_lock<std::mutex> lg(m_mtx);
		m_cv.wait(lg, [this] {
			return !m_queue.empty();
		});
		std::shared_ptr<T> ref = m_queue.front();
		m_queue.pop();
		return ref;
	}

	size_t size() {
		std::lock_guard<std::mutex> lg(m_mtx);
		return m_queue.size();
	}

	bool wait_pop(T& ref, long long timeout_ms = 20000) {
		std::unique_lock<std::mutex> lg(m_mtx);
		auto timeout_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);		// 타임아웃 20초
		if (!m_cv.wait_until(lg, timeout_time, [this] {
			return !m_queue.empty();		// wait_until : 절대시간, wait_for : 시스템 상대시간
			})) {
			return false; // 타임아웃 발생 시 false 반환
		}
		ref = *(m_queue.front());
		m_queue.pop();
		return true;
	}

	bool pop(T& ref) {
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

	std::shared_ptr<T> try_pop() {
		std::lock_guard<std::mutex> lg(m_mtx);
		if (m_queue.empty()) {
			return std::shared_ptr<T>();
		}
		std::shared_ptr<T> res = m_queue.front();
		m_queue.pop();
		return res;
	}

	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lg(m_mtx);
		if (m_queue.empty()) {
			return false;
		}
		value = *m_queue.front();
		m_queue.pop();
		return true;
	}
};

class CJoinThreads {
	std::vector<std::thread>& m_threads;

public:
	explicit CJoinThreads(std::vector<std::thread>& _threads)
		: m_threads(_threads)
	{}

	~CJoinThreads() {
		for (size_t i = 0; i < m_threads.size(); ++i) {
			if (m_threads[i].joinable()) {
				m_threads[i].join();
			}
		}
	}
};

class function_wrapper {
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
	};

	std::unique_ptr<impl_base> impl;
public:
	template<typename F>
	function_wrapper(F&& f)
		: impl(new impl_type<F>(std::move(f))) 
	{}

	void operator()() { impl->call(); }

	function_wrapper() {}

	function_wrapper(function_wrapper&& other) 
		: impl(std::move(other.impl))
	{}
	function_wrapper& operator=(function_wrapper&& other) {
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;
};

class CSimpleThreadPool
{
	std::atomic_bool m_done;
	CSafeQueue<std::function<void()>> m_work_queue;
	std::vector<std::thread> m_threads;
	CJoinThreads m_joiner;

	void worker_thread() {
		while (!m_done) {
			std::function<void()> task;
			if (m_work_queue.wait_pop(task)) {
				task();
			}
		}
	}

public:
	CSimpleThreadPool(int _thread_count = std::thread::hardware_concurrency())
		: m_done(false), m_joiner(m_threads) 
	{
		printf("thread count : %d\n", _thread_count);

		try {
			for (int i = 0; i < _thread_count; i++) {
				m_threads.push_back(std::thread(&CSimpleThreadPool::worker_thread, this));
			}
		}
		catch (...) {
			m_done = true;
			throw;
		}
	}

	~CSimpleThreadPool() {
		m_done = true;

		for (size_t i = 0; i < m_threads.size(); ++i) {
			m_work_queue.push([] {});			// 더미 작업 추가
		}

		// 스레드가 종료되기를 기다립니다.
		for (auto& thread : m_threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	template<typename Function_type>
	std::future<typename std::invoke_result<Function_type>::type> submit(Function_type f)
	{
		typedef typename std::invoke_result<Function_type>::type result_type;
		auto task = std::make_shared<std::packaged_task<result_type()>>(std::move(f));
		std::future<result_type> res = task->get_future();

		m_work_queue.push([task]() { (*task)(); });

		return res;
	}
};

CSafeQueue<int> g_queue1;
int g_value = 0;

// 빼서 쓰기
void func82_pop500() {

	while (true) {
		bool pop_result = g_queue1.wait_pop(g_value);
		if (pop_result == true)
		{
			printf("pop: %d, size:%llu\n", g_value, g_queue1.size());
			Sleep(500);				// 작업 처리 시간 0.5초
			if (g_value >= 19) {
				break;			// pop 종료 조건
			}
		}
		else if(g_value >= 19){
			printf("timeout wait_pop and break\n");
			break;
		}
		else {
			printf("timeout wait_pop\n");
		}
	}
	printf("finish pop\n");
}

// 넣기
void func82_push100() {
	static int x = 0;

	// 20까지만
	while (x < 20) {
		g_queue1.push(x);
		printf("pushing %d, size:%llu\n", x, g_queue1.size());
		x++;
		Sleep(100);				// 수신 시간 0.1초 주기
	}
	printf("finish push\n");
}

int main() {
	int thread_count = std::thread::hardware_concurrency() * 2 + 1;
	if (thread_count <= 1) {
		thread_count = 4;
	}

	CSimpleThreadPool pool(thread_count);
	
	for (int i = 0; i < 100; i++) {
		pool.submit([=] {
			printf(" %d printed by thread - %d \n", i, std::this_thread::get_id());
			Sleep(100);
		});
	}

	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_pop500);
	pool.submit(func82_push100);

	while (true) {
		std::string str;
		std::getline(std::cin, str);
		if (str == "") {
			break;
		}
	}
	system("pause");

	return 0;
}