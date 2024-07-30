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
	// �׽�Ʈ�� ���� ū ���� ����
	std::vector<int> vecNumbers(g_testSize);
	for (size_t i = 0; i < g_testSize; i++) {
		vecNumbers[i] = i;
	}

	// std::find �� seq, par ��
	int small = 5;			// ������ ã��
	int middle = 500'000;			// ������ �� ã��
	int big = 50'000'000;			// ū�� ã��

	// std::find(seq) [small] : 0.04ms (�������� ������ ����ó���ϴµ� �ð��� �� �ɸ���)
	auto startTime = std::chrono::high_resolution_clock::now();
	std::find(vecNumbers.begin(), vecNumbers.end(), small);
	auto endTime = std::chrono::high_resolution_clock::now();
	print_results("STL sequential(small) :", startTime, endTime);

	// std::find(seq) [middle] : 0.32ms
	startTime = std::chrono::high_resolution_clock::now();
	std::find(vecNumbers.begin(), vecNumbers.end(), middle);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL sequential(middle) :", startTime, endTime);

	// std::find(seq) [big] : 24.4ms (���� �ɸ��� ã��� ����ó���ؾ� �ӵ������� �ȴ�)
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