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
    MyTimer() : m_running(false) 
    {}

    virtual ~MyTimer()
    {
        stop();
    }

    virtual void start(const long long a_milliseconds, std::function<void()> callback)
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

    // [2] 전역함수 타이머 시작
    timer1.start(2000, gg);

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

    // [4] 객체의 메서드에 대해 타이머 사용 (단, 타이머 실행 중 객체가 해제되지 않게할 것!)
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