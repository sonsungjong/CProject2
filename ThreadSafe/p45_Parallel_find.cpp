#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <execution>
#include <memory>
#include <vector>
#include <atomic>
#include <algorithm>

#include "utils.h"
#include "common_objs.h"


const size_t g_testSize = 100'000'000;

int main45() {
	// 테스트를 위한 큰 백터 생성
	std::vector<int> vecNumbers(g_testSize);
	for (size_t i = 0; i < g_testSize; i++) {
		vecNumbers[i] = i;
	}

	// std::find 의 seq, par 비교
	int small = 5;			// 작은값 찾기
	int middle = 500'000;			// 적당한 값 찾기
	int big = 50'000'000;			// 큰값 찾기

	// std::find(seq) [small] : 0.04ms (작은값은 오히려 병렬처리하는데 시간이 더 걸린다)
	auto startTime = std::chrono::high_resolution_clock::now();
	std::find(vecNumbers.begin(), vecNumbers.end(), small);
	auto endTime = std::chrono::high_resolution_clock::now();
	print_results("STL sequential(small) :", startTime, endTime);

	// std::find(seq) [middle] : 0.32ms
	startTime = std::chrono::high_resolution_clock::now();
	std::find(vecNumbers.begin(), vecNumbers.end(), middle);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL sequential(middle) :", startTime, endTime);

	// std::find(seq) [big] : 24.4ms (오래 걸리는 찾기는 병렬처리해야 속도개선이 된다)
	startTime = std::chrono::high_resolution_clock::now();
	std::find(vecNumbers.begin(), vecNumbers.end(), big);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL sequential(big) :", startTime, endTime);

	printf("\n===============\n");

	// std::find(par) [small] :  0.08ms
	startTime = std::chrono::high_resolution_clock::now();
	std::find(std::execution::par, vecNumbers.begin(), vecNumbers.end(), small);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL parallel-par(small) :", startTime, endTime);

	// std::find(par) [middle] :  0.18ms
	startTime = std::chrono::high_resolution_clock::now();
	std::find(std::execution::par, vecNumbers.begin(), vecNumbers.end(), middle);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL parallel-par(middle) :", startTime, endTime);

	// std::find(par) [big] :  3.8ms
	startTime = std::chrono::high_resolution_clock::now();
	std::find(std::execution::par, vecNumbers.begin(), vecNumbers.end(), big);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL parallel-par(big) :", startTime, endTime);

	return 0;
}