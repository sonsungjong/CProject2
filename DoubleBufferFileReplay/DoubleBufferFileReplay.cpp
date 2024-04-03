#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>
#include <atomic>
#include <mutex>
#include <fstream>

#pragma pack(push, 1)
typedef struct FileHeader
{
    long long startTime;
    long long endTime;
    long long totalPlayTime;                // ms
    char padding;               // ETX(3)
} stFileHeader;             // 25

typedef struct DataFormat
{
    long long curTime;                         // ms
    int fullSize;                               // 8 + 4 + 4 + x + 1
    int id;                                    // 훈련수행&재생 전용식별자
    std::string data;                   // 바이너리 데이터 (구조체)
    char padding;                       // ETX(3)
} stDataFormat;                 // 8 + 4 + 4 + x + 1 (17+)
#pragma pack(pop)

// 큐의 데이터를 저장할 때 구분자(3, ETX)를 넣고
// 특정 시간대부터 불러오고 싶을 때 구분자를 기준으로 그다음 8바이트만큼 가져와서 시간을 훑은다음
// 위치찾아서 해당 위치의 5개 전 데이터부터 큐에 쌓아놓는다 (첫 시작시 waypoint 5개 보내주기 위해서)
// 해당 시간대보다 이전인 큐데이터는 모조리 송신한다

class DoubleBufferFileLoadAndReplay
{
public:
    int m_maxBufferSize = 65536;             // 64kb
    std::queue<std::string> m_buffer1;
    std::queue<std::string> m_buffer2;
    std::queue<std::string>* m_currentBuffer;                 // 비워지면 교체하고 파일에서 채워놓는다 (수신 재현용)
    std::queue<std::string>* m_processingBuffer;         // 미리 채워놓는다 (fileLoad)
    long long m_startTime;
    long long m_endTime;
    long long m_totalPlayTime;                       // 파일의 header (총 재현시간 ms단위, 파일 맨앞에 있음)
    long long m_timerTime;               // 타이머의 경과시간
    std::atomic<bool> m_endFlag;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    

    // 2000ms마다 타이머를 통해 pReplayBuffer의 time값이 타이머의 시간보다 작거나 같은 값들을 순차적으로 모두 printf
    DoubleBufferFileLoadAndReplay() {
        // pFileLoadBuffer 를 미리 채워놓고 pReplayBuffer와 swap한다
        
    }
    
    ~DoubleBufferFileLoadAndReplay() {

    }

    void printTimer() {
        // 별도의 쓰레드에서 2000ms 마다 동작
        // 타이머타임보다 이전 시간 값인 pReplayBuffer의 값들을 순차적으로 printf 하고
        // 전부 사용했으면 swap한다

    }

    void fileloader() {
        // swap된 버퍼pFileLoadBuffer를 별도의 쓰레드에서 채워놓는다
        // 

    }

    void swapBuffer() {
        // 전부 사용된 버퍼는 교체한다
        std::queue<std::string>* temp = m_currentBuffer;
        m_currentBuffer = m_processingBuffer;
        m_processingBuffer = temp;
    }
};

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64kb
    std::atomic<int> m_curBufferSize;
    std::queue<std::string> m_buffer1;
    std::queue<std::string> m_buffer2;
    std::queue<std::string>* mp_currentBuffer;            // 수신받을때마다 전처리하여 채우고 일정량이 채워지면 교체한다
    std::queue<std::string>* mp_processingBuffer;          // 비우면서 파일에 기록한다
    
    long long m_startTime;                         // 수신을 시작한 ms시간 (나중에 수신종료 명령시 현재시간에서 startTime을 빼서 playTime을 기록)
    std::atomic<long long> m_endTime;
    long long m_playTime;                       // 파일에 기록할 총 경과시간, 마지막에 모든 버퍼를 파일로 저장하고 맨 앞에 기록한다, ms 단위
    std::atomic<long long> m_timerTime;               // 타이머의 경과시간 (30ms 단위, 30Hz이상)
    std::atomic<bool> m_endFlag;                               // 종료 플래그
    std::mutex m_mutex;             // 버퍼 스왑을 위한 뮤텍스
    std::condition_variable m_cv;

    std::thread m_sendThread;
    std::thread m_addBufThread;
    std::thread m_saveThread;

    DoubleBufferRecvAndSaveFile() :
        m_maxBufferSize(65536)
    {
        init();
    }

    ~DoubleBufferRecvAndSaveFile() {
        // 스레드를 안전하게 종료

    }

    void init()
    {
        if (!m_buffer1.empty()) {
            std::queue<std::string> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty()) {
            std::queue<std::string> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }
        m_curBufferSize = 0;
        m_endFlag.store(false);

        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        m_startTime = 0;
        m_timerTime = 0;
        m_endTime = 0;
        m_playTime = 0;

    }

    void startSave()
    {
        m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        // 폴더 및 파일을 생성하고 맨 앞 부분에 훈련시간을 8바이트만큼 0으로 기록한다 (경로에 폴더가 없으면 생성, 기존에 파일이 있으면 기존거 삭제)
        m_playTime = m_endTime - m_startTime;
        std::string filename = "C:\\UITCC\\scenario_" + std::to_string(m_startTime) + ".dat";
        std::ofstream timeWriter(filename.c_str());
        if (!timeWriter.is_open()) {
            printf("Failed to open file\n");
        }
        else {
            timeWriter.write(reinterpret_cast<const char*>(&m_playTime), sizeof(m_playTime));
            if (!timeWriter)
            {
                printf("Failed to write data\n");
            }

            timeWriter.close();
        }

        // recv를 시작한다 (==> Queue 저장로직을 연다)
        m_addBufThread = std::thread([this] {this->addBufTimer(); });
        m_addBufThread.detach();
    }

    // Recv중지 명령 받음
    void endSave() {
        // recv 멈추고
        m_endFlag.store(true);

        // end_time 계산하고
        m_endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

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
        
        // m_playTime으로 맨 앞 8바이트를 덮어쓴다
        m_playTime = m_endTime - m_startTime;
        std::string filename = "C:\\UITCC\\scenario_" + std::to_string(m_startTime) + ".dat";
        std::ofstream timeWriter(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
        if (!timeWriter.is_open()) {
            printf("Failed to open file\n");
        }
        else {
            timeWriter.seekp(0, std::ios::beg);
            timeWriter.write(reinterpret_cast<const char*>(&m_playTime), sizeof(m_playTime));
            if (!timeWriter)
            {
                printf("Failed to write data\n");
                return;
            }

            timeWriter.close();
        }

    }

    void sendTask(std::string data)
    {
        printf("%s\n", data.c_str());
    }

    void addBufTimer()
    {
        // 별도의 쓰레드에서 버퍼를 채운다 (추후에는 수신받을때 바디사이즈와 시간과 id까지 기록해서 push)
        while (m_endFlag != true)
        {
            std::string recvStr(1651, '1');             // 수신모의 (매개변수)
            m_curBufferSize += recvStr.size();         // 추가하고
            //printf("%s\n", recvStr.c_str());               // 송신하고
            //sendTask(recvStr);
            m_sendThread = std::thread([this, recvStr]() {this->sendTask(recvStr); });
            m_sendThread.detach();

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                mp_currentBuffer->push(recvStr);               // 저장하고
            }

            if (m_curBufferSize >= (m_maxBufferSize * 0.9))
            {
                swapBuffer();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));            // 수신 임시모의
        }
    }

    void fileSaver() {
        // 가득차서 swap된 pFileLoadBuffer버퍼는 별도의 쓰레드를 통해 파일에 기록한다
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string filename = "C:\\UITCC\\scenario_" + std::to_string(m_startTime) + ".dat";
        std::ofstream outFile(filename.c_str(), std::ios::binary | std::ios::app | std::ios::out);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }

        while (!mp_processingBuffer->empty())
        {
            const std::string& data = mp_processingBuffer->front();
            outFile.write(data.c_str(), data.size());
            mp_processingBuffer->pop();
        }
        outFile.close();                // 파일 닫기
        m_cv.notify_one();
    }

    void swapBuffer() {
        // 90%이상 채워진 pRecvBuffer를 temp를 통해 교체한다
        std::lock_guard<std::mutex> lock(m_mutex);
        std::queue<std::string>* temp = mp_currentBuffer;
        mp_currentBuffer = mp_processingBuffer;
        mp_processingBuffer = temp;
        m_curBufferSize = 0;

        m_saveThread = std::thread([this] {this->fileSaver(); });
        m_saveThread.detach();
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    receiver->startSave();

    while (true) {
        std::string input = "";
        printf("0.프로그램 종료, 1.Recv중지, 2.Replay중지\n");
        std::getline(std::cin, input);
        if (input == "0")
        {
            break;
        }
        else if (input == "1")
        {
            receiver->endSave();
        }
    }

    delete receiver;
}
