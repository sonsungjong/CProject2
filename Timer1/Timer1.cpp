#include <cstdio>
#include <functional>
#include <chrono>
#include <thread>

/*
    Ÿ�̸� ���� (Ÿ�̸� ������ ���Ÿ� ������ ��üȭ �Ǵ� ��ž�ϰ� ��ŸƮ)
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

    // �ֱ�(ms), �ݹ��Լ�, �ִ����Ƚ��
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
    int m_count;                // �ݹ��Լ��� ȣ��� Ƚ��
};

class OtherClass
{
public:
    void helloFunc()
    {
        printf("hello world! %d\n", m_num++);
        if (m_num >= 10) {
            m_timer.stop();
            m_num = 0;              // ���߰� �ʱ�ȭ
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
        // �޼��� Ÿ�̸� ����
        if (m_timer.isRunning() == false)
        {
            const long long ms = 1000;
            auto callback = std::bind(&OtherClass::helloFunc, this);
            m_timer.start(ms, callback);
        }
        else
        {
            printf("helloFunc Ÿ�̸Ӱ� �̹� ���� ��\n");
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

    // [1] Ŭ���� ���ο��� Ÿ�̸� ��ü �����ؼ� ���
    other.run();

    // [2] �����Լ� Ÿ�̸� ���� (+�ִ� 5��)
    timer1.start(2000, gg, 5);

    // [3] ���ٷ� �޼��带 Ŀ���͸���¡�ؼ� Ÿ�̸� ���
    timer3.start(3000, [&]() 
    {
        other.byeFunc();                // �ٸ� ��ü�� �޼���
        if (other.m_bye >= 10) 
        {
            timer3.stop();
        }
    }
    );

    // [4] ��ü�� �޼��忡 ���� Ÿ�̸� ���
    timer4.start(2500, std::bind(&OtherClass::loopInfinity, other));


    while (true) 
    {
        printf("==== main UI thread ==== \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (other.isTimerWorking() == false)
        {
            other.run();                // Ÿ�̸� �����
        }
    }

    return 0;
}