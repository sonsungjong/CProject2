import "HeaderUnits.h";

int k1286;											// ��������
thread_local int n1286;				// �� �����忡�� ������ �����ذ� ����

void threadFunction(int _id)
{
	printf("Thread %d: k=%d, n=%d\n", _id, k1286, n1286);
	++n1286;
	++k1286;
}

int main1286()
{
	std::thread th1(threadFunction, 1);
	th1.join();					// k=0, n=0

	std::thread th2(threadFunction, 2);
	th2.join();					// k=1, n=0

	return 0;
}