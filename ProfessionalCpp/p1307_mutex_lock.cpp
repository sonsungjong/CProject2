import "HeaderUnits.h";
#include <mutex>

std::mutex g_mutex1;
std::mutex g_mutex2;

void process1307()
{
	// defer_lock : �����ڿ��� �ٷ� ����� �ʰ� ��ٸ��� �÷���
	std::unique_lock lock1 { g_mutex1, std::defer_lock };
	std::unique_lock lock2 { g_mutex2, std::defer_lock };
	std::lock(lock1, lock2);			// ������� �����ϸ鼭 ���ÿ� ��ٴ�
	// ���� �Ǵ�
}			// ���� �ڵ����� �����ȴ�

void scopedProcess1307()
{
	std::scoped_lock locks{ g_mutex1, g_mutex2 };			// ���� ���� �� �� ȿ����
	// ���� �Ǵ�
}		// ���� �ڵ����� �����ȴ�

int main1307()
{
	process1307();
	scopedProcess1307();

	return 0;
}