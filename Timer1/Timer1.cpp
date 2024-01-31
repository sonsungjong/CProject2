#include <iostream>
#include <tchar.h>
#include <future>
#include <functional>
#include <chrono>
#include <thread>

/*
    타이머 예제
*/
class MyTimer
{
public:
    MyTimer() : m_running(false) {}

    void start(const long long a_milliseconds, std::function<void()> callback)
    {
        if (m_running == false)
        {
            m_running = true;
            std::thread([=]() {
                while (m_running == true)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(a_milliseconds));
                    if (m_running == true)
                    {
                        callback();
                    }
                }
                }).detach();
        }
    }

    void stop()
    {
        if (m_running == true)
        {
            m_running = false;
        }
    }

    bool isRunning() const
    {
        return m_running;
    }

private:
    bool m_running;
};

class OtherClass
{
public:
    void helloFunc()
    {
        printf("hello world! %d\n", m_num++);
        if (m_num >= 10) {
            m_timer.stop();
        }
    }

    void byeFunc()
    {
        printf("bye! %d\n", m_bye++);
    }

    void loopInfinity()
    {
        printf("loop\n");
    }

    void run()
    {
        // 메서드 타이머 시작
        m_timer.start(1000, std::bind(&OtherClass::helloFunc, this));
    }

    int m_bye = 0;

private:
    int m_num = 0;
    MyTimer m_timer;
};

void gg()
{
    printf("gg\n");
}

int main()
{
    MyTimer timer1;
    OtherClass other;
    MyTimer timer3;
    MyTimer timer4;

    // [1] 클래스 내부에서 타이머 객체 생성해서 사용
    other.run();

    // [2] 전역함수 타이머 시작
    timer1.start(2000, gg);

    // [3] 람다로 메서드를 커스터마이징해서 타이머 사용
    timer3.start(3000, [&]() {
        other.byeFunc();
        if (other.m_bye >= 10) {
            timer3.stop();
        }
    });

    // [4] 객체의 메서드에 대해 타이머 사용 (단, 타이머 객체 주소를 해제하는 곳에서 접근가능하게 선언해주거나 무한타이머로 사용할 것)
    timer4.start(2500, std::bind(&OtherClass::loopInfinity, other));


    while (true) 
    {
        printf("==== main UI thread ==== \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}