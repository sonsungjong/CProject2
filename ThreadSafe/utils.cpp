#include "utils.h"
#include "common_objs.h"

void print_results(const char* const tag, std::chrono::high_resolution_clock::time_point startTime, std::chrono::high_resolution_clock::time_point endTime)
{
    printf("%s: Time: %fms\n", tag,
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count());
}
