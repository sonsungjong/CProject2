#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>
#include <chrono>

// 큐없이 구현한 멀티스레드안전 큐
template<typename T>
class CSequentialQueue{
private:
	struct node {
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;

		node() {}

		node(T _data) : data(std::move(_data)){}
	};

	std::unique_ptr<node> m_head;
	node* m_tail;
	std::mutex m_head_mutex;
	std::mutex m_tail_mutex;
	std::condition_variable m_cv;

	node* getTail() {
		std::lock_guard<std::mutex> lg(m_tail_mutex);
		return m_tail;
	}

	std::unique_ptr<node> waitPopHead() {
		std::unique_lock<std::mutex> lock(m_head_mutex);

		m_cv.wait(lock, [&] {return m_head.get() != getTail(); });

		std::unique_ptr<node> old_head = std::move(m_head);

		m_head = std::move(old_head->next);
		return old_head;
	}

public:
	CSequentialQueue()
		: m_head(new node)
		, m_tail(m_head.get())
	{}

	void push(T value){
		// 현재 더미노드를 value로 바꾼다
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
		std::unique_ptr<node> p(new node);

		node* const new_tail = p.get();
		{
			std::lock_guard<std::mutex> lg(m_tail_mutex);
			m_tail->data = new_data;
			m_tail->next = std::move(p);
			m_tail = new_tail;
		}
		
		m_cv.notify_one();
	}

	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lg(m_head_mutex);
		if (m_head.get() == getTail()) {
			return std::shared_ptr<T>();
		}
		std::shared_ptr<T> const res(m_head->data);
		std::unique_ptr<node> const old_head = std::move(m_head);
		m_head = std::move(old_head->next);
		return res;
	}

	std::shared_ptr<T> waitPop() {
		std::unique_ptr<node> old_head = waitPopHead();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}

	bool waitPop(T& ref) {
		std::unique_ptr<node> old_head = waitPopHead();
		ref = *(old_head->data.get());
		return true;
	}

	void printData();
};

template<typename T>
inline void CSequentialQueue<T>::printData() {
	if (m_head.get() == getTail()) {
		printf("the Queue is empty\n");
		return;
	}

	std::lock_guard<std::mutex> lg(m_head_mutex);
	node* current = m_head.get();
	int index{};
	while (current->data != nullptr) {
		std::cout << "current: " << current << ", value [" << index++ << "]: " << *(current->data) << std::endl;
		current = (current->next).get();
	}
	printf("==== end of the queue====\n");
}

CSequentialQueue<int> queueInteger;

void func2_1() {
	static int value = 0;

	// 20까지만
	while (value < 20) {
		//value = *queueInteger.waitPop().get();
		queueInteger.waitPop(value);
		printf("pop: %d\n", value);
	}
	printf("finish pop\n");
}

void func2_2() {
	static int x = 10;

	// 20까지만
	while (x < 20) {
		queueInteger.push(++x);
		printf("pushing!\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	printf("finish push\n");
}

int main2()
{
	queueInteger.push(1111);
	queueInteger.push(2222);
	queueInteger.push(3333);
	queueInteger.push(4444);
	queueInteger.push(5555);

	queueInteger.printData();

	std::cout << "Removing: " << *(queueInteger.pop().get()) << std::endl;
	std::cout << "Removing: " << *(queueInteger.pop().get()) << std::endl;
	std::cout << "Removing by wait_pop(): " << *(queueInteger.waitPop().get()) << std::endl;
	queueInteger.printData();

	std::cout << "Removing by wait_pop(): " << *(queueInteger.waitPop().get()) << std::endl;
	queueInteger.printData();

	std::cout << "Removing: " << *(queueInteger.pop().get()) << std::endl;
	queueInteger.printData();

	std::thread th1(func2_1);				// pop쓰레드
	std::thread th2(func2_2);				// push쓰레드

	th1.join();
	th2.join();

	return 0;
}