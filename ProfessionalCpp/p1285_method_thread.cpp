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

	// �޼��带 ������� �����Ű�� ���
	// Request �ν��Ͻ��� req�� process() �޼��带 �����ϴ� �����带 �����Ѵ�
	std::thread th(&Request1285::process, &req);

	th.join();

	return 0;
}
