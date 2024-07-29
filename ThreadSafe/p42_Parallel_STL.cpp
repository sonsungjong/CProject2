#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <ratio>
#include <vector>
#include <chrono>
#include <execution>				// 병렬 작업(C++17 이상)

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
	시리얼STL과 병렬STL 알고리즘의 성능 차이 비교
	병렬 정렬이 대용량 데이터셋을 정렬할 때 더 빠르다
	여러 CPU 코어를 활용하여 병렬로 수행

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
		std::sort(sorted.begin(), sorted.end());			// seq (직렬로 정렬)
		const auto endTime = std::chrono::high_resolution_clock::now();
		printResults("Serial STL ", sorted, startTime, endTime);
	}

	for (int i = 0; i < 5; ++i) {
		std::vector<double> sorted(doubles);
		const auto startTime = std::chrono::high_resolution_clock::now();
		std::sort(std::execution::par, sorted.begin(), sorted.end());						// par (병렬로 정렬)
		const auto endTime = std::chrono::high_resolution_clock::now();
		printResults("Parallel STL ", sorted, startTime, endTime);
	}

	return 0;
}