#include "pch.h"
#include <WinSock2.h>
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <unordered_map>

#pragma comment(lib, "winmm.lib")

typedef struct _ST_DataDeleteCheck {
    int station;
    int id;
    int type;
} ST_DataDeleteCheck;

// �ػ� ���� �ڵ尡 ���� ����Ÿ�̸�(timeGetDevCaps, timeBeginPeriod ����)
// ���� �� 1�ʿ� �ѹ��� �����ؼ� �� 5��(5��) ������ ���� ���ϸ� ī��Ʈ�� 5���� �ö󰡼� �����ϰ� printf�Ѵ�
// ���Ź��� ���̵� ���� �����ϸ� ī��Ʈ�� 0���� �ʱ�ȭ�Ѵ�
// map�� �����Ͱ� ������ Ÿ�̸Ӹ� �����ϰ� map�� �����Ͱ� ��� �����Ǿ� ���̻� �������� ������ Ÿ�̸Ӹ� �����Ѵ�
class CSimpleTimer
{
public:
    CSimpleTimer() : m_timerID(0) 
    {}

    void startTimer(UINT interval) {
        // Ÿ�̸Ӱ� ���� ���� �ƴҶ��� ����
        if (m_timerID == 0) {
            m_timerID = timeSetEvent(interval, 0, TimerCallback, (DWORD_PTR)this, TIME_PERIODIC);
        }
    }

    void stopTimer() {
        if (m_timerID != 0) {
            timeKillEvent(m_timerID);
            m_timerID = 0;
        }
    }

    void recvData(const ST_DataDeleteCheck& data) {
        dataMap[data.id] = data;                // ���ŵ����� ����
        missedCountMap[data.id] = 0;                // ���� ���� ī��Ʈ 0

        startTimer(1000);               // Ÿ�̸Ӱ� ���� ������ ������ Ÿ�̸� ���� (���� if ó��)
    }

    static void CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
        CSimpleTimer* p_this = reinterpret_cast<CSimpleTimer*>(dwUser);
        p_this->onTick();
    }

    void onTick() {
        if (dataMap.empty()) {
            stopTimer();  // �����Ͱ� ������ Ÿ�̸� ����
            return;
        }

        for (auto it = missedCountMap.begin(); it != missedCountMap.end(); ) {
            it->second++;  // ���� ���� Ƚ�� ����

            if (it->second >= 5) {
                int id = it->first;
                auto data = dataMap[id];            // ���������� ���
                dataMap.erase(id);  // ������ �ʿ��� �ش� ������ ����
                it = missedCountMap.erase(it);  // ���� ���� Ƚ�� �ʿ��� �ش� ������ ����

                std::cout << "Data with ID " << id << " (Type: " << data.type << ", Station: " << data.station << ") has not been received for 5 seconds! Removing from map\n";
            }
            else {
                ++it;  // ���� ��ҷ� �̵�
            }
        }

        if (missedCountMap.empty()) {
            stopTimer();  // ��� �����Ͱ� ó���Ǹ� Ÿ�̸� ����
        }
    }

    void isEmpty() {
        printf("%llu, %llu\n", dataMap.size(), missedCountMap.size());
    }

private:
    MMRESULT m_timerID;
    std::unordered_map<int, ST_DataDeleteCheck> dataMap;
    std::unordered_map<int, int> missedCountMap;                // ID�� ���� ���� Ƚ�� üũ
};

int main()
{
    CSimpleTimer myTimer;

    ST_DataDeleteCheck data1 = { 1, 1, 7 };                 // �����̼�, ���̵�, Ÿ��
    ST_DataDeleteCheck data2 = { 1, 2, 7 };


    // ���ϴ� �׽�Ʈ�ڵ�
    myTimer.recvData(data1);            // ID 1 ����
    printf("ID1 ����\n"); myTimer.isEmpty();
    Sleep(1000); printf("1�� ���\n");
    myTimer.recvData(data1);            // ID 1 ����
    printf("ID1 ����\n"); myTimer.isEmpty();
    myTimer.recvData(data2);                // ID 2 ����
    printf("ID2 ����\n"); myTimer.isEmpty();
    Sleep(1000); printf("1�� ���\n");

    Sleep(5000);        printf("5�� ���\n");
    myTimer.recvData(data2);                // 6�� �� ID 2 ����
    printf("ID2 ����\n"); myTimer.isEmpty();

    Sleep(5000); printf("5�� ���\n");               // �߰� ���� ����

    myTimer.isEmpty();

}
