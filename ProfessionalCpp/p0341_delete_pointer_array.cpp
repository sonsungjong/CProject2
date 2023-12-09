import "HeaderUnits.h";

class Simple0341
{
public:
	Simple0341() { printf("Simple constructor called!\n"); }
	~Simple0341() { printf("Simple destructor called!\n"); }

};

int main0341() 
{
	// 포인터 배열에 대해 delete[] 를 호출할 때는 각 원소가 가리키는 객체를 일일이 해제해야 한다. 
	const size_t size { 4 };
	Simple0341** mySimplePtrArray = new Simple0341*[size];					// 1번

	// 포인터마다 객체를 할당
	mySimplePtrArray[0] = new Simple0341();				// 2번
	mySimplePtrArray[1] = new Simple0341();				// 3번
	mySimplePtrArray[2] = new Simple0341();				// 4번
	mySimplePtrArray[3] = new Simple0341();				// 5번

	// 할당된 객체 삭제
	delete mySimplePtrArray[0];				// 1번
	mySimplePtrArray[0] = NULL;
	delete mySimplePtrArray[1];				// 2번
	mySimplePtrArray[1] = NULL;
	delete mySimplePtrArray[2];				// 3번
	mySimplePtrArray[2] = NULL;
	delete mySimplePtrArray[3];				// 4번
	mySimplePtrArray[3] = NULL;

	// 배열 삭제
	delete[] mySimplePtrArray;				// 5번
	mySimplePtrArray = NULL;

	return 0;
}