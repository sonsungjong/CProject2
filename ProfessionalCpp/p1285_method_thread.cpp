import "HeaderUnits.h";

class Request1285
{
public:
	Request1285(int id)
		: m_id(id)
	{
	}

	void process()
	{
		printf("Processing Request %d\n", m_id);
	}

private:
	int m_id;
};

int main1285()
{
	Request1285 req(100);

	// 메서드를 쓰레드로 실행시키는 방법
	// Request 인스턴스인 req의 process() 메서드를 실행하는 스레드를 생성한다
	std::thread th(&Request1285::process, &req);

	th.join();

	return 0;
}
