import "HeaderUnits.h";

class Simple0373
{
public:
	Simple0373() 
	{
		printf("Simple373 constructor called\n");
	}
	~Simple0373()
	{
		printf("Simple373 destructor called\n");
	}
};

void useResource373(weak_ptr<Simple0373>& a_weak_int)
{
	auto res = a_weak_int.lock();
	if (res) {
		printf("Resource still alive.\n");
	}
	else {
		printf("Resource has been freed.\n");
	}
}

int main0373() 
{
	auto shared_int_ptr = make_shared<Simple0373>();
	weak_ptr<Simple0373> weak_int = shared_int_ptr;

	useResource373(weak_int);				// shared_ptr이 잘 참조 중인지 해제됬는지 검사

	shared_int_ptr.reset();

	useResource373(weak_int);				// shared_ptr이 잘 참조 중인지 해제됬는지 검사

	return 0;
}