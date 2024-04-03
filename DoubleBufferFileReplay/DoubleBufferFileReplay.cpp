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
    int id;                                    // �Ʒü���&��� ����ĺ���
    std::string data;                   // ���̳ʸ� ������ (����ü)
    char padding;                       // ETX(3)
} stDataFormat;                 // 8 + 4 + 4 + x + 1 (17+)
#pragma pack(pop)

// ť�� �����͸� ������ �� ������(3, ETX)�� �ְ�
// Ư�� �ð������ �ҷ����� ���� �� �����ڸ� �������� �״��� 8����Ʈ��ŭ �����ͼ� �ð��� ��������
// ��ġã�Ƽ� �ش� ��ġ�� 5�� �� �����ͺ��� ť�� �׾Ƴ��´� (ù ���۽� waypoint 5�� �����ֱ� ���ؼ�)
// �ش� �ð��뺸�� ������ ť�����ʹ� ������ �۽��Ѵ�

class DoubleBufferFileLoadAndReplay
{
public:
    int m_maxBufferSize = 65536;             // 64kb
    std::queue<std::string> m_buffer1;
    std::queue<std::string> m_buffer2;
    std::queue<std::string>* m_currentBuffer;                 // ������� ��ü�ϰ� ���Ͽ��� ä�����´� (���� ������)
    std::queue<std::string>* m_processingBuffer;         // �̸� ä�����´� (fileLoad)
    long long m_startTime;
    long long m_endTime;
    long long m_totalPlayTime;                       // ������ header (�� �����ð� ms����, ���� �Ǿտ� ����)
    long long m_timerTime;               // Ÿ�̸��� ����ð�
    std::atomic<bool> m_endFlag;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    

    // 2000ms���� Ÿ�̸Ӹ� ���� pReplayBuffer�� time���� Ÿ�̸��� �ð����� �۰ų� ���� ������ ���������� ��� printf
    DoubleBufferFileLoadAndReplay() {
        // pFileLoadBuffer �� �̸� ä������ pReplayBuffer�� swap�Ѵ�
        
    }
    
    ~DoubleBufferFileLoadAndReplay() {

    }

    void printTimer() {
        // ������ �����忡�� 2000ms ���� ����
        // Ÿ�̸�Ÿ�Ӻ��� ���� �ð� ���� pReplayBuffer�� ������ ���������� printf �ϰ�
        // ���� ��������� swap�Ѵ�

    }

    void fileloader() {
        // swap�� ����pFileLoadBuffer�� ������ �����忡�� ä�����´�
        // 

    }

    void swapBuffer() {
        // ���� ���� ���۴� ��ü�Ѵ�
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
    std::queue<std::string>* mp_currentBuffer;            // ���Ź��������� ��ó���Ͽ� ä��� �������� ä������ ��ü�Ѵ�
    std::queue<std::string>* mp_processingBuffer;          // ���鼭 ���Ͽ� ����Ѵ�
    
    long long m_startTime;                         // ������ ������ ms�ð� (���߿� �������� ��ɽ� ����ð����� startTime�� ���� playTime�� ���)
    std::atomic<long long> m_endTime;
    long long m_playTime;                       // ���Ͽ� ����� �� ����ð�, �������� ��� ���۸� ���Ϸ� �����ϰ� �� �տ� ����Ѵ�, ms ����
    std::atomic<long long> m_timerTime;               // Ÿ�̸��� ����ð� (30ms ����, 30Hz�̻�)
    std::atomic<bool> m_endFlag;                               // ���� �÷���
    std::mutex m_mutex;             // ���� ������ ���� ���ؽ�
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
        // �����带 �����ϰ� ����

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
        // ���� �� ������ �����ϰ� �� �� �κп� �Ʒýð��� 8����Ʈ��ŭ 0���� ����Ѵ� (��ο� ������ ������ ����, ������ ������ ������ ������ ����)
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

        // recv�� �����Ѵ� (==> Queue ��������� ����)
        m_addBufThread = std::thread([this] {this->addBufTimer(); });
        m_addBufThread.detach();
    }

    // Recv���� ��� ����
    void endSave() {
        // recv ���߰�
        m_endFlag.store(true);

        // end_time ����ϰ�
        m_endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        // m_processingBuffer�� ��� ������� ���� ��ٸ�����
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]() {return mp_processingBuffer->empty(); });
        }
        
        // m_currentBuffer�� ��� ���Ͽ� ����ϰ�
        if (!mp_currentBuffer->empty())
        {
            swapBuffer();
            // �ٽ� m_processingBuffer�� ��� ������� ���� ��ٸ�����
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]() {return mp_processingBuffer->empty(); });
        }
        
        // m_playTime���� �� �� 8����Ʈ�� �����
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
        // ������ �����忡�� ���۸� ä��� (���Ŀ��� ���Ź����� �ٵ������� �ð��� id���� ����ؼ� push)
        while (m_endFlag != true)
        {
            std::string recvStr(1651, '1');             // ���Ÿ��� (�Ű�����)
            m_curBufferSize += recvStr.size();         // �߰��ϰ�
            //printf("%s\n", recvStr.c_str());               // �۽��ϰ�
            //sendTask(recvStr);
            m_sendThread = std::thread([this, recvStr]() {this->sendTask(recvStr); });
            m_sendThread.detach();

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                mp_currentBuffer->push(recvStr);               // �����ϰ�
            }

            if (m_curBufferSize >= (m_maxBufferSize * 0.9))
            {
                swapBuffer();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));            // ���� �ӽø���
        }
    }

    void fileSaver() {
        // �������� swap�� pFileLoadBuffer���۴� ������ �����带 ���� ���Ͽ� ����Ѵ�
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
        outFile.close();                // ���� �ݱ�
        m_cv.notify_one();
    }

    void swapBuffer() {
        // 90%�̻� ä���� pRecvBuffer�� temp�� ���� ��ü�Ѵ�
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
        printf("0.���α׷� ����, 1.Recv����, 2.Replay����\n");
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
