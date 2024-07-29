#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <ratio>
#include <vector>
#include <chrono>
#include <execution>				// ���� �۾�(C++17 �̻�)

const size_t g_ullSize = 1'000'000;

void printResults(
	const char* const _szTag, 
	const std::vector<double>& _vecSorted,
	std::chrono::high_resolution_clock::time_point _startTime,
	std::chrono::high_resolution_clock::time_point _endTime
) 
{
	printf("%s: Lowest: %g Highest: %g Time: %fms\n", 
		_szTag,
		_vecSorted.front(),
		_vecSorted.back(),
		std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(_endTime - _startTime).count());
}

/*
	�ø���STL�� ����STL �˰����� ���� ���� ��
	���� ������ ��뷮 �����ͼ��� ������ �� �� ������
	���� CPU �ھ Ȱ���Ͽ� ���ķ� ����

	Serial STL : Lowest: 4502 Highest: 4.29497e+09 Time: 316.047100ms
	Parallel STL : Lowest: 4502 Highest: 4.29497e+09 Time: 48.393900ms
*/
int main3() {
	std::random_device rd;

	printf("Testing with %zu...\n", g_ullSize);
	std::vector<double> doubles(g_ullSize);
	for (auto& dbNum : doubles) {
		dbNum = static_cast<double>(rd());
	}

	// time how long it takes to sort them:
	for (int i = 0; i < 5; ++i) {
		std::vector<double> sorted(doubles);
		const auto startTime = std::chrono::high_resolution_clock::now();
		std::sort(sorted.begin(), sorted.end());			// seq (���ķ� ����)
		const auto endTime = std::chrono::high_resolution_clock::now();
		printResults("Serial STL ", sorted, startTime, endTime);
	}

	for (int i = 0; i < 5; ++i) {
		std::vector<double> sorted(doubles);
		const auto startTime = std::chrono::high_resolution_clock::now();
		std::sort(std::execution::par, sorted.begin(), sorted.end());						// par (���ķ� ����)
		const auto endTime = std::chrono::high_resolution_clock::now();
		printResults("Parallel STL ", sorted, startTime, endTime);
	}

	return 0;
}