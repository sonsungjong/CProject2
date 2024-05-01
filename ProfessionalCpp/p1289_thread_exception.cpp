import "HeaderUnits.h";


// 스레드에서 발생한 익셉션을 처리하는 방법
void doSomeWork1289()
{
	for (int i = 0; i < 5; ++i)
	{
		printf("%d\n", i);
	}
	printf("Thread throwing a runtime_error exception...\n");
	throw std::runtime_error("Exception from thread");					// 익셉션을 던지게 한다
}

// 처리할 익셉션에 대한 레퍼런스를 받아서 exception_ptr 매개변수에 대입하고 스레드는 정상적으로 종료하도록 구성
void threadFunc1289(std::exception_ptr& err)
{
	try
	{
		doSomeWork1289();
	}
	catch (...)
	{
		printf("Thread caught exception, returning exception...\n");
		err = std::current_exception();				// 처리할 익셉션의 레퍼런스(current_exception())를 대입!!
	}
}

void doWorkInThread1289()
{
	std::exception_ptr error;
	std::thread th(threadFunc1289, std::ref(error));
	th.join();

	// 스레드에서 익셉션이 발생했는지 검사한다
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
