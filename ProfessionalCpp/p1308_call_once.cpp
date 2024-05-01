import "HeaderUnits.h";
// std::call_once : ���� �����忡�� �����ϴ� �� �ϳ��� �����常 ����ϰ� �� �� (std::once_flag �� ���� ���)

std::once_flag g_onceFlag;

void init1308()
{
	// ���� �����尡 ����� ���� ���ҽ��� �ʱ�ȭ�Ѵ�
	printf("���� �ڿ� �ʱ�ȭ��\n");
}

// ���� �����忡�� ���������� ����� �Լ�
void processing1308()
{
	// ���� ���ҽ��� �ݵ�� �ʱ�ȭ�ϵ� �ѹ���...
	std::call_once(g_onceFlag, init1308);			// ���� �����尡 �־ �ѹ��� ����

	// ���ϴ� �۾��� �����Ѵ� (���� ���ҽ� ���)
	printf("Processing\n");
}

int main1308()
{
	// ������ 3���� ����
	std::thread threads[3];
	for (auto& t : threads)
	{
		t = std::thread{ processing1308 };			// ������ ����
	}

	// �� �����忡 ���� join�� ȣ���ؼ� ���ξ������� ������ �����
	for (auto& t : threads)
	{
		t.join();
	}

	return 0;
}
