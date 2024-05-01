import "HeaderUnits.h";
#include <vector>
#include <chrono>

// int Ÿ�� �迭�� ¦���� Ȧ���� ������ �Լ�
void separateOddsAndEvens1(const int arr[], size_t size, int** odds, size_t* numOdds, int** evens, size_t* numEvens)
{
	// ¦���� Ȧ���� ������ ����
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

	// ���� ���� �� �迭�� ũ�⿡ �´� ������ �Ҵ��Ѵ�
	*odds = new int[*numOdds];
	*evens = new int[*numEvens];

	// ���� �迭�� ��� Ȧ���� ¦�� ���Ҹ� ���� ���� �迭�� �����Ѵ�
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

// �����͸� ���۷����� ������ ���
void separateOddsAndEvens2(const int arr[], size_t size, int*& odds, size_t& numOdds, int*& evens, size_t& numEvens)
{
	// ¦���� Ȧ���� ������ ����
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

	// ���� ���� �� �迭�� ũ�⿡ �´� ������ �Ҵ��Ѵ�
	odds = new int[numOdds];
	evens = new int[numEvens];

	// ���� �迭�� ��� Ȧ���� ¦�� ���Ҹ� ���� ���� �迭�� �����Ѵ�
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

// ���͸� ����� ���
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
	printf("�ҿ�ð� : %.6lf microseconds\n", elapsed.count());

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