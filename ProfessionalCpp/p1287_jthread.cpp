import "HeaderUnits.h";

/*
std::jthread : �Ҹ��ڿ��� �ڵ����� ����, ������ ��� ����
std::stop_token : stop_requested() �� �ֱ������� ȣ���ؼ� ����ؾ��ϴ��� Ȯ��
std::stop_source : stop_requested() �� ���� ��û�� �̹� ���Դ��� �˻�
get_stop_token()
get_stop_source()

*/

int main1287()
{
	std::jthread job([](stop_token token) {
		while (!token.stop_requested())
		{

		}
	});

	// �ٸ� �����忡�� job.request_stop(); �� ���� ��ҿ�û�� �� �� �ִ�


	return 0;
}