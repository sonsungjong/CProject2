import "HeaderUnits.h";


// �����忡�� �߻��� �ͼ����� ó���ϴ� ���
void doSomeWork1289()
{
	for (int i = 0; i < 5; ++i)
	{
		printf("%d\n", i);
	}
	printf("Thread throwing a runtime_error exception...\n");
	throw std::runtime_error("Exception from thread");					// �ͼ����� ������ �Ѵ�
}

// ó���� �ͼ��ǿ� ���� ���۷����� �޾Ƽ� exception_ptr �Ű������� �����ϰ� ������� ���������� �����ϵ��� ����
void threadFunc1289(std::exception_ptr& err)
{
	try
	{
		doSomeWork1289();
	}
	catch (...)
	{
		printf("Thread caught exception, returning exception...\n");
		err = std::current_exception();				// ó���� �ͼ����� ���۷���(current_exception())�� ����!!
	}
}

void doWorkInThread1289()
{
	std::exception_ptr error;
	std::thread th(threadFunc1289, std::ref(error));
	th.join();

	// �����忡�� �ͼ����� �߻��ߴ��� �˻��Ѵ�
	if (error)
	{
		printf("Main Thread received exception, rethrowing it...\n");
		rethrow_exception(error);
	}
	else
	{
		printf("Main Thread did not receive any exception\n");
	}
}

int main1289()
{
	try
	{
		doWorkInThread1289();
	}
	catch (const std::exception& e)
	{
		printf("Main function caught: %s\n", e.what());
	}

	return 0;
}
