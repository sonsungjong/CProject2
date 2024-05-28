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
#include <deque>

long long g_debugReplayEndTime = 0;

#pragma pack(push, 1)
typedef struct ReplayHeader
{
    long long startTime;
    long long endTime;
} stReplayHeader;

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

/*
��ȭ���ΰ� üũ�Ǿ�������
�ó����� ������ �� �� ��ǥ������ �����ϰ� startTime�� 0, endTime�� 0, �ó��������� ���
�Ʒ��� ������ �� �ð��� ��ǥ���Ͽ� �����
�Ʒ��� ������ �� �ð��� ��ǥ���Ͽ� �����

����� �� ��ǥ����
*/

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
    std::atomic<bool> m_endFlag;                               // ���� �÷���
    std::mutex m_mutex;             // ���� ������ ���� ���ؽ�
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
        // �����带 �����ϰ� ����
        m_endFlag.store(true);
    }

    // �ó������� ������ �� 
    // �����ϱ� �� ��ȭ���ΰ� üũ�Ǿ� ������ �ش� �Լ��� ���۽��Ѽ� ������ �ó����� ������ �޾Ƴ��´�
    void initPlay()
    {
        // ���۸� ����
        if (!m_buffer1.empty()) {
            std::queue<stDataFormat> temp;       // ������
            m_buffer1.swap(temp);
        }
        if (!m_buffer2.empty()) {
            std::queue<stDataFormat> temp;       // ������
            m_buffer2.swap(temp);
        }

        // ����� ����
        m_curBufferSize = 0;
        // �����÷��׸� true�� �ʱ�ȭ�Ѵ�
        m_endFlag.store(true);

        // ���۸� �����Ѵ�
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;

        m_startTime = 0;                // �Ʒ� ���۽ð� �ʱ�ȭ
        m_endTime = 0;                  // �Ʒ� ����ð� �ʱ�ȭ
    }

    // ��Ͻ��� (�Ʒ� ���� �� �ó����� ���� �� ���� ���ϸ�� �Ʒý��� �ð��� �޾ƿ´�)
    void startSave(std::string replayFileName, long long startTime)
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);

            //m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // �Ʒ� ���� �ð�
            m_startTime = startTime;
            stReplayHeader* p_header = new stReplayHeader;
            memset(p_header, 0, sizeof(stReplayHeader));
            p_header->startTime = m_startTime;
            p_header->endTime = 0;                              // ó���� 0���� �ֱ�
            
            // �Ʒ� ���� �������� ��������� ���� �����Ѵ�.
            m_filePath = replayFileName + "_" + std::to_string(m_startTime) +"_st1" + ".dat";

            std::thread headerSaver([=]() {
                std::ofstream outFile(m_filePath, std::ios::binary);
                if (outFile.is_open())
                {
                    outFile.write(reinterpret_cast<const char*>(&p_header->startTime), sizeof(p_header->startTime));            // �Ʒ� ���۽ð� ��� (8����Ʈ)
                    outFile.write(reinterpret_cast<const char*>(&p_header->endTime), sizeof(p_header->endTime));                // �ӽð� ��� (8����Ʈ)
                    outFile.close();
                }
            });

            headerSaver.join();

            delete p_header;
            

            // recv�� �����Ѵ� (==> Queue ��������� ����)
            std::thread([this] {this->addBufTimer(); }).detach();
            printf("�÷��� ����!\n");
        }
        else
        {
            printf("�÷��̰� �̹� ���� ��\n");
        }
    }

    // �Ʒ����� ��� ���� (�Ʒ����� �����ϰ� ������� ����� �Ʒ������Ϸ����� ����Ʈ��)
    void endSave(long long endTime) 
    {
        // recv ���߰�
        if (m_endFlag == false)
        {
            m_endFlag.store(true);

            m_endTime = endTime;                // �Ʒ� ����ð� �޾ƿ��� (�Ʒ����� ���� ���� �ð��� ���� ��)

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

            stReplayHeader* p_header = new stReplayHeader;
            memset(p_header, 0, sizeof(stReplayHeader));
            p_header->startTime = m_startTime;
            p_header->endTime = m_endTime;

            printf("���۽ð�: %lld\n", p_header->startTime);
            printf("����ð�: %lld\n", p_header->endTime);
            printf("�� �÷���Ÿ��: %lld, �� ť ����: %d\n", m_endTime - m_startTime, m_maxQueueCount);

            // �� �� ����� �����
            std::ofstream headerWriter(m_filePath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
            if (!headerWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                headerWriter.seekp(sizeof(long long), std::ios::beg);             // ������ġ�κ��� startTime��ŭ �̵�
                headerWriter.write(reinterpret_cast<const char*>(&m_endTime), sizeof(m_endTime));           // ����ð� ���
                if (!headerWriter)
                {
                    printf("Failed to write data\n");
                    return;
                }
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

    // �����׽�Ʈ
    void addBufTimer()
    {
        // ������ �����忡�� ���۸� ä��� (���Ŀ��� ���Ź����� �ٵ������� �ð��� id���� ����ؼ� push)
        while (m_endFlag != true)
        {
            stDataFormat dataset = { 0, 0, 0, "", 3 };
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

            // ������ �ʾҴٸ�
            if (dataset.data != "")
            {
                m_curBufferSize += dataset.fullSize;         // �߰��ϰ�

                std::thread([this, dataset]() {this->sendTask(dataset); }).detach();

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
        std::ofstream outFile(m_filePath.c_str(), std::ios::binary | std::ios::app | std::ios::out);
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

        std::thread([this] {
            this->fileSaver();
            }).detach();
    }
};

// �ó������� ��ü �ð��� ���Ϸε��� �� �̹� �������Ҵ�
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
    //long long m_startTime;
    //long long m_endTime;
    long long m_totalPlayTime;                       // ������ header (�� �����ð� ms����, ���� �Ǿտ� ����)
    long long m_curPlayTime;                              // ����ð� (ms)
    std::atomic<int> m_endReplayFlag;                        // ���� �÷���,, [0]����, [1]����, [2]�Ͻ�����
    std::mutex m_mutex;                             // ���� ������ ���� ���ý�
    std::condition_variable m_cv;
    std::string m_filePath;                         // st1�� ���÷��� ���� ���
    std::ifstream m_fileStream;
    stReplayHeader m_curHeader;
    double m_speed;              // ���
    int m_minSize;

    std::thread m_sendThread;
    std::thread m_useBufThread;
    std::thread m_loadThread;
    std::thread m_timerThread;

    // 2000ms���� Ÿ�̸Ӹ� ���� pReplayBuffer�� time���� Ÿ�̸��� �ð����� �۰ų� ���� ������ ���������� ��� printf
    DoubleBufferFileLoadAndReplay() : m_maxBufferSize(65536)
    {
        initReplay();           // �ʱ�ȭ
    }

    ~DoubleBufferFileLoadAndReplay() {
        if (m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }

    void initReplay()
    {
        // ���۸� ����
        if (!m_buffer1.empty())
        {
            std::queue<stDataFormat> temp;
            m_buffer1.swap(temp);
        }
        if (!m_buffer2.empty())
        {
            std::queue<stDataFormat> temp;
            m_buffer2.swap(temp);
        }

        // �������� �ʱ�ȭ�Ѵ�
        m_currentBufferSize = 0;
        m_processingBufferSize = 0;
        m_endReplayFlag = 1;          // ��������
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        //m_startTime = 0;
        //m_endTime = 0;
        m_curPlayTime = 0;
        m_totalPlayTime = 0;
        m_speed = 1.0;
        m_minSize = 17;
    }

    // �÷��̽ð� ����
    void startReplayTimer()
    {
        auto lastTime = std::chrono::steady_clock::now();
        while (m_endReplayFlag != 1)             // �������°� �ƴ϶��
        {
            if (m_endReplayFlag == 0)               // ������¶��
            {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
                lastTime = now;

                m_curPlayTime += (elapsed * m_speed);  // ��� �ð��� �ӵ��� ���Ͽ� ���� �ð��� ����

                std::this_thread::sleep_for(std::chrono::milliseconds(1));  // ��� ���
            }
            else if (m_endReplayFlag == 2)               // �Ͻ����� ���¶��
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));          // �ٸ� ����� �˾ƺ���
            }
        }
    }

    // ��� ���� ��ư (�߽������� ���۽ð��� ����ð��� �޾ƿ´�)
    void startReplay()
    {
        // �Ͻ����������� �̾, �������¿����� ó������ �����б�
        // �ش� ������ �ð� �������� �����͸� ä���ִ´�
        if (m_endReplayFlag == 1)               // �������¶��
        {
            m_endReplayFlag.store(0);           // �������� ����

            // �ʱ�ȭ�� init �Ǵ� ������ư���� �Ѵ�
            //m_startTime = startTime;
            //m_endTime = endTime;

            m_fileStream.clear();
            m_fileStream.seekg(sizeof(stReplayHeader), std::ios_base::beg);         // �����ŭ �ǳʶٰ�
            std::cout << "position: " << m_fileStream.tellg() << std::endl;
            fillBuffer();               // ����ä���
            swapBuffer();           // ��ü

            // �÷���Ÿ�̸� ��� ����
            std::thread([this] {
                this->startReplayTimer();
                }).detach();

                // ���� ��� ����
                std::thread([this] {
                    this->useBuffer();
                    }).detach();

                    printf("��� ����\n");
        }
        else if (m_endReplayFlag == 2)          // �Ͻ����� ���ٸ�
        {
            m_endReplayFlag.store(0);               // ������� ����

            printf("�Ͻ����� ����\n");
        }
        else
        {
            printf("�̹� ��� ��\n");
        }
    }

    // ���� ��ư
    void stopReplay()
    {
        if (m_endReplayFlag != 1)               // ������ �ƴϾ��ٸ�
        {
            m_endReplayFlag.store(1);               // ������
            m_speed = 1;                        // �ӵ� �ʱ�ȭ
            m_curPlayTime = 0;              // �÷��̽ð� �ʱ�ȭ

            // ���ۿ� ���ۻ����� �ʱ�ȭ
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

            printf("��� ����\n");
        }
        else
        {
            printf("�̹� ����\n");
        }
    }

    void pauseReplay()
    {
        if (m_endReplayFlag == 0)               // ��� �� �̾��ٸ�
        {
            m_endReplayFlag.store(2);               // �Ͻ� ������

            printf("��� �Ͻ�����\n");
        }
        else
        {
            printf("��� ���� �ƴմϴ�\n");
        }
    }

    void speedReplay(int _speed)
    {
        m_speed = _speed;           // 1, 2, 4, 8 (��� ����)
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
        int curReplayFlag = m_endReplayFlag.load();         // ���� ���� ��� ([0]���, [1]����, [2]�Ͻ�����)
        if (m_endReplayFlag == 0)               // ��� ���̾��ٸ�
        {
            m_endReplayFlag.store(2);           // �Ͻ�������
        }

        // ���ۿ� ���ۻ����� �ʱ�ȭ
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
        m_curPlayTime = sectionTime;                // �÷���Ÿ���� �̵���������
        
        // ������ ó����ġ���� �ش� �ð�((m_startTime+m_curPlayTime) =< curTime)�� ��ġ�� ã�������� waypoint[5]�� ���¸޽������� ����Ѵ�
        // ã�� ��ġ�� seek �� �̵���Ų��

        // ���۸� ä���

        // ���� ���°� ������̾��ٸ� ����� �����ϰ� 
    }

    // �����̼� 1�� ���ϸ� �ҷ��´�
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
                memset(&m_curHeader, 0, sizeof(stReplayHeader));
                char* temp = new char[sizeof(stReplayHeader) + 1] {0};
                m_fileStream.read(temp, sizeof(stReplayHeader));
                memcpy(&m_curHeader, temp, sizeof(stReplayHeader));             // ���̳ʸ��ϱ� memcpy�� �д´�

                m_curPlayTime = m_curHeader.startTime;
                convertMsToTime(m_curHeader.endTime - m_curHeader.startTime);           // ������

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
            m_endReplayFlag.store(1);         // ����
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

            std::thread([this] {
                this->fillBuffer();
                }).detach();

        }


        if (mp_processingBuffer->empty() && mp_currentBuffer->empty())
        {
            // �� ���۰� ��� ������� ������ �����Ѵ�
            m_endReplayFlag.store(1);         // ����
        }
    }

    void sendTask(const stDataFormat& dataset)
    {
        printf("�ð�:%lld, ��ũ��:%d, �ĺ���:%d, ������:%s, �е�%d\n", dataset.curTime, dataset.fullSize, dataset.id, dataset.data.c_str(), dataset.padding);
    }

    void useBuffer() {
        // (����: ������ �����忡�� 32ms ���� ���� Ÿ�̸�Ÿ�Ӻ��� ���� �ð� ���� pReplayBuffer�� ������ ���������� printf �ϰ�)
        // �׽�Ʈ�� : ���� �����忡�� 32ms���� ť�� �ϳ��� ������ printf (������ ����� ������ ������� �������)
        while (m_endReplayFlag != 1)
        {
            stDataFormat popData = { 0, 0, 0, "", 3 };
            if (m_endReplayFlag == 0)
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
                        
                    }
                }

                //std::this_thread::sleep_for(std::chrono::milliseconds(10));

            }
            else if (m_endReplayFlag == 2)
            {
                while (m_endReplayFlag == 2)
                {
                    // �Ͻ�����
                    std::this_thread::sleep_for(std::chrono::milliseconds(32));
                }
                // �Ͻ����� ������
                // HW��ġ�� �Ͻ����� ����
            }
            else if (m_endReplayFlag == 1)
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
                    m_endReplayFlag.store(1);
                    m_speed = 1;
                    g_debugReplayEndTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    //printf("��� �Ϸ� [���÷���Ÿ��: %lld]\n", m_curPlayTime - m_curHeader.startTime);
                    printf("���� �ð� [���÷���Ÿ��: %lld]\n", m_curHeader.endTime - m_curHeader.startTime);
                }
            }
        }
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    DoubleBufferFileLoadAndReplay* replayer = new DoubleBufferFileLoadAndReplay;
    replayer->loadFile("C:\\data\\scenario_1716894176237_st1.dat");             // req�����Լ�
    long long startTime = 0;

    while (true) {
        std::string input = "";
        printf("[0]���α׷� ����, [1]Save����, [2]Save����, [3]Replay����, [4]Replay����, [5]Replay�Ͻ�����, [6]Replay�����̵�, [7]���1, [8]���2, [9]���4, [10]���8\n");
        std::getline(std::cin, input);
        if (input == "0")
        {
            replayer->convertMsToTime(g_debugReplayEndTime - startTime);            // ���÷��� ����ð�
            printf("����ð�: %lld\n", g_debugReplayEndTime - startTime);
            
            break;
        }
        else if (input == "1")
        {
            std::string distributionName = "C:\\data\\scenario";
            long long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // �Ʒ� ���� �ð�
            receiver->startSave(distributionName, time);          // �������ص� ���װ� �������
        }
        else if (input == "2")
        {
            long long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();           // �Ʒ� ���� �ð�
            receiver->endSave(time);
        }
        else if (input == "3")
        {
            startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();         // ���÷��� ���۽ð�
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
            //long long clickTime = 3000;                 // �������κ��� 3�� �� ��ġ
            //replayer->moveReplay(clickTime);           // ���۽ð�(ms) + ms - waypoint5��
        }
        else if (input == "7")
        {
            replayer->speedReplay(1);             // 1��� ���
        }
        else if (input == "8")
        {
            replayer->speedReplay(2);             // 2��� ���
        }
        else if (input == "9")
        {
            replayer->speedReplay(4);             // 4��� ���
        }
        else if (input == "10")
        {
            replayer->speedReplay(8);             // 8��� ���
        }
    }

    //delete replayer;
    delete receiver;
}
