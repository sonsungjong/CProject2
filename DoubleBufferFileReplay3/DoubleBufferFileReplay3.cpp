#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>
#include <atomic>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <stack>

#include <Windows.h>
#include <mmsystem.h>

long long g_debugReplayStartTime = 0;
long long g_debugReplayEndTime = 0;
long long g_debugPauseTime = 0;
std::vector<long long> g_debugPauseEndTime;

#pragma pack(push, 1)
typedef struct ReplayHeader
{
    long long startTime;
    long long endTime;
} stReplayHeader;

typedef struct DataSet
{
    long long curTime;
    int fullSize;
    int id;
    std::string data;
    char padding;                       // ETX(3)
} stDataSet;                 // 8 + 4 + 4 + x + 1 (17+)

typedef struct __myPoint
{
    double latitude;
    double longitude;
} stPoint;

#pragma pack(pop)

// 큐의 데이터를 저장할 때 구분자(3, ETX)를 넣고
// 특정 시간대부터 불러오고 싶을 때 구분자를 기준으로 그다음 8바이트만큼 가져와서 시간을 훑은다음
// 위치찾아서 해당 위치의 5개 전 데이터부터 큐에 쌓아놓는다 (첫 시작시 waypoint 5개 보내주기 위해서)

/*
녹화여부가 체크되어있으면
시나리오 배포를 할 때 대표파일을 생성하고 startTime은 0, endTime은 0, 시나리오내용 기록
훈련을 시작할 때 시간을 대표파일에 덮어쓴다
훈련을 종료할 때 시간을 대표파일에 덮어쓴다

재생할 때 대표파일
*/

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64KB
    std::atomic<int> m_curBufferSize;
    std::queue<stDataSet> m_buffer1;
    std::queue<stDataSet> m_buffer2;
    std::queue<stDataSet>* mp_currentBuffer;            // 수신받을때마다 전처리하여 채우고 일정량이 채워지면 교체한다
    std::queue<stDataSet>* mp_processingBuffer;          // 비우면서 파일에 기록한다
    int m_maxQueueCount = 0;

    long long m_startTime;                         // 수신을 시작한 ms시간 (나중에 수신종료 명령시 현재시간에서 startTime을 빼서 playTime을 기록)
    long long m_endTime;
    std::atomic<bool> m_endFlag;                               // 종료 플래그
    std::mutex m_mutex;             // 버퍼 스왑을 위한 뮤텍스
    std::condition_variable m_cv;

    std::thread m_sendThread;
    std::thread m_addBufThread;
    std::thread m_saveThread;
    int m_testerMsg = 0;
    std::string m_filePath;
    std::string m_scenarioContent;

    DoubleBufferRecvAndSaveFile() :
        m_maxBufferSize(65536)
    {
        initPlay();
    }

    ~DoubleBufferRecvAndSaveFile() {
        // 스레드를 안전하게 종료
        m_endFlag.store(true);
    }

    // 시나리오를 배포할 때 
    // 배포하기 전 녹화여부가 체크되어 있으면 해당 함수를 동작시켜서 배포할 시나리오 내용을 받아놓는다
    void initPlay()
    {
        // 버퍼를 비운다
        if (!m_buffer1.empty()) {
            std::queue<stDataSet> temp;       // 껍데기
            m_buffer1.swap(temp);
        }
        if (!m_buffer2.empty()) {
            std::queue<stDataSet> temp;       // 껍데기
            m_buffer2.swap(temp);
        }

        // 사이즈를 비운다
        m_curBufferSize = 0;
        // 중지플래그를 true로 초기화한다
        m_endFlag.store(true);

        // 버퍼를 연결한다
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;

        m_startTime = 0;                // 훈련 시작시간 초기화
        m_endTime = 0;                  // 훈련 종료시간 초기화
    }

    // 기록시작 (훈련 시작 시 시나리오 배포 때 정한 파일명과 훈련시작 시간을 받아온다)
    void startSave(std::string replayFileName, long long startTime)
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);

            //m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // 훈련 시작 시간
            m_startTime = startTime;
            stReplayHeader* p_header = new stReplayHeader;
            memset(p_header, 0, sizeof(stReplayHeader));
            p_header->startTime = m_startTime;
            p_header->endTime = 0;                              // 처음엔 0으로 넣기
            
            // 훈련 시작 배포전에 헤더파일을 먼저 저장한다.
            m_filePath = replayFileName + "_" + std::to_string(m_startTime) +"_st1" + ".dat";

            std::thread headerSaver([=]() {
                std::ofstream outFile(m_filePath, std::ios::binary);
                if (outFile.is_open())
                {
                    outFile.write(reinterpret_cast<const char*>(&p_header->startTime), sizeof(p_header->startTime));            // 훈련 시작시간 기록 (8바이트)
                    outFile.write(reinterpret_cast<const char*>(&p_header->endTime), sizeof(p_header->endTime));                // 임시값 기록 (8바이트)
                    outFile.close();
                }
            });

            headerSaver.join();

            delete p_header;
            

            // recv를 시작한다 (==> Queue 저장로직을 연다)
            std::thread([this] {this->addBufTimer(); }).detach();
            printf("플레이 시작!\n");
        }
        else
        {
            printf("플레이가 이미 실행 중\n");
        }
    }

    // 훈련중지 명령 받음 (훈련중지 배포하고 헤더파일 덮어쓰고 훈련중지완료응답 프론트로)
    void endSave(long long endTime) 
    {
        // recv 멈추고
        if (m_endFlag == false)
        {
            m_endFlag.store(true);

            m_endTime = endTime;                // 훈련 종료시간 받아오고 (훈련중지 배포 후의 시간을 넣을 것)

            // m_processingBuffer가 모두 비워지는 것을 기다린다음
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this]() {return mp_processingBuffer->empty(); });
            }

            // m_currentBuffer를 모두 파일에 기록하고
            if (!mp_currentBuffer->empty())
            {
                swapBuffer();
                // 다시 m_processingBuffer가 모두 비워지는 것을 기다린다음
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this]() {return mp_processingBuffer->empty(); });
            }

            stReplayHeader* p_header = new stReplayHeader;
            memset(p_header, 0, sizeof(stReplayHeader));
            p_header->startTime = m_startTime;
            p_header->endTime = m_endTime;

            printf("시작시간: %lld\n", p_header->startTime);
            printf("종료시간: %lld\n", p_header->endTime);
            printf("총 플레이타임: %lld, 총 큐 개수: %d\n", m_endTime - m_startTime, m_maxQueueCount);

            // 맨 앞 헤더를 덮어쓴다
            std::ofstream headerWriter(m_filePath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
            if (!headerWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                headerWriter.seekp(sizeof(long long), std::ios::beg);             // 시작위치로부터 startTime만큼 이동
                headerWriter.write(reinterpret_cast<const char*>(&m_endTime), sizeof(m_endTime));           // 종료시간 덮어씀
                if (!headerWriter)
                {
                    printf("Failed to write data\n");
                    return;
                }
            }
            printf("플레이가 정상 중지\n");
        }
        else
        {
            printf("플레이가 실행중이 아님\n");
        }

    }

    void sendTask(const stDataSet& dataset)
    {
        // View로 보내는 작업을 처리한다
        printf("시간:%lld, 총크기:%d, 식별자:%d, 데이터:%s, 패딩:%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    // 수신테스트
    void addBufTimer()
    {
        // 별도의 쓰레드에서 버퍼를 채운다 (추후에는 수신받을때 바디사이즈와 시간과 id까지 기록해서 push)
        while (m_endFlag != true)
        {
            stDataSet dataset = { 0, 0, 0, "", 3 };
            std::string recvStr = "";

            if (m_testerMsg == 0)
            {
                dataset.id = 1;
                recvStr.assign(200, '1');
            }
            else if (m_testerMsg == 1)
            {
                dataset.id = 2;
                recvStr.assign(1651, '2');
            }
            else if (m_testerMsg == 2)
            {
                dataset.id = 44;
                recvStr.assign(6426, '4');
            }
            else if (m_testerMsg == 3)
            {
                dataset.id = 55;
                recvStr.assign(500, '5');
            }
            dataset.curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            dataset.fullSize = sizeof(dataset.curTime) + sizeof(dataset.fullSize) + sizeof(dataset.id) + sizeof(dataset.padding) + recvStr.size();          // 17 + x
            dataset.data.assign(recvStr);

            // 꼬이지 않았다면
            if (dataset.data != "")
            {
                m_curBufferSize += dataset.fullSize;         // 추가하고

                std::thread([this, dataset]() {this->sendTask(dataset); }).detach();

                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    mp_currentBuffer->push(dataset);               // 저장하고
                    m_maxQueueCount++;
                }

                if (m_curBufferSize >= (m_maxBufferSize * 0.9))
                {
                    swapBuffer();
                }

                m_testerMsg = (m_testerMsg + 1) % 4;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));            // 수신 임시모의
            }
        }
    }

    void fileSaver() {
        // 가득차서 swap된 pFileLoadBuffer버퍼는 별도의 쓰레드를 통해 파일에 기록한다
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream outFile(m_filePath.c_str(), std::ios::binary | std::ios::app | std::ios::out);
        if (!outFile.is_open()) {
            printf("Failed to open file for writing\n");
            return;
        }

        // 바이너리로 저장
        std::string binData;
        while (!mp_processingBuffer->empty())
        {
            binData.clear();
            const stDataSet& dataset = mp_processingBuffer->front();
            binData.assign(reinterpret_cast<const char*>(&dataset), 16);
            binData.append(dataset.data);
            binData.append(1, dataset.padding);
            outFile.write(binData.c_str(), dataset.fullSize);
            mp_processingBuffer->pop();
        }
        outFile.close();                // 파일 닫기
        m_cv.notify_one();
    }

    void swapBuffer() {
        // 90%이상 채워진 pRecvBuffer를 temp를 통해 교체한다
        std::lock_guard<std::mutex> lock(m_mutex);
        std::queue<stDataSet>* temp = mp_currentBuffer;
        mp_currentBuffer = mp_processingBuffer;
        mp_processingBuffer = temp;
        m_curBufferSize = 0;

        std::thread([this] {
            this->fileSaver();
            }).detach();
    }
};

// 시나리오와 전체 시간은 파일로드할 때 이미 보내놓았다
class DoubleBufferFileLoadAndReplay
{
public:
    const int m_maxBufferSize;             // 64KB
    int m_currentBufferSize;
    int m_processingBufferSize;
    std::queue<stDataSet> m_buffer1;
    std::queue<stDataSet> m_buffer2;
    std::queue<stDataSet>* mp_currentBuffer;                 // 비워지면 교체하고 파일에서 채워놓는다 (재현용)
    std::queue<stDataSet>* mp_processingBuffer;         // fileLoad 담당
    long long m_startTime;
    long long m_endTime;
    
    long long m_curPlayTime;                              // 경과시간 (ms)
    std::atomic<int> m_endReplayFlag;                        // 종료 플래그,, [0]시작, [1]중지, [2]일시정지
    
    std::mutex m_mutex;                             // 버퍼 스왚을 위한 뮤택스
    std::condition_variable m_cv;
    std::string m_filePath;                         // st1의 리플레이 파일 경로
    std::ifstream m_fileStream;
    stReplayHeader m_curHeader;
    
    int m_minSize;

    UINT m_timerInterval;
    TIMECAPS m_tc;
    UINT m_timerId;

    std::thread m_sendThread;
    std::thread m_useBufThread;
    std::thread m_loadThread;
    std::thread m_timerThread;

    // 2000ms마다 타이머를 통해 pReplayBuffer의 time값이 타이머의 시간보다 작거나 같은 값들을 순차적으로 모두 printf
    DoubleBufferFileLoadAndReplay() : m_maxBufferSize(65536)
    {
        m_curPlayTime = 0;
        m_timerInterval = 32;           // 32ms (최초속도)
        m_timerId = 0;
        if (timeGetDevCaps(&m_tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
        {
            printf("Failed to get timer capabilities.\n");
            return;
        }
        timeBeginPeriod(m_tc.wPeriodMin);

        initReplay();           // 기타 초기화
    }

    ~DoubleBufferFileLoadAndReplay() {
        if (m_timerId != 0)
        {
            timeKillEvent(m_timerId);
        }
        timeEndPeriod(m_tc.wPeriodMin);
        if (m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }

    void initReplay()
    {
        // 버퍼를 비운다
        if (!m_buffer1.empty())
        {
            std::queue<stDataSet> temp;
            m_buffer1.swap(temp);
        }
        if (!m_buffer2.empty())
        {
            std::queue<stDataSet> temp;
            m_buffer2.swap(temp);
        }

        // 변수들을 초기화한다
        m_currentBufferSize = 0;
        m_processingBufferSize = 0;
        m_endReplayFlag = 1;          // 중지상태
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        
        m_minSize = 17;
    }



    // 재생 시작 버튼 (중심파일의 시작시간과 종료시간을 받아온다)
    void startReplay()
    {
        // 일시중지였으면 이어서, 정지상태였으면 처음부터 버퍼읽기
        // 해당 구간의 시간 지점부터 데이터를 채워넣는다
        if (m_endReplayFlag == 1)               // 정지상태라면
        {
            g_debugReplayStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();        // 디버깅용
            m_endReplayFlag.store(0);           // 시작으로 변경

            // 초기화는 init 또는 정지버튼에서 한다
            //m_startTime = startTime;
            //m_endTime = endTime;
            m_curPlayTime = m_curHeader.startTime;

            m_fileStream.clear();
            m_fileStream.seekg(sizeof(stReplayHeader), std::ios_base::beg);         // 헤더만큼 건너뛰고
            fillBuffer();               // 버퍼채우기
            swapBuffer();           // 교체

            // 타이머 시작
            useBufferTimer();

            printf("재생 시작\n");
        }
        else if (m_endReplayFlag == 2)          // 일시정지 였다면
        {
            m_endReplayFlag.store(0);               // 재생으로 변경
            useBufferTimer();
            g_debugPauseEndTime.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - g_debugPauseTime);

            printf("일시정지 해제\n");
        }
        else
        {
            printf("이미 재생 중\n");
        }
    }

    // 정지 버튼
    void stopReplay()
    {
        if (m_endReplayFlag != 1)               // 정지가 아니었다면
        {
            m_endReplayFlag.store(1);               // 정지로
            // 타이머 종료
            timeKillEvent(m_timerId);
            m_curPlayTime = m_curHeader.startTime;              // 플레이시간 초기화

            // 버퍼와 버퍼사이즈 초기화
            if (!m_buffer1.empty())
            {
                std::queue<stDataSet> emptyQueue1;
                m_buffer1.swap(emptyQueue1);
            }
            m_currentBufferSize = 0;
            if (!m_buffer2.empty())
            {
                std::queue<stDataSet> emptyQueue2;
                m_buffer2.swap(emptyQueue2);
            }
            m_processingBufferSize = 0;
            initReplay();

            printf("재생 중지\n");
        }
        else
        {
            printf("이미 중지\n");
        }
    }

    void pauseReplay()
    {
        if (m_endReplayFlag == 0)               // 재생 중 이었다면
        {
            m_endReplayFlag.store(2);               // 일시 정지로
            timeKillEvent(m_timerId);

            printf("재생 일시중지\n");
        }
        else
        {
            printf("재생 중이 아닙니다\n");
        }
    }

    void speedReplay(int _speed)
    {
        UINT newInterval = m_timerInterval;

        if (_speed == 1)
        {
            newInterval = 32;
        }
        else if (_speed == 2)
        {
            newInterval = 16;
        }
        else if (_speed == 4)
        {
            newInterval = 8;
        }
        else if (_speed == 8)
        {
            newInterval = 4;
        }

        if (newInterval != m_timerInterval)
        {
            m_timerInterval = newInterval;
            if (m_timerId != 0)
            {
                timeKillEvent(m_timerId);
                m_timerId = timeSetEvent(m_timerInterval, 1, TimerUseBuffer, reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC);
            }
        }
    }

    /*
        정지 또는 일시정지가 아니라면 일시정지하고 데이터비운다음
        선택한 구간에 대해 시작으로부터 x미리초 후 위치로 받아서
        재생시간 변경 후
        저장해놓은 시작시간으로부터 더한 다음
        이전 5개의 waypoint 그리기를 위해 해당 지점으로 백한다음 버퍼에 받아온다음 스왚
        스타트
    */
    void moveReplay(long long sectionTime)
    {
        int curReplayFlag = m_endReplayFlag.load();         // 기존 상태 백업 ([0]재생, [1]중지, [2]일시중지)

        // 재생장치를 일시정지시킨다
        m_endReplayFlag.store(2);           // 일시정지로
        timeKillEvent(m_timerId);           // 타이머 중지

        // 버퍼와 버퍼사이즈 초기화
        if (!m_buffer1.empty())
        {
            std::queue<stDataSet> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        m_currentBufferSize = 0;
        if (!m_buffer2.empty())
        {
            std::queue<stDataSet> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }
        m_processingBufferSize = 0;
        m_curPlayTime = sectionTime + m_curHeader.startTime;                // 플레이타임을 이동시점으로

        // 파일의 처음위치부터 해당 시간(m_curPlayTime >= curTime)의 위치를 찾을때까지
        // 파일을 읽어 상태메시지는 큐에 담고 wayPoint는 스택에 담는다. (버퍼는 사용하지 않는다)
        // wayPoint는 5개만 보낸 후 비운다
        // 상태메시지는 일일히 하나씩 쭉 보낸다 (65535 오버플로우 방지)
        m_fileStream.clear();
        m_fileStream.seekg(sizeof(stReplayHeader), std::ios_base::beg);         // 헤더만큼 건너뛰고

        // 스택과 큐를 생성한다
        std::queue<std::string> stateLog;
        std::stack<stPoint> wayPointLog;
        std::stack<stPoint> wayPoint5;          // 역순 백업용 5개

        while (!m_fileStream.eof()) 
        {
            // 파일에서 DataSet을 읽는다
             
            //if (m_curPlayTime > 읽은시간) 
            {
                // wayPoint(특정ID에 대해 wayPoint추출)와 상태메시지(메시지의 뒤에서부터 100바이트 읽기)를 쌓는다
                
            }
            //else
            {
                // 도달
                // wayPoint 마지막 5개만 뽑아서 백업 후 원본 비우고 하나씩 보낸다
                // 상태메시지는 비워질때까지 계속 보낸다 
                break;
            }
        }

        // 버퍼를 채운다
        fillBuffer();               // 버퍼채우기
        swapBuffer();           // 교체

        // 기존 상태가 재생중이었다면 재생을 시작을 시작한다
        m_endReplayFlag = curReplayFlag;
        if (curReplayFlag == 0)
        {
            // 여기서 재생장치를 실행시킨다
            m_timerId = timeSetEvent(m_timerInterval, 1, TimerUseBuffer, reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC);
        }
    }

    // 스테이션 1의 파일만 불러온다
    void loadFile(const std::string& _fullPath)
    {
        // 해당 파일명을 멤버변수에 저장하고
        // 헤더정보를 통해 멤버변수를 셋팅하고

        m_processingBufferSize = 0;

        // 파일이 있는지 체크하고 파일이 있으면 파일로부터 mp_currentBuffer 버퍼를 채운다
        if (std::filesystem::exists(_fullPath))
        {
            // 이전 파일이 다른 파일이었다면 닫는다
            if (m_filePath != _fullPath && m_filePath != "")
            {
                m_fileStream.close();
            }

            // 열려있는 파일이 없다면 연다
            if (!m_fileStream.is_open())
            {
                m_fileStream.open(_fullPath, std::ios::binary);
                m_filePath = _fullPath;

                // 헤더시간(플레이타임) 읽기
                memset(&m_curHeader, 0, sizeof(stReplayHeader));
                char* temp = new char[sizeof(stReplayHeader) + 1] {0};
                m_fileStream.read(temp, sizeof(stReplayHeader));
                memcpy(&m_curHeader, temp, sizeof(stReplayHeader));             // 바이너리니깐 memcpy로 읽는다

                m_curPlayTime = m_curHeader.startTime;          // 현재시간에 시작시간을 담아놓는다
                m_startTime = m_curHeader.startTime;
                m_endTime = m_curHeader.endTime;

                convertMsToTime(m_curHeader.endTime - m_curHeader.startTime);           // 디버깅용

                delete[] temp;
            }
            else
            {
                printf("파일 열기 실패\n");
            }
        }
        else
        {
            printf("경로에 파일이 없습니다.\n");
        }
    }

    void convertMsToTime(long long milliseconds) {
        const long long msPerSecond = 1000;
        const long long msPerMinute = msPerSecond * 60;
        const long long msPerHour = msPerMinute * 60;
        const long long msPerDay = msPerHour * 24;

        long long days = milliseconds / msPerDay;
        milliseconds -= days * msPerDay;
        long long hours = milliseconds / msPerHour;
        milliseconds -= hours * msPerHour;
        long long minutes = milliseconds / msPerMinute;
        milliseconds -= minutes * msPerMinute;
        long long seconds = milliseconds / msPerSecond;
        milliseconds -= seconds * msPerSecond;

        printf("%lld일 %lld시간 %lld분 %lld초 %lld밀리초\n", days, hours, minutes, seconds, milliseconds);
    }


    void fillBuffer()
    {
        // 프로세싱 버퍼를 파일로부터 채운다
        if (!m_fileStream.is_open())
        {
            printf("파일 열기 실패\n");
            m_endReplayFlag.store(1);         // 중지
        }

        while (m_fileStream && (m_maxBufferSize - m_processingBufferSize) > m_minSize)
        {
            char dumpFrontData[17]{ 0 };             // 앞쪽 3개만
            stDataSet tempFormat = { 0, 0, 0, "", 3 };           // 시간, 사이즈, 아이디
            if (!m_fileStream.read(dumpFrontData, (std::min)(16, m_maxBufferSize - m_processingBufferSize)))
            {
                break;          // 파일에서 더이상 읽을 데이터가 없음
            }

            // 파일 끝에 도달했거나, 지정된 크기만큼 읽지못했다면 중단
            int bytesRead = m_fileStream.gcount();              // 실제로 읽은 바이트 수
            if (m_fileStream.eof())
            {
                break;
            }

            memcpy(&tempFormat, dumpFrontData, 16);

            // 바디와 패딩을 추가로 읽는다
            int bodySize = tempFormat.fullSize - 17;
            if (bodySize > 0)
            {
                tempFormat.data.resize(bodySize);
                m_fileStream.read(&tempFormat.data[0], bodySize);
            }
            else
            {
                printf("바디사이즈0\n");
            }

            m_fileStream.read(&tempFormat.padding, 1);
            if (m_fileStream.eof())
            {
                break;
            }

            mp_processingBuffer->push(tempFormat);
            m_processingBufferSize += tempFormat.fullSize;
        }

        printf("현재 %d, 예비 %d\n", m_currentBufferSize, m_processingBufferSize);
    }

    void swapBuffer() {
        // 전부 사용된 버퍼는 교체한다 (단, 상대 버퍼도 비어있으면 스왚하지않는다)
        if (!mp_processingBuffer->empty())
        {
            std::queue<stDataSet>* temp = mp_currentBuffer;
            mp_currentBuffer = mp_processingBuffer;
            mp_processingBuffer = temp;
            m_currentBufferSize = m_processingBufferSize;

            // 스왚 후 내용을 비운다
            std::queue<stDataSet> initQueue;
            mp_processingBuffer->swap(initQueue);
            m_processingBufferSize = 0;
            // 스왚했으면 별도쓰레드에서 내용을 채운다

            std::thread([this] {
                this->fillBuffer();
            }).detach();

        }


        if (mp_processingBuffer->empty() && mp_currentBuffer->empty())
        {
            // 두 버퍼가 모두 비었으니 재현을 종료한다
            m_endReplayFlag.store(1);         // 중지
        }
    }

    void sendTask(const stDataSet& dataset)
    {
        printf("시간:%lld, 총크기:%d, 식별자:%d, 데이터:%s, 패딩%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    static void CALLBACK TimerUseBuffer(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
    {
        DoubleBufferFileLoadAndReplay* p_instance = reinterpret_cast<DoubleBufferFileLoadAndReplay*>(dwUser);
        if (p_instance)
        {
            p_instance->TimerUseBufferImpl(uTimerID, uMsg, dwUser, dw1, dw2);
        }
    }

    void TimerUseBufferImpl(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
    {
        m_curPlayTime += 32;        // 이미 시작시간이 담겨있음
        if (m_endReplayFlag == 0)           // 재생 중
        {        
            std::lock_guard<std::mutex> lock(m_mutex);
            stDataSet popData = { 0, 0, 0, "", 3 };
            if (!mp_currentBuffer->empty())          // 비어있지 않은 동안에
            {
                popData = mp_currentBuffer->front();
                if (popData.curTime <= m_curPlayTime)
                {
                    mp_currentBuffer->pop();
                    sendTask(popData);
                }

                if (mp_currentBuffer->empty())      // 현재꺼 비워짐
                {
                    if (!mp_processingBuffer->empty())            // 다른쪽은 채워져있니?
                    {
                        swapBuffer();           // 채워져있으면 스왚
                    }
                    else
                    {
                        // 둘다 전부 사용했다 (끝)
                        timeKillEvent(m_timerId);
                        m_curPlayTime = m_curHeader.startTime;
                        initReplay();
                        g_debugReplayEndTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        printf("원본 시간 [총플레이타임: %lld]\n", m_curHeader.endTime - m_curHeader.startTime);
                    }
                }
            }
        }
    }

    void useBufferTimer() {
        m_timerId = timeSetEvent(m_timerInterval, 1, TimerUseBuffer, reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC);
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    DoubleBufferFileLoadAndReplay* replayer = new DoubleBufferFileLoadAndReplay;
    replayer->loadFile("C:\\data\\scenario_1716890355512_st1.dat");             // req수신함수

    while (true) {
        std::string input = "";
        printf("[0]프로그램 종료, [1]Save시작, [2]Save종료, [3]Replay시작, [4]Replay중지, [5]Replay일시중지, [6]Replay구간이동, [7]배속1, [8]배속2, [9]배속4, [10]배속8\n");
        std::getline(std::cin, input);
        if (input == "0")
        {
            long long totalTime = g_debugReplayEndTime - g_debugReplayStartTime;
            for (long long value : g_debugPauseEndTime) {
                totalTime -= value;
            }
            replayer->convertMsToTime(totalTime);            // 리플레이 경과시간
            printf("경과시간: %lld\n", totalTime);
            
            break;
        }
        else if (input == "1")
        {
            std::string distributionName = "C:\\data\\scenario";
            long long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // 훈련 시작 시간
            receiver->startSave(distributionName, time);          // 여러번해도 버그가 없어야함
        }
        else if (input == "2")
        {
            long long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // 훈련 종료 시간
            receiver->endSave(time);
        }
        else if (input == "3")
        {
            replayer->startReplay();
        }
        else if (input == "4")
        {
            replayer->stopReplay();
            g_debugReplayEndTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }
        else if (input == "5")
        {
            replayer->pauseReplay();
            g_debugPauseTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }
        else if (input == "6")
        {
            long long clickTime = 3000;                 // 시작으로부터 3초 후 위치
            replayer->moveReplay(clickTime);           // 시작시간(ms) + ms - waypoint5개
        }
        else if (input == "7")
        {
            replayer->speedReplay(1);             // 1배속 재생
        }
        else if (input == "8")
        {
            replayer->speedReplay(2);             // 2배속 재생
        }
        else if (input == "9")
        {
            replayer->speedReplay(4);             // 4배속 재생
        }
        else if (input == "10")
        {
            replayer->speedReplay(8);             // 8배속 재생
        }
    }

    //delete replayer;
    delete receiver;
}
