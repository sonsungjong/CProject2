import "HeaderUnits.h";
#include <time.h>
#include <chrono>

using namespace std;

struct StructPointer
{
	int salary;
};

int main0122()
{
	long start, end;
	double cpu_time, real_time;
	// auto start_chrono = std::chrono::high_resolution_clock::now();

	int i { 8 };
	int* pointer { &i };				// 래퍼런스(참조) 연산자& 를 사용해서 주소값 사용

	StructPointer* emp = new StructPointer{.salary = 10};
	
	//time(&start);				// real_time
	start = clock();				// cpu_time
	if (emp != NULL && emp != nullptr && emp->salary > 0) {
		cout << (*emp).salary << endl;
		cout << emp->salary << endl;
	}

	end = clock();				// cpu_time
	// time(&end);				// real_time
	// auto end = std::chrono::high_resolution_clock::now();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;				// cpu_time
	//real_time = difftime(end, start);												// real_time
	// std::chrono::duration<double, std::micro> elapsed = end - start;


	printf("소요 시간 : %lf 초 \n", cpu_time);					// cpu_time
	//printf("소요 시간 : %lf 초 \n", real_time);				// real_time
	// std::cout << "소요 시간: " << elapsed.count() << " microseconds\n";

	return 0;
}