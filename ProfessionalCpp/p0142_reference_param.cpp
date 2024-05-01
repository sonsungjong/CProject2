import "HeaderUnits.h";
#include <vector>
#include <chrono>

// int 타입 배열을 짝수와 홀수로 나누는 함수
void separateOddsAndEvens1(const int arr[], size_t size, int** odds, size_t* numOdds, int** evens, size_t* numEvens)
{
	// 짝수와 홀수의 개수를 센다
	*numOdds = *numEvens = 0;
	for (int i = 0; i < size; ++i)
	{
		if (arr[i] % 2 == 1)
		{
			++(*numOdds);
		}
		else {
			++(*numEvens);
		}
	}

	// 새로 만들 두 배열의 크기에 맞는 공간을 할당한다
	*odds = new int[*numOdds];
	*evens = new int[*numEvens];

	// 원본 배열에 담긴 홀수와 짝수 원소를 새로 만들 배열에 복사한다
	size_t oddsPos = 0;
	size_t evensPos = 0;

	for (int i = 0; i < size; ++i) {
		if (arr[i] % 2 == 1) {
			(*odds)[oddsPos++] = arr[i];
		}
		else {
			(*evens)[evensPos++] = arr[i];
		}
	}
}

// 포인터를 래퍼런스로 변경할 경우
void separateOddsAndEvens2(const int arr[], size_t size, int*& odds, size_t& numOdds, int*& evens, size_t& numEvens)
{
	// 짝수와 홀수의 개수를 센다
	numOdds = numEvens = 0;
	for (int i = 0; i < size; ++i)
	{
		if (arr[i] % 2 == 1)
		{
			++numOdds;
		}
		else {
			++numEvens;
		}
	}

	// 새로 만들 두 배열의 크기에 맞는 공간을 할당한다
	odds = new int[numOdds];
	evens = new int[numEvens];

	// 원본 배열에 담긴 홀수와 짝수 원소를 새로 만들 배열에 복사한다
	size_t oddsPos = 0;
	size_t evensPos = 0;

	for (int i = 0; i < size; ++i) {
		if (arr[i] % 2 == 1) {
			odds[oddsPos++] = arr[i];
		}
		else {
			evens[evensPos++] = arr[i];
		}
	}
}

// 백터를 사용할 경우
void separateOddsAndEvens3(const std::vector<int>& arr, std::vector<int>& odds, std::vector<int>& evens)
{
	for (int i : arr) {
		if (i % 2 == 1) {
			odds.push_back(i);
		}
		else {
			evens.push_back(i);
		}
	}
}

struct OddsAndEvens { std::vector<int> odds, evens; };

OddsAndEvens separateOddsAndEvens4(const std::vector<int>& arr)
{
	std::vector<int> odds, evens;
	for (int i : arr) {
		if (i % 2 == 1) {
			odds.push_back(i);
		}
		else {
			evens.push_back(i);
		}
	}

	return OddsAndEvens{ .odds = odds, .evens = evens };
}

int main0142()
{
	int unSplit[]{ 1,2,3,4,5,6,7,8,9,10 };
	int* oddNums{ nullptr };
	int* evenNums{ nullptr };
	size_t numOdds{ 0 };
	size_t numEvens{ 0 };

	std::vector<int> venUnSplit {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> odds, evens;

	auto start = std::chrono::high_resolution_clock::now();
	separateOddsAndEvens1(unSplit, std::size(unSplit), &oddNums, &numOdds, &evenNums, &numEvens);				// 0.4 microseconds
	//separateOddsAndEvens2(unSplit, std::size(unSplit), oddNums, numOdds, evenNums, numEvens);							// 0.4 microseconds
	//separateOddsAndEvens3(venUnSplit, odds, evens);							// 1.3 microseconds
	//auto oddsAndEvens{ separateOddsAndEvens4(venUnSplit) };				// 1.3 microseconds
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> elapsed = end - start;
	printf("소요시간 : %.6lf microseconds\n", elapsed.count());

	for (int i = 0; i < numOdds; ++i) {
		printf("%d ", oddNums[i]);
	}

	printf("\n");

	for (int i = 0; i < numEvens; ++i) {
		printf("%d ", evenNums[i]);
	}
	delete[] oddNums;
	oddNums = nullptr;
	delete[] evenNums;
	evenNums = nullptr;


	//for (int i : odds) {
	//	printf("%d ", i);
	//}
	//printf("\n");
	//for (int i : evens) {
	//	printf("%d ", i);
	//}


	return 0;
}