import "HeaderUnits.h";
#include <semaphore>
#include <thread>
#include <vector>

// ���� ������ ��ü
// acquire() �� ȣ���ϴ� ������� �� ������� ���� release()�� ȣ���ϴ� �����尡 ��Ÿ�������� ��ϵȴ�

// acquire() : ī���͸� ���ҽ�Ų��. ī���Ͱ� 0�� �Ǹ� �ٽ� ������ ������ ��ϵȴ�
// release() : ������ ����ŭ ī���͸� ������Ű�� acquire() ȣ�⿡ ���� ��ϵ� �����带 �����Ѵ�

int main1321()
{
	std::counting_semaphore semaphore(4);
	std::vector<std::jthread> threads;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(std::jthread([&semaphore] {
			semaphore.acquire();

			// ������ ���� ���� (�ִ� �� ������ ���� ���� ����)
			semaphore.release();
		}));
	}

	return 0;
}