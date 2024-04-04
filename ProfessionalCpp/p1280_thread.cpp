import "HeaderUnits.h";

// 전역 함수
void counter(int id, int numIterations)
{
	for (int i = 0; i < numIterations; ++i)
	{
		printf("Counter %d has value %d\n", id, i);
	}
}

// 함수 객체
class Counter1283
{
public:
	Counter1283(int _id, int _numIterations)
		: m_id(_id), m_numIterations(_numIterations)
	{
	}

	void operator()() const
	{
		for (int i = 0; i < m_numIterations; ++i)
		{
			printf("Counter %d has value %d\n", m_id, i);
		}
	}

private:
	int m_id;
	int m_numIterations;
};

int main1280()
{
	std::thread th1(counter, 1, 6 );			// 가변인수 템플릿기반 std::thread
	// 첫번째 인수는 새로 생성한 스레드에서 실행시킬 함수의 이름
	// 그 뒤의 인수는 실행할 함수에 전달할 인수들
	std::thread th2{ counter, 2, 4};

	th1.join();
	th2.join();

	// ===== Block ======= join끼리만 같이 실행될 수 있음

	std::thread th3(Counter1283(3, 20));
	Counter1283 counter(4, 12);
	thread th4(counter);

	th3.join();				// 같은 구간내에서 join끼리는 경쟁
	th4.join();				// 같은 구간내에서 join끼리는 경쟁

	// ===== Block ======= join끼리만 같이 실행될 수 있음

	return 0;
}