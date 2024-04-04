import "HeaderUnits.h";

int main1284()
{
	int id = 1;
	int numIterations = 5;

	// lambda ǥ�������� ������ ����ϱ�
	std::thread th1([id, numIterations] {
		for (int i = 0; i < numIterations; ++i)
		{
			printf("Counter %d has value %d\n", id, i);
		}
	});

	th1.join();

	return 0;
}
