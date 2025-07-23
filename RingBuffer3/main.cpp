#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "CSafeRingBuffer2.h"
std::atomic_bool g_program_end = false;

int main()
{
    int max_buffer = 1024 * 1024 * 2;           // 2MB
    int ring_id = 0;
    CSafeRingBuffer ring1(max_buffer, ring_id);                     // 2MB, ID = 0

    std::thread consumer([&ring1]() {
        while (g_program_end == false)
        {
            std::unique_ptr<unsigned char[]> data;
            size_t buf_size = ring1.wait_read(data);
            if (data)
            {
                unsigned long long offset = 0;
                while (offset < buf_size)
                {
                    // 분기처리 : 헤더를 먼저 체크하고
                    // 바디를 읽는다

                    // 최소 헤더의 크기가 있어야 함
                    if (offset + sizeof(unsigned short) > buf_size)
                    {
                        // 헤더 크기만큼의 데이터가 부족하면 break (파싱할 수 없음)
                        break;
                    }

                    // 헤더에서 바디의 길이(예: unsigned short)를 읽음
                    unsigned short body_size = 0;
                    memcpy(&body_size, data.get() + offset, sizeof(unsigned short));
                    offset += sizeof(unsigned short);

                    // 헤더에서 지정한 바디의 크기만큼의 데이터가 남아있는지 확인
                    if (offset + body_size > buf_size)
                    {
                        // 남은 데이터가 부족하면 break (메시지 손상 또는 파싱 대기 처리 필요)
                        break;
                    }

                    // 분기처리 (크기 또는 아이디)
                    unsigned char body_msg[256] = { 0 };
                    memcpy(body_msg, data.get() + offset, body_size);
                    if (body_size == 256)
                    {
                        printf(">>%s (NULL포함)\n", (const char*)body_msg);
                    }
                    else
                    {
                        printf(">>%s\n", (const char*)body_msg);
                    }

                    if (strcmp((const char*)body_msg, "exit") == 0)
                    {
                        // exit 를 받으면 종료
                        //std::lock_guard<std::mutex> lock(g_end_mtx);
                        //g_program_end = true;
                        //g_cv_program_end.notify_one();
                        g_program_end = true;
                    }

                    offset += body_size;
                }
            }
        }
        printf("추가 쓰레드 종료\n");
        });
    consumer.detach();

    while (g_program_end == false)
    {
        char keyboard_input[256] = { 0, };
        memset(keyboard_input, 0, 256);
        rewind(stdin);
        scanf_s("%[^\n]s", keyboard_input, 255);

        // 처음 2바이트에 바디사이즈를 넣고 그 다음에 바디를 넣는다 (하나는 strlen만큼, 하나는 NULL을 포함해서 256만큼)

        unsigned short body_size = (unsigned short)strlen(keyboard_input);      // strlen 만큼 넣는다
        unsigned long long total_size = sizeof(body_size) + body_size;
        std::unique_ptr<unsigned char[]> header_body(new unsigned char[total_size]);
        memcpy(header_body.get(), &body_size, sizeof(body_size));
        memcpy(header_body.get() + sizeof(body_size), keyboard_input, body_size);

        unsigned short body_size2 = 256;                // NULL을 포함해서 넣는다
        unsigned long long total_size2 = sizeof(body_size2) + body_size2;
        std::unique_ptr<unsigned char[]> header_body2(new unsigned char[total_size2]);
        memcpy(header_body2.get(), &body_size2, sizeof(body_size2));
        memcpy(header_body2.get() + sizeof(body_size2), keyboard_input, body_size2);

        ring1.write(header_body.get(), total_size);
        ring1.write(header_body2.get(), total_size2);

        if (strcmp(keyboard_input, "exit") == 0)
        {
            printf("입력을 종료\n");
            g_program_end = true;
        }
    }

#ifdef _WIN32
    Sleep(100);
#else
    usleep(100 * 1000);
#endif

    printf("프로그램 정상 종료\n");
}

