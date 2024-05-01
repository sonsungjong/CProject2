import "HeaderUnits.h";
#include <mutex>

std::mutex g_mutex1;
std::mutex g_mutex2;

void process1307()
{
	// defer_lock : 생성자에서 바로 잠기지 않고 기다리는 플래그
	std::unique_lock lock1 { g_mutex1, std::defer_lock };
	std::unique_lock lock2 { g_mutex2, std::defer_lock };
	std::lock(lock1, lock2);			// 데드락을 방지하면서 동시에 잠근다
	// 락을 건다
}			// 락이 자동으로 해제된다

void scopedProcess1307()
{
	std::scoped_lock locks{ g_mutex1, g_mutex2 };			// 여러 락을 걸 때 효율적
	// 락을 건다
}		// 락이 자동으로 해제된다

int main1307()
{
	process1307();
	scopedProcess1307();

	return 0;
}