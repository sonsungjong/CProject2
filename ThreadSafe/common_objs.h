#pragma once

#include <vector>
#include <thread>

class thread_guard {
	std::thread& m_thread;

public:
	explicit thread_guard(std::thread& _t) : m_thread(_t)
	{}

	~thread_guard() {
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	thread_guard(thread_guard& const) = delete;
	thread_guard& operator= (thread_guard& const) = delete;
};

class join_threads {
	std::vector<std::thread>& m_threads;

public:
	explicit join_threads(std::vector<std::thread>& _threads)
		: m_threads(_threads)
	{}

	~join_threads(){
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
	function_wrapper(){}

	function_wrapper(function_wrapper&& other) noexcept
		: impl(std::move(other.impl))
	{}

	function_wrapper& operator=(function_wrapper&& other) noexcept
	{
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;
};