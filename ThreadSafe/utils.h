#pragma once
#include <stddef.h>
#include <stdio.h>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>

void print_results(const char* const tag, std::chrono::high_resolution_clock::time_point startTime, std::chrono::high_resolution_clock::time_point endTime);

//unsigned long get_optimal_num_threads();
