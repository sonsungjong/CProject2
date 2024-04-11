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

#pragma pack(push, 1)
typedef struct FileHeader
{
    long long startTime;
    long long endTime;
    long long totalPlayTime;                            // ms
    char scenarioContent[65510];               // 시나리오 배포하는 내용을 저장 (여러개 로드할 수 있기 때문) 
    char padding;               // ETX(3)
} stFileHeader;             // 65535

typedef struct DataFormat
{
    long long curTime;
    int fullSize;
    int id;
    std::string data;
    char padding;                       // ETX(3)
} stDataFormat;                 // 8 + 4 + 4 + x + 1 (17+)
#pragma pack(pop)

// 큐의 데이터를 저장할 때 구분자(3, ETX)를 넣고
// 특정 시간대부터 불러오고 싶을 때 구분자를 기준으로 그다음 8바이트만큼 가져와서 시간을 훑은다음
// 위치찾아서 해당 위치의 5개 전 데이터부터 큐에 쌓아놓는다 (첫 시작시 waypoint 5개 보내주기 위해서)
// 해당 시간대보다 이전인 큐데이터는 모조리 송신한다

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64KB
    std::atomic<int> m_curBufferSize;
    std::queue<stDataFormat> m_buffer1;
    std::queue<stDataFormat> m_buffer2;
    std::queue<stDataFormat>* mp_currentBuffer;            // 수신받을때마다 전처리하여 채우고 일정량이 채워지면 교체한다
    std::queue<stDataFormat>* mp_processingBuffer;          // 비우면서 파일에 기록한다
    int m_maxQueueCount = 0;

    long long m_startTime;                         // 수신을 시작한 ms시간 (나중에 수신종료 명령시 현재시간에서 startTime을 빼서 playTime을 기록)
    long long m_endTime;
    long long m_playTime;                       // 파일에 기록할 총 경과시간, 마지막에 모든 버퍼를 파일로 저장하고 맨 앞에 기록한다, ms 단위
    //std::atomic<long long> m_timerTime;               // 타이머의 경과시간 (32ms 단위, 약 30Hz)
    std::atomic<bool> m_endFlag;                               // 종료 플래그
    std::mutex m_mutex;             // 버퍼 스왑을 위한 뮤텍스
    std::condition_variable m_cv;

    char m_scenarioPathList[_MAX_PATH][10];             // 최대 10개 중첩
    std::thread m_sendThread;
    std::thread m_addBufThread;
    std::thread m_saveThread;
    int m_testerMsg = 0;

    DoubleBufferRecvAndSaveFile() :
        m_maxBufferSize(65536)
    {
        initPlay();
    }

    ~DoubleBufferRecvAndSaveFile() {
        // 스레드를 안전하게 종료
        m_endFlag.store(true);
    }

    void initPlay()
    {
        if (!m_buffer1.empty()) {
            std::queue<stDataFormat> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty()) {
            std::queue<stDataFormat> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }
        m_curBufferSize = 0;
        m_endFlag.store(true);

        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        m_startTime = 0;
        //m_timerTime = 0;
        m_endTime = 0;
        m_playTime = 0;
        memset(m_scenarioPathList, 0, sizeof(m_scenarioPathList));
    }

    // 
    void initPlay(const char(&scenarioPathList)[_MAX_PATH][10])
    {
        if (!m_buffer1.empty()) {
            std::queue<stDataFormat> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty()) {
            std::queue<stDataFormat> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }
        m_curBufferSize = 0;
        m_endFlag.store(true);

        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        m_startTime = 0;
        m_endTime = 0;
        m_playTime = 0;
        memcpy(m_scenarioPathList, scenarioPathList, sizeof(m_scenarioPathList));

    }


    // 훈련시작
    void startSave()
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);
            m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // 폴더 및 파일을 생성하고 맨 앞 부분에 헤더만큼 임시값을 기록한다 (경로에 폴더가 없으면 생성, 기존에 파일이 있으면 기존거 삭제)
            stFileHeader* header = new stFileHeader;
            memset(header, 0, sizeof(stFileHeader));
            header->padding = 3;

            std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
            std::ofstream timeWriter(filename.c_str());
            if (!timeWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                timeWriter.write(reinterpret_cast<const char*>(header), sizeof(stFileHeader));               // 일단 기록해놓기 (추후 스타트시간이 0이면 깨진 파일)
                if (!timeWriter)
                {
                    printf("Failed to write data\n");
                }

                timeWriter.close();
            }

            // recv를 시작한다 (==> Queue 저장로직을 연다)
            m_addBufThread = std::thread([this] {this->addBufTimer(); });
            m_addBufThread.detach();
            delete header;
            printf("플레이 시작!\n");
        }
        else
        {
            printf("플레이가 이미 실행 중\n");
        }
    }

    // 훈련중지 명령 받음
    void endSave() {
        // recv 멈추고
        if (m_endFlag == false)
        {
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
            m_playTime = m_endTime - m_startTime;

            // 맨 앞 헤더를 덮어쓴다
            stFileHeader* header = new stFileHeader;
            memset(header, 0, sizeof(stFileHeader));
            header->padding = 3;
            header->startTime = m_startTime;
            header->endTime = m_endTime;
            header->totalPlayTime = m_playTime;
            printf("총 플레이타임: %lld, 총 큐 개수: %d\n", m_playTime, m_maxQueueCount);

            std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
            std::ofstream timeWriter(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
            if (!timeWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                timeWriter.seekp(0, std::ios::beg);
                timeWriter.write(reinterpret_cast<const char*>(header), sizeof(stFileHeader));
                if (!timeWriter)
                {
                    printf("Failed to write data\n");
                    return;
                }

                timeWriter.close();
                printf("저장완료\n");
            }
            printf("플레이가 정상 중지\n");
        }
        else
        {
            printf("플레이가 실행중이 아님\n");
        }

    }

    void sendTask(const stDataFormat& dataset)
    {
        // View로 보내는 작업을 처리한다
        printf("시간:%lld, 총크기:%d, 식별자:%d, 데이터:%s, 패딩:%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    void addBufTimer()
    {
        // 별도의 쓰레드에서 버퍼를 채운다 (추후에는 수신받을때 바디사이즈와 시간과 id까지 기록해서 push)
        while (m_endFlag != true)
        {
            stDataFormat dataset = { 0, 0, 0, "", 3 };
            std::string recvStr = "";
            if (m_testerMsg == 0)
            {
                recvStr.assign(200, '1');
                dataset.id = 1;
            }
            else if (m_testerMsg == 1)
            {
                recvStr.assign(1651, '2');
                dataset.id = 2;
            }
            else if (m_testerMsg == 2)
            {
                recvStr.assign(6426, '4');
                dataset.id = 44;
            }
            else if (m_testerMsg == 3)
            {
                recvStr.assign(500, '5');
                dataset.id = 55;
            }
            dataset.curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            dataset.fullSize = 17 + recvStr.size();
            dataset.data.assign(recvStr);


            // 꼬이지 않았다면
            if (dataset.data != "")
            {
                m_curBufferSize += dataset.fullSize;         // 추가하고

                m_sendThread = std::thread([this, dataset]() {this->sendTask(dataset); });
                m_sendThread.detach();
                //sendTask(dataset);

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
        std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
        std::ofstream outFile(filename.c_str(), std::ios::binary | std::ios::app | std::ios::out);
        if (!outFile.is_open()) {
            printf("Failed to open file for writing\n");
            return;
        }

        // 바이너리로 저장
        std::string binData;
        while (!mp_processingBuffer->empty())
        {
            binData.clear();
            const stDataFormat& dataset = mp_processingBuffer->front();
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
        std::queue<stDataFormat>* temp = mp_currentBuffer;
        mp_currentBuffer = mp_processingBuffer;
        mp_processingBuffer = temp;
        m_curBufferSize = 0;

        m_saveThread = std::thread([this] {
            this->fileSaver();
            });
        m_saveThread.detach();
    }
};

class DoubleBufferFileLoadAndReplay
{

public:
    const int m_maxBufferSize;             // 64KB
    int m_currentBufferSize;
    int m_processingBufferSize;
    std::queue<stDataFormat> m_buffer1;
    std::queue<stDataFormat> m_buffer2;
    std::queue<stDataFormat>* mp_currentBuffer;                 // 비워지면 교체하고 파일에서 채워놓는다 (재현용)
    std::queue<stDataFormat>* mp_processingBuffer;         // fileLoad 담당
    long long m_startTime;
    long long m_endTime;
    long long m_totalPlayTime;                       // 파일의 header (총 재현시간 ms단위, 파일 맨앞에 있음)
    long long m_curPlayTime;                              // 경과시간 (ms)
    std::atomic<int> m_endFlag;                        // 종료 플래그,, [0]시작, [1]중지, [2]일시정지
    std::mutex m_mutex;                             // 버퍼 스왚을 위한 뮤택스
    std::condition_variable m_cv;
    std::string m_filePath;                         // 대상 파일경로
    std::ifstream m_fileStream;
    long long m_speed;
    stFileHeader m_curHeader;
    int m_minSize;

    std::thread m_sendThread;
    std::thread m_useBufThread;
    std::thread m_loadThread;
    std::thread m_timerThread;

    // 2000ms마다 타이머를 통해 pReplayBuffer의 time값이 타이머의 시간보다 작거나 같은 값들을 순차적으로 모두 printf
    DoubleBufferFileLoadAndReplay() : m_maxBufferSize(65536)
    {
        initReplay();
    }

    ~DoubleBufferFileLoadAndReplay() {
        if (m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }

    void initReplay()
    {
        // m_buffer1을 비운다
        // m_buffer2를 비운다
        if (!m_buffer1.empty())
        {
            std::queue<stDataFormat> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty())
        {
            std::queue<stDataFormat> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }

        // 변수들을 초기화한다
        m_currentBufferSize = 0;
        m_processingBufferSize = 0;
        m_endFlag = 1;          // 중지상태
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        m_startTime = 0;
        m_curPlayTime = 0;
        m_endTime = 0;
        m_totalPlayTime = 0;
        m_speed = 1;
        m_minSize = 17;
    }

    void startReplayTimer()
    {
        while (m_endFlag != 1)             // 재생중이면
        {
            if (m_endFlag == 2)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;               // 다른 대기방법 알아보기
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
            m_curPlayTime += (32*m_speed);
        }
    }

    void startReplay()
    {
        // 일시중지였으면 이어서, 정지상태였으면 처음부터 버퍼읽기
        // 해당 구간의 시간 지점부터 데이터를 채워넣는다
        if (m_endFlag == 1)
        {
            m_endFlag.store(0);
            // 별도 쓰레드에서 버퍼를 사용시킨다
            //loadFile(m_filePath);               // 처음부터 재생
            if (!m_buffer1.empty())
            {
                std::queue<stDataFormat> emptyQueue1;
                m_buffer1.swap(emptyQueue1);
            }
            m_currentBufferSize = 0;
            if (!m_buffer2.empty())
            {
                std::queue<stDataFormat> emptyQueue2;
                m_buffer2.swap(emptyQueue2);
            }
            m_processingBufferSize = 0;
            //m_curPlayTime = m_curHeader.startTime;
            
            m_fileStream.clear();
            m_fileStream.seekg(sizeof(stFileHeader), std::ios_base::beg);
            std::cout << "position: " << m_fileStream.tellg() << std::endl;
            fillBuffer();
            swapBuffer();

            m_timerThread = std::thread([this] {
                this->startReplayTimer();
                });
            m_timerThread.detach();
            m_useBufThread = std::thread([this] {
                this->useBuffer();
                });
            m_useBufThread.detach();
            printf("재생 시작\n");
        }
        else if (m_endFlag == 2)
        {
            m_endFlag.store(0);

            printf("일시정지 해제\n");
        }
        else
        {
            printf("이미 재생 중\n");
        }
    }

    void stopReplay()
    {
        if (m_endFlag != 1)
        {
            m_endFlag.store(1);
            m_speed = 1;
            m_curPlayTime = 0;
            printf("재생 중지\n");
        }
        else
        {
            printf("이미 중지\n");
        }
    }

    void pauseReplay()
    {
        if (m_endFlag == 0)
        {
            m_endFlag.store(2);

            printf("재생 일시중지\n");
        }
        else
        {
            printf("재생 중이 아닙니다\n");
        }
    }

    void speedReplay(int _speed)
    {
        m_speed = _speed;           // 1, 2, 4, 8
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

    }

    // (추후 : 사용된 모든 시나리오 내용을 프론트에 보내준다)
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
                memset(&m_curHeader, 0, sizeof(stFileHeader));
                char* temp = new char[sizeof(stFileHeader) + 1] {0};
                m_fileStream.read(temp, sizeof(stFileHeader));
                memcpy(&m_curHeader, temp, sizeof(stFileHeader));             // 바이너리니깐 memcpy로 읽는다

                m_curPlayTime = m_curHeader.startTime;
                convertMsToTime(m_curHeader.totalPlayTime);

                //fillBuffer();
                //swapBuffer();

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
            m_endFlag.store(1);         // 중지
        }

        while (m_fileStream && (m_maxBufferSize - m_processingBufferSize) > m_minSize)
        {
            char dumpFrontData[17]{ 0 };             // 앞쪽 3개만
            stDataFormat tempFormat = { 0, 0, 0, "", 3 };           // 시간, 사이즈, 아이디
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
            std::queue<stDataFormat>* temp = mp_currentBuffer;
            mp_currentBuffer = mp_processingBuffer;
            mp_processingBuffer = temp;
            m_currentBufferSize = m_processingBufferSize;

            // 스왚 후 내용을 비운다
            std::queue<stDataFormat> initQueue;
            mp_processingBuffer->swap(initQueue);
            m_processingBufferSize = 0;
            // 스왚했으면 별도쓰레드에서 내용을 채운다
            m_loadThread = std::thread([this] {
                this->fillBuffer();
            });
            m_loadThread.detach();
        }


        if(mp_processingBuffer->empty() && mp_currentBuffer->empty())
        {
            // 두 버퍼가 모두 비었으니 재현을 종료한다
            m_endFlag.store(1);         // 중지
        }
    }

    void sendTask(const stDataFormat& dataset)
    {
        printf("시간:%lld, 총크기:%d, 식별자:%d, 데이터:%s, 패딩%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    void useBuffer() {
        // (추후: 별도의 쓰레드에서 32ms 마다 동작 타이머타임보다 이전 시간 값인 pReplayBuffer의 값들을 순차적으로 printf 하고)
        // 테스트용 : 별도 쓰레드에서 32ms마다 큐를 하나씩 꺼내서 printf (원래는 저장과 동일한 사이즈로 맞춰야함)
        while (m_endFlag != 1)
        {
            stDataFormat popData = { 0, 0, 0, "", 3 };
            if (m_endFlag == 0)
            {
                // 추후에는 타이머보다 이하인 시간은 일괄 전송하다가 넘어가면 pop안하고 전송안하는 것으로 넘기기
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    while (!mp_currentBuffer->empty())
                    {
                        popData = mp_currentBuffer->front();
                        if (popData.curTime < m_curPlayTime)
                        {
                            m_curPlayTime = popData.curTime;
                            mp_currentBuffer->pop();
                            sendTask(popData);
                        }
                        else
                        {
                            break;
                        }
                        //m_sendThread = std::thread([this, result]() {this->sendTask(result); });
                        //m_sendThread.detach();
                    }
                }

                //std::this_thread::sleep_for(std::chrono::milliseconds(10));

            }
            else if (m_endFlag == 2)
            {
                while (m_endFlag == 2)
                {
                    // 일시정지
                    std::this_thread::sleep_for(std::chrono::milliseconds(32));
                }
                // 일시정지 해제시
                // HW장치도 일시정지 해제
            }
            else if (m_endFlag == 1)
            {
                printf("====재생 중지====\n");
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                printf("이건 뭐지??\n");
            }

            if (mp_currentBuffer->empty())
            {
                // 다른쪽은 채워져있니?
                if (!mp_processingBuffer->empty())
                {
                    swapBuffer();
                }
                else
                {
                    // 둘다 전부 사용했다 (끝)
                    m_endFlag.store(1);
                    m_speed = 1;
                    printf("재생 완료 [총플레이타임: %lld]\n", m_curPlayTime - m_curHeader.startTime);
                    printf("원본 시간 [총플레이타임: %lld]\n", m_curHeader.totalPlayTime);
                }
            }
        }
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    DoubleBufferFileLoadAndReplay* replayer = new DoubleBufferFileLoadAndReplay;
    replayer->loadFile("C:\\data\\scenario_1712554792301.dat");             // req수신함수

    while (true) {
        std::string input = "";
        printf("[0]프로그램 종료, [1]Save시작, [2]Save종료, [3]Replay시작, [4]Replay중지, [5]Replay일시중지, [6]Replay배속8배, [7]Replay구간이동, [8]Replay배속1배\n");
        std::getline(std::cin, input);
        if (input == "0")
        {
            break;
        }
        else if (input == "1")
        {
            receiver->startSave();          // 여러번해도 버그가 없어야함
        }
        else if (input == "2")
        {
            receiver->endSave();            // 여러번해도 버그가 없어야함
        }
        else if (input == "3")
        {
            replayer->startReplay();
        }
        else if (input == "4")
        {
            replayer->stopReplay();
        }
        else if (input == "5")
        {
            replayer->pauseReplay();
        }
        else if (input == "6")
        {
            replayer->speedReplay(8);             // 8배속 재생
        }
        else if (input == "7")
        {
            //long long clickTime = 3000;                 // 시작으로부터 3초 후 위치
            //replayer->moveReplay(clickTime);           // 시작시간(ms) + ms - waypoint5개
        }
        else if (input == "8")
        {
            //replayer->speedReplay(1);             // 1배속 재생
        }
    }

    //delete replayer;
    delete receiver;
}
