import "HeaderUnits.h";
#include <time.h>

using namespace std;

struct StructPointer
{
	int salary;
};

int main0122()
{
	long start, end;
	double cpu_time, real_time;

	int i { 8 };
	int* pointer { &i };				// ���۷���(����) ������& �� ����ؼ� �ּҰ� ���

	StructPointer* emp = new StructPointer{.salary = 10};
	
	//time(&start);				// real_time
	start = clock();				// cpu_time
	if (emp != NULL && emp != nullptr && emp->salary > 0) {
		cout << (*emp).salary << endl;
		cout << emp->salary << endl;
	}

	end = clock();				// cpu_time
	//time(&end);				// real_time
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;				// cpu_time
	//real_time = difftime(end, start);												// real_time

	printf("�ҿ� �ð� : %lf �� \n", cpu_time);					// cpu_time
	//printf("�ҿ� �ð� : %lf �� \n", real_time);				// real_time

	return 0;
}