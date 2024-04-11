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
    char scenarioContent[65510];               // �ó����� �����ϴ� ������ ���� (������ �ε��� �� �ֱ� ����) 
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

// ť�� �����͸� ������ �� ������(3, ETX)�� �ְ�
// Ư�� �ð������ �ҷ����� ���� �� �����ڸ� �������� �״��� 8����Ʈ��ŭ �����ͼ� �ð��� ��������
// ��ġã�Ƽ� �ش� ��ġ�� 5�� �� �����ͺ��� ť�� �׾Ƴ��´� (ù ���۽� waypoint 5�� �����ֱ� ���ؼ�)
// �ش� �ð��뺸�� ������ ť�����ʹ� ������ �۽��Ѵ�

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64KB
    std::atomic<int> m_curBufferSize;
    std::queue<stDataFormat> m_buffer1;
    std::queue<stDataFormat> m_buffer2;
    std::queue<stDataFormat>* mp_currentBuffer;            // ���Ź��������� ��ó���Ͽ� ä��� �������� ä������ ��ü�Ѵ�
    std::queue<stDataFormat>* mp_processingBuffer;          // ���鼭 ���Ͽ� ����Ѵ�
    int m_maxQueueCount = 0;

    long long m_startTime;                         // ������ ������ ms�ð� (���߿� �������� ��ɽ� ����ð����� startTime�� ���� playTime�� ���)
    long long m_endTime;
    long long m_playTime;                       // ���Ͽ� ����� �� ����ð�, �������� ��� ���۸� ���Ϸ� �����ϰ� �� �տ� ����Ѵ�, ms ����
    //std::atomic<long long> m_timerTime;               // Ÿ�̸��� ����ð� (32ms ����, �� 30Hz)
    std::atomic<bool> m_endFlag;                               // ���� �÷���
    std::mutex m_mutex;             // ���� ������ ���� ���ؽ�
    std::condition_variable m_cv;

    char m_scenarioPathList[_MAX_PATH][10];             // �ִ� 10�� ��ø
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
        // �����带 �����ϰ� ����
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


    // �Ʒý���
    void startSave()
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);
            m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // ���� �� ������ �����ϰ� �� �� �κп� �����ŭ �ӽð��� ����Ѵ� (��ο� ������ ������ ����, ������ ������ ������ ������ ����)
            stFileHeader* header = new stFileHeader;
            memset(header, 0, sizeof(stFileHeader));
            header->padding = 3;

            std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
            std::ofstream timeWriter(filename.c_str());
            if (!timeWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                timeWriter.write(reinterpret_cast<const char*>(header), sizeof(stFileHeader));               // �ϴ� ����س��� (���� ��ŸƮ�ð��� 0�̸� ���� ����)
                if (!timeWriter)
                {
                    printf("Failed to write data\n");
                }

                timeWriter.close();
            }

            // recv�� �����Ѵ� (==> Queue ��������� ����)
            m_addBufThread = std::thread([this] {this->addBufTimer(); });
            m_addBufThread.detach();
            delete header;
            printf("�÷��� ����!\n");
        }
        else
        {
            printf("�÷��̰� �̹� ���� ��\n");
        }
    }

    // �Ʒ����� ��� ����
    void endSave() {
        // recv ���߰�
        if (m_endFlag == false)
        {
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
            m_playTime = m_endTime - m_startTime;

            // �� �� ����� �����
            stFileHeader* header = new stFileHeader;
            memset(header, 0, sizeof(stFileHeader));
            header->padding = 3;
            header->startTime = m_startTime;
            header->endTime = m_endTime;
            header->totalPlayTime = m_playTime;
            printf("�� �÷���Ÿ��: %lld, �� ť ����: %d\n", m_playTime, m_maxQueueCount);

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
                printf("����Ϸ�\n");
            }
            printf("�÷��̰� ���� ����\n");
        }
        else
        {
            printf("�÷��̰� �������� �ƴ�\n");
        }

    }

    void sendTask(const stDataFormat& dataset)
    {
        // View�� ������ �۾��� ó���Ѵ�
        printf("�ð�:%lld, ��ũ��:%d, �ĺ���:%d, ������:%s, �е�:%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    void addBufTimer()
    {
        // ������ �����忡�� ���۸� ä��� (���Ŀ��� ���Ź����� �ٵ������� �ð��� id���� ����ؼ� push)
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


            // ������ �ʾҴٸ�
            if (dataset.data != "")
            {
                m_curBufferSize += dataset.fullSize;         // �߰��ϰ�

                m_sendThread = std::thread([this, dataset]() {this->sendTask(dataset); });
                m_sendThread.detach();
                //sendTask(dataset);

                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    mp_currentBuffer->push(dataset);               // �����ϰ�
                    m_maxQueueCount++;
                }

                if (m_curBufferSize >= (m_maxBufferSize * 0.9))
                {
                    swapBuffer();
                }

                m_testerMsg = (m_testerMsg + 1) % 4;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));            // ���� �ӽø���
            }
        }
    }

    void fileSaver() {
        // �������� swap�� pFileLoadBuffer���۴� ������ �����带 ���� ���Ͽ� ����Ѵ�
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
        std::ofstream outFile(filename.c_str(), std::ios::binary | std::ios::app | std::ios::out);
        if (!outFile.is_open()) {
            printf("Failed to open file for writing\n");
            return;
        }

        // ���̳ʸ��� ����
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
        outFile.close();                // ���� �ݱ�
        m_cv.notify_one();
    }

    void swapBuffer() {
        // 90%�̻� ä���� pRecvBuffer�� temp�� ���� ��ü�Ѵ�
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
    std::queue<stDataFormat>* mp_currentBuffer;                 // ������� ��ü�ϰ� ���Ͽ��� ä�����´� (������)
    std::queue<stDataFormat>* mp_processingBuffer;         // fileLoad ���
    long long m_startTime;
    long long m_endTime;
    long long m_totalPlayTime;                       // ������ header (�� �����ð� ms����, ���� �Ǿտ� ����)
    long long m_curPlayTime;                              // ����ð� (ms)
    std::atomic<int> m_endFlag;                        // ���� �÷���,, [0]����, [1]����, [2]�Ͻ�����
    std::mutex m_mutex;                             // ���� ������ ���� ���ý�
    std::condition_variable m_cv;
    std::string m_filePath;                         // ��� ���ϰ��
    std::ifstream m_fileStream;
    long long m_speed;
    stFileHeader m_curHeader;
    int m_minSize;

    std::thread m_sendThread;
    std::thread m_useBufThread;
    std::thread m_loadThread;
    std::thread m_timerThread;

    // 2000ms���� Ÿ�̸Ӹ� ���� pReplayBuffer�� time���� Ÿ�̸��� �ð����� �۰ų� ���� ������ ���������� ��� printf
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
        // m_buffer1�� ����
        // m_buffer2�� ����
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

        // �������� �ʱ�ȭ�Ѵ�
        m_currentBufferSize = 0;
        m_processingBufferSize = 0;
        m_endFlag = 1;          // ��������
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
        while (m_endFlag != 1)             // ������̸�
        {
            if (m_endFlag == 2)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;               // �ٸ� ����� �˾ƺ���
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
            m_curPlayTime += (32*m_speed);
        }
    }

    void startReplay()
    {
        // �Ͻ����������� �̾, �������¿����� ó������ �����б�
        // �ش� ������ �ð� �������� �����͸� ä���ִ´�
        if (m_endFlag == 1)
        {
            m_endFlag.store(0);
            // ���� �����忡�� ���۸� ����Ų��
            //loadFile(m_filePath);               // ó������ ���
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
            printf("��� ����\n");
        }
        else if (m_endFlag == 2)
        {
            m_endFlag.store(0);

            printf("�Ͻ����� ����\n");
        }
        else
        {
            printf("�̹� ��� ��\n");
        }
    }

    void stopReplay()
    {
        if (m_endFlag != 1)
        {
            m_endFlag.store(1);
            m_speed = 1;
            m_curPlayTime = 0;
            printf("��� ����\n");
        }
        else
        {
            printf("�̹� ����\n");
        }
    }

    void pauseReplay()
    {
        if (m_endFlag == 0)
        {
            m_endFlag.store(2);

            printf("��� �Ͻ�����\n");
        }
        else
        {
            printf("��� ���� �ƴմϴ�\n");
        }
    }

    void speedReplay(int _speed)
    {
        m_speed = _speed;           // 1, 2, 4, 8
    }

    /*
        ���� �Ǵ� �Ͻ������� �ƴ϶�� �Ͻ������ϰ� �����ͺ�����
        ������ ������ ���� �������κ��� x�̸��� �� ��ġ�� �޾Ƽ�
        ����ð� ���� ��
        �����س��� ���۽ð����κ��� ���� ����
        ���� 5���� waypoint �׸��⸦ ���� �ش� �������� ���Ѵ��� ���ۿ� �޾ƿ´��� ����
        ��ŸƮ
    */
    void moveReplay(long long sectionTime)
    {

    }

    // (���� : ���� ��� �ó����� ������ ����Ʈ�� �����ش�)
    void loadFile(const std::string& _fullPath)
    {
        // �ش� ���ϸ��� ��������� �����ϰ�
        // ��������� ���� ��������� �����ϰ�
        
        m_processingBufferSize = 0;

        // ������ �ִ��� üũ�ϰ� ������ ������ ���Ϸκ��� mp_currentBuffer ���۸� ä���
        if (std::filesystem::exists(_fullPath))
        {
            // ���� ������ �ٸ� �����̾��ٸ� �ݴ´�
            if (m_filePath != _fullPath && m_filePath != "")
            {
                m_fileStream.close();
            }

            // �����ִ� ������ ���ٸ� ����
            if (!m_fileStream.is_open())
            {
                m_fileStream.open(_fullPath, std::ios::binary);
                m_filePath = _fullPath;

                // ����ð�(�÷���Ÿ��) �б�
                memset(&m_curHeader, 0, sizeof(stFileHeader));
                char* temp = new char[sizeof(stFileHeader) + 1] {0};
                m_fileStream.read(temp, sizeof(stFileHeader));
                memcpy(&m_curHeader, temp, sizeof(stFileHeader));             // ���̳ʸ��ϱ� memcpy�� �д´�

                m_curPlayTime = m_curHeader.startTime;
                convertMsToTime(m_curHeader.totalPlayTime);

                //fillBuffer();
                //swapBuffer();

                delete[] temp;
            }
            else
            {
                printf("���� ���� ����\n");
            }
        }
        else
        {
            printf("��ο� ������ �����ϴ�.\n");
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

        printf("%lld�� %lld�ð� %lld�� %lld�� %lld�и���\n", days, hours, minutes, seconds, milliseconds);
    }


    void fillBuffer()
    {
        // ���μ��� ���۸� ���Ϸκ��� ä���
        if (!m_fileStream.is_open())
        {
            printf("���� ���� ����\n");
            m_endFlag.store(1);         // ����
        }

        while (m_fileStream && (m_maxBufferSize - m_processingBufferSize) > m_minSize)
        {
            char dumpFrontData[17]{ 0 };             // ���� 3����
            stDataFormat tempFormat = { 0, 0, 0, "", 3 };           // �ð�, ������, ���̵�
            if (!m_fileStream.read(dumpFrontData, (std::min)(16, m_maxBufferSize - m_processingBufferSize)))
            {
                break;          // ���Ͽ��� ���̻� ���� �����Ͱ� ����
            }

            // ���� ���� �����߰ų�, ������ ũ�⸸ŭ �������ߴٸ� �ߴ�
            int bytesRead = m_fileStream.gcount();              // ������ ���� ����Ʈ ��
            if (m_fileStream.eof()) 
            { 
                break;
            }

            memcpy(&tempFormat, dumpFrontData, 16);

            // �ٵ�� �е��� �߰��� �д´�
            int bodySize = tempFormat.fullSize - 17;
            if (bodySize > 0)
            {
                tempFormat.data.resize(bodySize);
                m_fileStream.read(&tempFormat.data[0], bodySize);
            }
            else 
            {
                printf("�ٵ������0\n");
            }

            m_fileStream.read(&tempFormat.padding, 1);
            if (m_fileStream.eof())
            {
                break;
            }

            mp_processingBuffer->push(tempFormat);
            m_processingBufferSize += tempFormat.fullSize;
        }

        printf("���� %d, ���� %d\n", m_currentBufferSize, m_processingBufferSize);
    }

    void swapBuffer() {
        // ���� ���� ���۴� ��ü�Ѵ� (��, ��� ���۵� ��������� ���������ʴ´�)
        if (!mp_processingBuffer->empty())
        {
            std::queue<stDataFormat>* temp = mp_currentBuffer;
            mp_currentBuffer = mp_processingBuffer;
            mp_processingBuffer = temp;
            m_currentBufferSize = m_processingBufferSize;

            // ���� �� ������ ����
            std::queue<stDataFormat> initQueue;
            mp_processingBuffer->swap(initQueue);
            m_processingBufferSize = 0;
            // ���������� ���������忡�� ������ ä���
            m_loadThread = std::thread([this] {
                this->fillBuffer();
            });
            m_loadThread.detach();
        }


        if(mp_processingBuffer->empty() && mp_currentBuffer->empty())
        {
            // �� ���۰� ��� ������� ������ �����Ѵ�
            m_endFlag.store(1);         // ����
        }
    }

    void sendTask(const stDataFormat& dataset)
    {
        printf("�ð�:%lld, ��ũ��:%d, �ĺ���:%d, ������:%s, �е�%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    void useBuffer() {
        // (����: ������ �����忡�� 32ms ���� ���� Ÿ�̸�Ÿ�Ӻ��� ���� �ð� ���� pReplayBuffer�� ������ ���������� printf �ϰ�)
        // �׽�Ʈ�� : ���� �����忡�� 32ms���� ť�� �ϳ��� ������ printf (������ ����� ������ ������� �������)
        while (m_endFlag != 1)
        {
            stDataFormat popData = { 0, 0, 0, "", 3 };
            if (m_endFlag == 0)
            {
                // ���Ŀ��� Ÿ�̸Ӻ��� ������ �ð��� �ϰ� �����ϴٰ� �Ѿ�� pop���ϰ� ���۾��ϴ� ������ �ѱ��
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
                    // �Ͻ�����
                    std::this_thread::sleep_for(std::chrono::milliseconds(32));
                }
                // �Ͻ����� ������
                // HW��ġ�� �Ͻ����� ����
            }
            else if (m_endFlag == 1)
            {
                printf("====��� ����====\n");
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                printf("�̰� ����??\n");
            }

            if (mp_currentBuffer->empty())
            {
                // �ٸ����� ä�����ִ�?
                if (!mp_processingBuffer->empty())
                {
                    swapBuffer();
                }
                else
                {
                    // �Ѵ� ���� ����ߴ� (��)
                    m_endFlag.store(1);
                    m_speed = 1;
                    printf("��� �Ϸ� [���÷���Ÿ��: %lld]\n", m_curPlayTime - m_curHeader.startTime);
                    printf("���� �ð� [���÷���Ÿ��: %lld]\n", m_curHeader.totalPlayTime);
                }
            }
        }
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    DoubleBufferFileLoadAndReplay* replayer = new DoubleBufferFileLoadAndReplay;
    replayer->loadFile("C:\\data\\scenario_1712554792301.dat");             // req�����Լ�

    while (true) {
        std::string input = "";
        printf("[0]���α׷� ����, [1]Save����, [2]Save����, [3]Replay����, [4]Replay����, [5]Replay�Ͻ�����, [6]Replay���8��, [7]Replay�����̵�, [8]Replay���1��\n");
        std::getline(std::cin, input);
        if (input == "0")
        {
            break;
        }
        else if (input == "1")
        {
            receiver->startSave();          // �������ص� ���װ� �������
        }
        else if (input == "2")
        {
            receiver->endSave();            // �������ص� ���װ� �������
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
            replayer->speedReplay(8);             // 8��� ���
        }
        else if (input == "7")
        {
            //long long clickTime = 3000;                 // �������κ��� 3�� �� ��ġ
            //replayer->moveReplay(clickTime);           // ���۽ð�(ms) + ms - waypoint5��
        }
        else if (input == "8")
        {
            //replayer->speedReplay(1);             // 1��� ���
        }
    }

    //delete replayer;
    delete receiver;
}
