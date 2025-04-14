#include "CircularQueue1.h"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>

std::mutex g_end_mtx;
std::condition_variable g_cv_program_end;
bool g_program_end = false;

int main()
{
    ST_CircularQueue queue1;
    char szSTR[256] = { 0, };
    int max_count = 100;
    int queue_id = 0;
    init_CircularQueue(&queue1, max_count, sizeof(szSTR), queue_id);

    std::thread consumer([&queue1]() {
        while (g_program_end == false)
        {
            void* data = wait_pop_CircularQueue(&queue1);
            if (data != NULL)
            {
                char szCOPY[256] = { 0, };
                memcpy(szCOPY, data, sizeof(szCOPY));
                free(data);             // deep_copy를 사용하므로 사용 후 free

                if (strcmp(szCOPY, "exit") == 0)
                {
                    // exit 를 받으면 종료
                    std::lock_guard<std::mutex> lock(g_end_mtx);
                    g_program_end = true;
                    g_cv_program_end.notify_one();
                    printf("추가 쓰레드 종료\n");
                    break;
                }

                printf(">>%s\n", szCOPY);
            }
        }
    });
    consumer.detach();

    while (g_program_end == false)
    {
        char keyboard_input[256] = { 0, };
        memset(keyboard_input, 0, 256);
        rewind(stdin);
        scanf_s("%[^\n]s", keyboard_input, 255);

        enqueue_CircularQueue(&queue1, keyboard_input);

        if (strcmp(keyboard_input, "exit") == 0)
        {
            printf("입력을 종료\n");
            break;
        }
    }

    // 메인 스레드는 consumer 스레드가 종료될 때까지 대기 (종료 신호)
    {
        std::unique_lock<std::mutex> lock(g_end_mtx);
        g_cv_program_end.wait(lock, [] { return g_program_end; });

        // Sleep(100);          // 모든 쓰레드가 종료될때까지 대기
    }

    destroy_CircularQueue(&queue1);

    printf("프로그램 정상 종료\n");
}
