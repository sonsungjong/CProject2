#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <queue>

// 멀티스레드안전 큐
template<typename T>
class CThreadSafeQueue {
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::queue<std::shared_ptr<T>> m_queue;

public:
    CThreadSafeQueue() {}

    CThreadSafeQueue(CThreadSafeQueue const& other_queue) {
        std::lock_guard<std::mutex> lg(other_queue.m_mtx);
        m_queue = other_queue.m_queue;
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
        }
        else {
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

    bool wait_pop(T& ref) {
        std::unique_lock<std::mutex> lg(m_mtx);
        m_cv.wait(lg, [this] {
            return !m_queue.empty();
            });
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
};

CThreadSafeQueue<int> queue1;

void func1_1() {
    static int value = 0;

    // 20까지만
    while (value < 20) {
        queue1.wait_pop(value);
        printf("pop: %d\n", value);
    }
    printf("finish pop\n");
}

void func1_2() {
    static int x = 10;

    // 20까지만
    while (x < 20) {
        queue1.push(++x);
        printf("pushing!\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    printf("finish push\n");
}

int main39()
{
    std::thread th1(func1_1);             // pop쓰레드
    std::thread th2(func1_2);             // push 쓰레드

    th1.join();
    th2.join();

    return 0;
}
