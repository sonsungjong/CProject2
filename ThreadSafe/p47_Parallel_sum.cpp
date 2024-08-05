#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <execution>
#include <memory>
#include <vector>
#include <atomic>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>

#include "utils.h"
#include "common_objs.h"

const size_t g_testSize1 = 1000;
const size_t g_testSize2 = 1'000'000;
const size_t g_testSize3 = 10'000'000;

template<typename Iterator, typename OutIterator>
void SequentialPartialSum(Iterator _first, Iterator _last, OutIterator _y) {
	unsigned long const length = std::distance(_first, _last);

	_y[0] = _first[0];
	for (size_t i = 1; i < length; ++i) {
		_y[i] = _first[i] + _y[i - 1];
	}
}

int main47() {
	std::vector<int> ints(g_testSize3, 1);
	std::vector<int> outs(g_testSize3);
	for (size_t i = 0; i < ints.size(); ++i) {
		ints[i] = i;
	}

	SequentialPartialSum(ints.begin(), ints.end(), outs.begin());

	for (size_t i = 0; i < outs.size(); ++i) {
		printf("%d ", outs[i]);
	}
	printf("\n");

	auto startTime = std::chrono::high_resolution_clock::now();
	std::inclusive_scan(ints.cbegin(), ints.cbegin(), outs.begin());
	auto endTime = std::chrono::high_resolution_clock::now();
	print_results("seq scan", startTime, endTime);

	startTime = std::chrono::high_resolution_clock::now();
	std::inclusive_scan(std::execution::par,ints.cbegin(), ints.cbegin(), outs.begin());
	endTime = std::chrono::high_resolution_clock::now();
	print_results("par scan", startTime, endTime);

	return 0;
};