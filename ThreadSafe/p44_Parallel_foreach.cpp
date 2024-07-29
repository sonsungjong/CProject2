#include <stdio.h>
#include <vector>
#include <thread>
#include <future>
#include <execution>

#include "utils.h"
#include "common_objs.h"

template<typename Iterator, typename Func>
void parallelForeach(Iterator first, Iterator last, Func f)
{
	unsigned long const length = std::distance(first, last);

	if (!length) return;

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;

	/*	Declare the needed data structures	*/

	std::vector<std::future<void>> futures(num_threads - 1);
	std::vector<std::thread> threads(num_threads - 1);
	join_threads joiner(threads);

	/*	Partition of data between threads	*/

	Iterator block_start = first;
	for (unsigned long i = 0; i <= (num_threads - 2); i++)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		std::packaged_task<void(void)> task(
			[=]()
			{
				std::for_each(block_start, block_end, f);
			}
		);

		futures[i] = task.get_future();
		threads[i] = std::thread(std::move(task));

		block_start = block_end;
	}

	// call the function for last block from this thread
	std::for_each(block_start, last, f);

	/*	wait until futures are ready	*/
	for (unsigned long i = 0; i < (num_threads - 1); ++i) {
		futures[i].get();
	}
}

template<typename Iterator, typename Func>
void parallelForeachAsync(Iterator first, Iterator last, Func f)
{
	unsigned long const length = std::distance(first, last);

	if (!length) return;

	unsigned long const min_per_thread = 25;

	if (length < 2 * min_per_thread)
	{
		std::for_each(first, last, f);
	}
	else
	{
		Iterator const mid_point = first + length / 2;
		std::future<void> first_half = std::async(&parallelForeachAsync<Iterator, Func>, first, mid_point, f);

		parallelForeachAsync(mid_point, last, f);
		first_half.get();
	}

}

size_t g_ullTestSize = 1000;

int main() {
	std::vector<int> vecNumbers(g_ullTestSize);
	for (auto& i : vecNumbers) {
		i = 1;
	}

	auto long_function = [](const int& n) {
		int nSum = 0;
		for (auto i = 0; i < 100000; i++) {
			nSum += 1 * (i - 499);
		}
	};

	auto startTime = std::chrono::high_resolution_clock::now();
	std::for_each(vecNumbers.cbegin(), vecNumbers.cend(), long_function);
	auto endTime = std::chrono::high_resolution_clock::now();
	print_results("STL                   ", startTime, endTime);

	startTime = std::chrono::high_resolution_clock::now();
	for_each(std::execution::seq, vecNumbers.cbegin(), vecNumbers.cend(), long_function);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL-seq               ", startTime, endTime);

	startTime = std::chrono::high_resolution_clock::now();
	std::for_each(std::execution::par, vecNumbers.cbegin(), vecNumbers.cend(), long_function);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("STL-par               ", startTime, endTime);

	startTime = std::chrono::high_resolution_clock::now();
	parallelForeach(vecNumbers.cbegin(), vecNumbers.cend(), long_function);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("Parallel-package_task ", startTime, endTime);

	startTime = std::chrono::high_resolution_clock::now();
	parallelForeachAsync(vecNumbers.cbegin(), vecNumbers.cend(), long_function);
	endTime = std::chrono::high_resolution_clock::now();
	print_results("Parallel-async        ", startTime, endTime);

	return 0;
}