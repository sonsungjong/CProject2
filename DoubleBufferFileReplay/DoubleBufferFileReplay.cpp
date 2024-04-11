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

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64KB
    std::atomic<int> m_curBufferSize;
    std::queue<std::string> m_buffer1;
    std::queue<std::string> m_buffer2;
    std::queue<std::string>* mp_currentBuffer;            // ���Ź��������� ��ó���Ͽ� ä��� �������� ä������ ��ü�Ѵ�
    std::queue<std::string>* mp_processingBuffer;          // ���鼭 ���Ͽ� ����Ѵ�

    long long m_startTime;                         // ������ ������ ms�ð� (���߿� �������� ��ɽ� ����ð����� startTime�� ���� playTime�� ���)
    long long m_endTime;
    long long m_playTime;                       // ���Ͽ� ����� �� ����ð�, �������� ��� ���۸� ���Ϸ� �����ϰ� �� �տ� ����Ѵ�, ms ����
    //std::atomic<long long> m_timerTime;               // Ÿ�̸��� ����ð� (32ms ����, �� 30Hz)
    std::atomic<bool> m_endFlag;                               // ���� �÷���
    std::mutex m_mutex;             // ���� ������ ���� ���ؽ�
    std::condition_variable m_cv;

    std::thread m_sendThread;
    std::thread m_addBufThread;
    std::thread m_saveThread;

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
            std::queue<std::string> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty()) {
            std::queue<std::string> emptyQueue2;
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

    }

    void startSave()
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);
            m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // ���� �� ������ �����ϰ� �� �� �κп� �Ʒýð��� 8����Ʈ��ŭ 0���� ����Ѵ� (��ο� ������ ������ ����, ������ ������ ������ ������ ����)
            m_playTime = m_endTime - m_startTime;
            std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
            std::ofstream timeWriter(filename.c_str());
            if (!timeWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                timeWriter.write(reinterpret_cast<const char*>(&m_playTime), sizeof(m_playTime));               // �ϴ� 0���� ����س���
                if (!timeWriter)
                {
                    printf("Failed to write data\n");
                }

                timeWriter.close();
            }

            // recv�� �����Ѵ� (==> Queue ��������� ����)
            m_addBufThread = std::thread([this] {this->addBufTimer(); });
            m_addBufThread.detach();
            printf("�÷��� ����!\n");
        }
        else
        {
            printf("�÷��̰� �̹� ���� ��\n");
        }
    }

    // Recv���� ��� ����
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

            // m_playTime���� �� �� 8����Ʈ�� �����
            m_playTime = m_endTime - m_startTime;
            std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
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
                else 
                {
                    printf("�� �÷���Ÿ��: %lld\n", m_playTime);
                }

                timeWriter.close();
            }
            printf("�÷��̰� ���� ����\n");
        }
        else 
        {
            printf("�÷��̰� �������� �ƴ�\n");
        }

    }

    void sendTask(const std::string& data)
    {
        // View�� ������ �۾��� ó���Ѵ�
        printf("%s\n", data.c_str());
    }

    void addBufTimer()
    {
        // ������ �����忡�� ���۸� ä��� (���Ŀ��� ���Ź����� �ٵ������� �ð��� id���� ����ؼ� push)
        while (m_endFlag != true)
        {
            std::string recvStr(1651, '1');             // ���Ÿ��� (�Ű�����)
            m_curBufferSize += recvStr.size();         // �߰��ϰ�

            //m_sendThread = std::thread([this, recvStr]() {this->sendTask(recvStr); });
            //m_sendThread.detach();
            sendTask(recvStr);

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
        std::string filename = "C:\\data\\scenario_" + std::to_string(m_startTime) + ".dat";
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
    static constexpr int m_chunkSize = 2;                    // �ѹ��� ������ ������ (�׽�Ʈ�� ���� 2byte��)
    int m_currentBufferSize;
    std::queue<std::string> m_buffer1;
    std::queue<std::string> m_buffer2;
    std::queue<std::string>* mp_currentBuffer;                 // ������� ��ü�ϰ� ���Ͽ��� ä�����´� (������)
    std::queue<std::string>* mp_processingBuffer;         // fileLoad ���
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

    std::thread m_sendThread;
    std::thread m_useBufThread;
    std::thread m_loadThread;

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
            std::queue<std::string> emptyQueue1;
            m_buffer1.swap(emptyQueue1);
        }
        if (!m_buffer2.empty())
        {
            std::queue<std::string> emptyQueue2;
            m_buffer2.swap(emptyQueue2);
        }

        // �������� �ʱ�ȭ�Ѵ�
        m_currentBufferSize = 0;
        m_endFlag = 1;          // ��������
        mp_currentBuffer = &m_buffer1;
        mp_processingBuffer = &m_buffer2;
        m_startTime = 0;
        m_curPlayTime = 0;
        m_endTime = 0;
        m_totalPlayTime = 0;
        m_speed = 1;
    }

    void startReplay()
    {
        // �Ͻ����������� �̾, �������¿����� ó������ �����б�
        // �ش� ������ �ð� �������� �����͸� ä���ִ´�
        if (m_endFlag == 1)
        {
            m_endFlag.store(0);
            //loadFile(m_filePath);
            if (!m_buffer1.empty())
            {
                std::queue<std::string> emptyQueue1;
                m_buffer1.swap(emptyQueue1);
            }
            if (!m_buffer2.empty())
            {
                std::queue<std::string> emptyQueue2;
                m_buffer2.swap(emptyQueue2);
            }
            m_currentBufferSize = 0;
            m_fileStream.clear();
            m_fileStream.seekg(8, std::ios_base::beg);
            std::cout << "position: " << m_fileStream.tellg() << std::endl;
            fillBuffer();
            swapBuffer();
            
            // ���� �����忡�� ���۸� ����Ų��
            m_useBufThread = std::thread([this] {
                this->useBuffer();
            });
            m_useBufThread.detach();
            printf("��� ����\n");
        }
        else if(m_endFlag == 2)
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

    // (�ε忡���� ����� �д´�)
    void loadFile(const std::string& _fullPath)
    {
        // �ش� ���ϸ��� ��������� �����ϰ�
        m_filePath = _fullPath;
        // ��������� ���� ��������� �����ϰ�
        //if (!m_buffer1.empty())
        //{
        //    std::queue<std::string> emptyQueue1;
        //    m_buffer1.swap(emptyQueue1);
        //}
        //if (!m_buffer2.empty())
        //{
        //    std::queue<std::string> emptyQueue2;
        //    m_buffer2.swap(emptyQueue2);
        //}
        m_currentBufferSize = 0;

        // ������ �ִ��� üũ�ϰ� ������ ������ ���Ϸκ��� mp_currentBuffer ���۸� ä���
        if (std::filesystem::exists(m_filePath))
        {
            // ������ ���� ������ ������ �ݱ�
            if (m_fileStream.is_open())
            {
                m_fileStream.close();
            }

            m_fileStream.open(m_filePath, std::ios::binary);
            if (m_fileStream)
            {
                // ����ð�(�÷���Ÿ��) �б�
                char header[9] = { 0 };
                m_fileStream.read(header, 8);
                memcpy(&m_totalPlayTime, header, 8);             // ���̳ʸ��ϱ� memcpy�� �д´�

                // �ð� �˻�
                long long milliseconds = m_totalPlayTime;
                convertMsToTime(milliseconds);

                //char oneData[m_chunkSize+1] = { 0 };
                //while (m_currentBufferSize + m_chunkSize <= m_maxBufferSize)
                //{
                //    if (m_fileStream.read(oneData, m_chunkSize))
                //    {
                //        mp_currentBuffer->push(std::string(oneData, m_chunkSize));
                //        m_currentBufferSize += m_chunkSize;
                //    }
                //}
                //fillBuffer();
                //swapBuffer();
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
            m_fileStream.open(m_filePath, std::ios::binary);
            if (!m_fileStream)
            {
                printf("���� ���� ����\n");
                m_endFlag.store(1);         // ����
            }
        }

        char oneData[m_chunkSize] = { 0 };
        while ((m_currentBufferSize + m_chunkSize <= m_maxBufferSize))
        {
            if ((m_maxBufferSize - m_currentBufferSize) <= m_chunkSize || (m_maxBufferSize - m_currentBufferSize) <= 0)
            {
                break;
            }

            if (!m_fileStream.read(oneData, m_chunkSize))
            {
                break;          // ���Ͽ��� ���̻� ���� �����Ͱ� ����
            }

            // ���� ���� �����߰ų�, ������ ũ�⸸ŭ �������ߴٸ� �ߴ�
            if (m_fileStream.eof() || m_fileStream.gcount() < m_chunkSize) { break; }

            int bytesRead = m_fileStream.gcount();              // ������ ���� ����Ʈ ��
            mp_processingBuffer->push(std::string(oneData, m_chunkSize));
            m_currentBufferSize += m_chunkSize;

            if (m_fileStream.eof() || bytesRead < m_chunkSize)
            {
                break;
            }
        }
    }

    void swapBuffer() {
        // ���� ���� ���۴� ��ü�Ѵ� (��, ��� ���۵� ��������� ���������ʴ´�)
        if (!mp_processingBuffer->empty())
        {
            std::queue<std::string>* temp = mp_currentBuffer;
            mp_currentBuffer = mp_processingBuffer;
            mp_processingBuffer = temp;

            // ���� �� ������ ����
            std::queue<std::string> initQueue;
            mp_processingBuffer->swap(initQueue);

            // ���������� ���������忡�� ������ ä���
            m_loadThread = std::thread([this] {
                this->fillBuffer();
            });
            m_loadThread.detach();
        }
        else
        {
            // �� ���۰� ��� ������� ������ �����Ѵ�
            m_endFlag.store(1);         // ����
        }
    }

    void sendTask(const std::string& data)
    {
        printf("%s", data.c_str());
    }

    void useBuffer() {
        // (����: ������ �����忡�� 32ms ���� ���� Ÿ�̸�Ÿ�Ӻ��� ���� �ð� ���� pReplayBuffer�� ������ ���������� printf �ϰ�)
        // �׽�Ʈ�� : ���� �����忡�� 32ms���� ť�� �ϳ��� ������ printf (������ ����� ������ ������� �������)
        while (m_endFlag != 1)
        {
            std::string popData;
            if (m_endFlag == 0)
            {
                // ���Ŀ��� Ÿ�̸Ӻ��� ������ �ð��� �ϰ� �����ϴٰ� �Ѿ�� pop���ϰ� ���۾��ϴ� ������ �ѱ��
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    if (!mp_currentBuffer->empty())
                    {
                        popData = mp_currentBuffer->front();
                        mp_currentBuffer->pop();
                        sendTask(popData);
                        //m_sendThread = std::thread([this, result]() {this->sendTask(result); });
                        //m_sendThread.detach();
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(32 / m_speed));
            }
            else if (m_endFlag == 2)
            {
                while (m_endFlag == 2)
                {
                    // �Ͻ�����
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                // �Ͻ����� ������
                // HW��ġ�� �Ͻ����� ����
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
                    printf("��� �Ϸ�\n");
                }
            }
        }
    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;
    DoubleBufferFileLoadAndReplay* replayer = new DoubleBufferFileLoadAndReplay;
    replayer->loadFile("C:\\data\\scenario_1712287203219.dat");

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
            replayer->speedReplay(1);             // 1��� ���
        }
    }

    delete replayer;
    delete receiver;
}
