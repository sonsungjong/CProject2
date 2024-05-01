import "HeaderUnits.h";
// std::call_once : 여러 쓰레드에서 공유하는 중 하나의 쓰레드만 사용하게 할 때 (std::once_flag 와 같이 사용)

std::once_flag g_onceFlag;

void init1308()
{
	// 여러 스레드가 사용할 공유 리소스를 초기화한다
	printf("공유 자원 초기화됨\n");
}

// 여러 쓰레드에서 공통적으로 사용할 함수
void processing1308()
{
	// 공유 리소스는 반드시 초기화하되 한번만...
	std::call_once(g_onceFlag, init1308);			// 여러 쓰레드가 있어도 한번만 사용됨

	// 원하는 작업을 수행한다 (공유 리소스 사용)
	printf("Processing\n");
}

int main1308()
{
	// 스레드 3개를 구동
	std::thread threads[3];
	for (auto& t : threads)
	{
		t = std::thread{ processing1308 };			// 스레드 생성
	}

	// 각 스레드에 대해 join을 호출해서 메인쓰레드의 진행을 멈춘다
	for (auto& t : threads)
	{
		t.join();
	}

	return 0;
}
