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

// 해상도 조절 코드가 없는 심플타이머(timeGetDevCaps, timeBeginPeriod 생략)
// 수신 후 1초에 한번씩 동작해서 총 5번(5초) 수신을 받지 못하면 카운트가 5까지 올라가서 삭제하고 printf한다
// 수신받은 아이디에 대해 수신하면 카운트를 0으로 초기화한다
// map에 데이터가 들어오면 타이머를 시작하고 map에 데이터가 모두 삭제되어 더이상 남아있지 않으면 타이머를 종료한다
class CSimpleTimer
{
public:
    CSimpleTimer() : m_timerID(0) 
    {}

    void startTimer(UINT interval) {
        // 타이머가 동작 중이 아닐때만 시작
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
        dataMap[data.id] = data;                // 수신데이터 보관
        missedCountMap[data.id] = 0;                // 수신 실패 카운트 0

        startTimer(1000);               // 타이머가 동작 중이지 않으면 타이머 시작 (내부 if 처리)
    }

    static void CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
        CSimpleTimer* p_this = reinterpret_cast<CSimpleTimer*>(dwUser);
        p_this->onTick();
    }

    void onTick() {
        if (dataMap.empty()) {
            stopTimer();  // 데이터가 없으면 타이머 중지
            return;
        }

        for (auto it = missedCountMap.begin(); it != missedCountMap.end(); ) {
            it->second++;  // 수신 실패 횟수 증가

            if (it->second >= 5) {
                int id = it->first;
                auto data = dataMap[id];            // 지역변수로 백업
                dataMap.erase(id);  // 데이터 맵에서 해당 데이터 제거
                it = missedCountMap.erase(it);  // 수신 실패 횟수 맵에서 해당 데이터 제거

                std::cout << "Data with ID " << id << " (Type: " << data.type << ", Station: " << data.station << ") has not been received for 5 seconds! Removing from map\n";
            }
            else {
                ++it;  // 다음 요소로 이동
            }
        }

        if (missedCountMap.empty()) {
            stopTimer();  // 모든 데이터가 처리되면 타이머 중지
        }
    }

    void isEmpty() {
        printf("%llu, %llu\n", dataMap.size(), missedCountMap.size());
    }

private:
    MMRESULT m_timerID;
    std::unordered_map<int, ST_DataDeleteCheck> dataMap;
    std::unordered_map<int, int> missedCountMap;                // ID로 수신 실패 횟수 체크
};

int main()
{
    CSimpleTimer myTimer;

    ST_DataDeleteCheck data1 = { 1, 1, 7 };                 // 스테이션, 아이디, 타입
    ST_DataDeleteCheck data2 = { 1, 2, 7 };


    // 이하는 테스트코드
    myTimer.recvData(data1);            // ID 1 수신
    printf("ID1 수신\n"); myTimer.isEmpty();
    Sleep(1000); printf("1초 대기\n");
    myTimer.recvData(data1);            // ID 1 수신
    printf("ID1 수신\n"); myTimer.isEmpty();
    myTimer.recvData(data2);                // ID 2 수신
    printf("ID2 수신\n"); myTimer.isEmpty();
    Sleep(1000); printf("1초 대기\n");

    Sleep(5000);        printf("5초 대기\n");
    myTimer.recvData(data2);                // 6초 후 ID 2 수신
    printf("ID2 수신\n"); myTimer.isEmpty();

    Sleep(5000); printf("5초 대기\n");               // 추가 수신 없음

    myTimer.isEmpty();

}
