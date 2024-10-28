#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
                });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& thread : threads_) {
            thread.join();
        }
    }

    template <typename F>
    auto submit(F&& f) -> std::future<decltype(f())> {
        auto task = std::make_shared<std::packaged_task<decltype(f())()>>(std::forward<F>(f));
        std::future<decltype(f())> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx_);
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        return result;
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;
};

int main() {
    ThreadPool pool(4);

    // Submit tasks
    auto future1 = pool.submit([] { std::cout << "\nTask 1 executed\n"; });
    auto future2 = pool.submit([] { std::cout << "\nTask 2 executed\n"; });
    auto future3 = pool.submit([] { std::cout << "\nTask 3 executed\n"; });

    // Wait for tasks to finish
    future1.get();
    future2.get();
    future3.get();

    return 0;
}
