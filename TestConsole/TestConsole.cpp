#include <cstdio>
#include <chrono>
#include <thread>

int main()
{
    unsigned int flag = 0;

    while (flag < 4294967294) {
        printf("hello docker\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++flag;
    }

    return 0;
}
