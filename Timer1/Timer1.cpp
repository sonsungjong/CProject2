#include <cstdio>
#include <functional>
#include <chrono>
#include <thread>

/*
    타이머 예제 (타이머 여러개 쓸거면 여러개 객체화 또는 스탑하고 스타트)
*/
class MyTimer
{
public:
    MyTimer() : m_running(false), m_count(0)
    {}

    virtual ~MyTimer()
    {
        stop();
    }

    // 주기(ms), 콜백함수, 최대실행횟수
    virtual void start(const long long a_milliseconds, std::function<void()> callback, int count = -1)
    {
        if (m_running == false)
        {
            m_running = true;
            m_count = count;
            std::thread([=]() mutable {
                while (m_running == true && (m_count != 0))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(a_milliseconds));
                    if (m_running == true)
                    {
                        callback();
                        if (m_count > 0) 
                        {
                            --m_count;
                            if (m_count == 0)
                            {
                                stop();
                            }
                        }
                    }
                }
            }).detach();
        }
    }

    virtual void stop()
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
    int m_count;                // 콜백함수가 호출될 횟수
};

class OtherClass
{
public:
    void helloFunc()
    {
        printf("hello world! %d\n", m_num++);
        if (m_num >= 10) {
            m_timer.stop();
            m_num = 0;              // 멈추고 초기화
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

    bool isTimerWorking()
    {
        bool isWorking = false;
        if (m_timer.isRunning() == false)
        {
            isWorking = false;
        }
        else
        {
            isWorking = true;
        }

        return isWorking;
    }

    void run()
    {
        // 메서드 타이머 시작
        if (m_timer.isRunning() == false)
        {
            const long long ms = 1000;
            auto callback = std::bind(&OtherClass::helloFunc, this);
            m_timer.start(ms, callback);
        }
        else
        {
            printf("helloFunc 타이머가 이미 실행 중\n");
        }
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

    // [2] 전역함수 타이머 시작 (+최대 5번)
    timer1.start(2000, gg, 5);

    // [3] 람다로 메서드를 커스터마이징해서 타이머 사용
    timer3.start(3000, [&]() 
    {
        other.byeFunc();                // 다른 객체의 메서드
        if (other.m_bye >= 10) 
        {
            timer3.stop();
        }
    }
    );

    // [4] 객체의 메서드에 대해 타이머 사용
    timer4.start(2500, std::bind(&OtherClass::loopInfinity, other));


    while (true) 
    {
        printf("==== main UI thread ==== \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (other.isTimerWorking() == false)
        {
            other.run();                // 타이머 재실행
        }
    }

    return 0;
}