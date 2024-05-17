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


// ���������� ������ ��������� �����ϰ� parser�� �����Ѵ�
std::string observation_scenario = R"(
<?xml version="1.0" encoding="EUC-KR"?>
<scenario name="UAEMain3" desc="UAEMain3" time="2024-04-17 15:15:18" los_check="0"><offense_scenario><ats id="1" kind="1" route_id="2" parameter="MIG-21MF" iff="1" time="0.000000" jamming="0" jammer_target_mfr_id="0" latitude="25.741282" longitude="56.487701" altitude="0.000000"><iff_data mode1="0001" mode2="0002" mode3="0003" mode4="NOTINTERROGATED" mode5="INTERROGATEDVALIDRESPONSE" mode5Pin="00034" mode5NO="1776"/><jammer id="0" type="0" range="0.000000" noj_param="NOJ_FLT"/></ats><ats id="3" kind="1" route_id="1" parameter="MIG-21MF" iff="1" time="0.000000" jamming="1" jammer_target_mfr_id="1" latitude="26.441418" longitude="55.061626" altitude="0.000000"><iff_data mode1="0003" mode2="0003" mode3="0003" mode4="INTERROGATEDVALIDRESPONSE" mode5="INTERROGATEDVALIDRESPONSE" mode5Pin="07125" mode5NO="0340"/><jammer id="1" type="3" range="0.000000" noj_param="NOJ_FLT"/></ats><route id="1"><waypoint id="1" type="4" mission="0" speed="30000000.000000" latitude="26.091873" longitude="55.166203" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="2" type="4" mission="0" speed="30000000.000000" latitude="24.877535" longitude="54.576759" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="3" type="4" mission="0" speed="30000000.000000" latitude="24.458519" longitude="54.938030" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="4" type="4" mission="0" speed="30000000.000000" latitude="24.129238" longitude="55.090145" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="5" type="4" mission="0" speed="30000000.000000" latitude="24.133577" longitude="55.251770" altitude="3000.000000" orbit_radius="0.000000"/></route><route id="2"><waypoint id="1" type="4" mission="0" speed="3000000.000000" latitude="24.756727" longitude="56.345093" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="2" type="4" mission="0" speed="30000000.000000" latitude="24.406584" longitude="56.240513" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="3" type="4" mission="0" speed="30000000.000000" latitude="24.081509" longitude="56.050369" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="4" type="4" mission="0" speed="30000000.000000" latitude="23.646791" longitude="55.941040" altitude="3000.000000" orbit_radius="0.000000"/><waypoint id="5" type="4" mission="0" speed="30000000.000000" latitude="23.507376" longitude="56.340340" altitude="3000.000000" orbit_radius="0.000000"/></route></offense_scenario><defense_scenario><ecs id="1" latitude="24.454191" longitude="55.441914" altitude="183.000000" main_fire_direction="0.000000" MDIL="22528" TADIL_J="36" use_order="1"><mfr id="1" latitude="24.454191" longitude="55.441914" altitude="183.000000" radar_mode="1"/><vls id="1" latitude="24.455748" longitude="55.442901" altitude="190.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/><vls id="2" latitude="24.454191" longitude="55.443889" altitude="165.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/><vls id="3" latitude="24.452634" longitude="55.442901" altitude="174.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/><vls id="4" latitude="24.452634" longitude="55.440926" altitude="172.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/><vls id="5" latitude="24.454191" longitude="55.439938" altitude="179.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/><vls id="6" latitude="24.455748" longitude="55.440926" altitude="195.000000" state="0" msl_link="1/1/1/1/1/1/1/1" msl_type="1/1/1/1/1/1/1/1" msl_SAU="1/1/1/1/1/1/1/1"/></ecs><icc id="1" latitude="24.782625" longitude="55.541737" altitude="194.000000" MDIL="5632" TADIL_J="97"/><ewr id="1" latitude="24.907719" longitude="55.922024" altitude="257.000000" radar_mode="1"/><c2 id="1" latitude="24.501781" longitude="55.793678" altitude="316.000000" MDIL="20480" TADIL_J="54"/></defense_scenario><region_scenario file_name="UAEMain3_region"/><draw_scenario file_name="UAEMain3_draw"/></scenario>
)";
std::string observation_scenario_name = "UAEMain3";             // �ó����� �̸� �����ؼ� ��������� ����

#pragma pack(push, 1)
typedef struct FileHeader
{
    long long startTime;
    long long endTime;
    char scenarioContent[60000];                    // ������ �ó����� ����
    char padding;           // 3
} stFileHeader;

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

class DoubleBufferRecvAndSaveFile
{
public:
    int m_maxBufferSize;             // 64KB
    std::atomic<int> m_curBufferSize;
    std::queue<stDataFormat> m_buffer1;
    std::queue<stDataFormat> m_buffer2;
    std::queue<stDataFormat> m_buffer_log;              // �ó����� ����� �α׿� ��������Ʈ ���
    std::queue<stDataFormat>* mp_currentBuffer;            // ���Ź��������� ��ó���Ͽ� ä��� �������� ä������ ��ü�Ѵ�
    std::queue<stDataFormat>* mp_processingBuffer;          // ���鼭 ���Ͽ� ����Ѵ�
    int m_maxQueueCount = 0;

    long long m_startTime;                         // ������ ������ ms�ð� (���߿� �������� ��ɽ� ����ð����� startTime�� ���� playTime�� ���)
    long long m_endTime;
    //long long m_playTime;                       // ���Ͽ� ����� �� ����ð�, �������� ��� ���۸� ���Ϸ� �����ϰ� �� �տ� ����Ѵ�, ms ����
    //std::atomic<long long> m_timerTime;               // Ÿ�̸��� ����ð� (32ms ����, �� 30Hz)
    std::atomic<bool> m_endFlag;                               // ���� �÷���
    std::mutex m_mutex;             // ���� ������ ���� ���ؽ�
    std::condition_variable m_cv;

    std::thread m_sendThread;
    std::thread m_addBufThread;
    std::thread m_saveThread;
    int m_testerMsg = 0;
    std::string m_filePath;
    std::string m_filePath_log;
    std::string m_scenarioContent;

    DoubleBufferRecvAndSaveFile() :
        m_maxBufferSize(65536)
    {
        
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
        if (!m_buffer_log.empty())
        {
            std::queue<stDataFormat> temp;       // ������
            m_buffer_log.swap(temp);
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
        //m_playTime = 0;                 // ��ü �÷���Ÿ�� �ʱ�ȭ
    }

    // �Ʒý��� (���������κ��� �ó����� ������ �޾Ƽ� ������Ͽ� �����Ѵ�, startSave�Ѵ��� �ó������� ����� ��)
    void startSave(const std::string& scenarioContent, std::string filePath = "C:\\UITCC\\scenario\\Replay\\Station1\\")
    {
        if (m_endFlag == true)
        {
            m_endFlag.store(false);

            std::string scenarioName = "";
            m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if (scenarioContent != "")
            {
                // �Ʒ� ���� �������� ��������� ���� �����Ѵ�.
                m_filePath = filePath + scenarioName + "_" + std::to_string(m_startTime) + ".dat";
                m_filePath_log = filePath + scenarioName + "_" + std::to_string(m_startTime) + "_logfile.dat";           // _logfile �� ���δ�
                stFileHeader* p_header = new stFileHeader;
                memset(p_header, 0, sizeof(stFileHeader));
                p_header->startTime = m_startTime;
                p_header->endTime = 0;                              // ó���� 0���� �ֱ�
                memset(p_header->scenarioContent, 0, sizeof(p_header->scenarioContent));
                memcpy(p_header->scenarioContent, scenarioContent.c_str(), scenarioContent.size());
                p_header->padding = 3;          // ETX

                std::thread headerSaver([=]() {
                    std::ofstream outFile(m_filePath_log, std::ios::binary);
                    if (outFile.is_open())
                    {
                        outFile.write(reinterpret_cast<const char*>(&p_header->startTime), sizeof(p_header->startTime));
                        outFile.write(reinterpret_cast<const char*>(&p_header->endTime), sizeof(p_header->endTime));
                        outFile.write(p_header->scenarioContent, sizeof(p_header->scenarioContent));
                        outFile.write(reinterpret_cast<const char*>(&p_header->padding), sizeof(p_header->padding));
                        outFile.close();
                    }
                });

                headerSaver.join();

                delete p_header;
            }

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
    void endSave() 
    {
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
            long long playTime = m_endTime - m_startTime;           // ������

            printf("�� �÷���Ÿ��: %lld, �� ť ����: %d\n", playTime, m_maxQueueCount);

            // �� �� ����� �����
            std::ofstream headerWriter(m_filePath_log.c_str(), std::ios::in | std::ios::out | std::ios::binary);
            if (!headerWriter.is_open()) {
                printf("Failed to open file\n");
            }
            else {
                headerWriter.seekp(m_startTime, std::ios::beg);             // ������ġ�κ��� startTime��ŭ �̵�
                headerWriter.write(reinterpret_cast<const char*>(&m_endTime), sizeof(m_endTime));
                if (!headerWriter)
                {
                    printf("Failed to write data\n");
                    return;
                }

                // ������ġ�κ��� stFileHeader��ŭ �̵�
                headerWriter.seekp(sizeof(stFileHeader), std::ios::beg);
                // �α׹��ۿ� �ִ� ������ ��� ����
                while (!m_buffer_log.empty())
                {
                    stDataFormat st = m_buffer_log.front();
                    std::string buffer = "";
                    buffer.resize(st.fullSize);
                    buffer.append(reinterpret_cast<const char*>(&st.curTime), sizeof(st.curTime));
                    buffer.append(reinterpret_cast<const char*>(&st.fullSize), sizeof(st.fullSize));
                    buffer.append(reinterpret_cast<const char*>(&st.id), sizeof(st.id));
                    buffer.append(st.data);
                    buffer.append(1, st.padding);

                    headerWriter.write(buffer.data(), buffer.size());
                    m_buffer_log.pop();
                }

                headerWriter.close();
                printf("���� ����Ϸ� %s\n", m_filePath_log.c_str());
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
            std::string displayLogMsg = "";
            std::string displayWaypoint = "";

            if (m_testerMsg == 0)
            {
                dataset.id = 1;
                displayLogMsg = "[1]���Խ��ϴ�.";
                recvStr.assign(200, '1');
                if (displayLogMsg != "")
                {
                    recvStr.append(displayLogMsg.c_str());              // display �޽����� ��� ����ü�� �߰��Ѵ�(append�� ����ü�� �ƴϴ� �ӽÿ�, ����Ʈ���� ������� ������ �α׸޽����� ���)
                }
            }
            else if (m_testerMsg == 1)
            {
                dataset.id = 2;
                recvStr.assign(1651, '2');
                displayLogMsg = "[2]���Խ��ϴ�.";
                if (displayLogMsg != "")
                {
                    recvStr.append(displayLogMsg.c_str());              // display �޽����� ��� ����ü�� �߰��Ѵ�(append�� ����ü�� �ƴϴ� �ӽÿ�)
                }
            }
            else if (m_testerMsg == 2)
            {
                dataset.id = 44;
                recvStr.assign(6426, '4');
                displayLogMsg = "[44]���Խ��ϴ�.";
                displayWaypoint = "1111.11/2222.22/3333.33";
                if (displayLogMsg != "")
                {
                    recvStr.append(displayLogMsg.c_str());              // display �޽����� ��� ����ü�� �߰��Ѵ�(append�� ����ü�� �ƴϴ� �ӽÿ�)
                }
            }
            else if (m_testerMsg == 3)
            {
                dataset.id = 55;
                recvStr.assign(500, '5');
                displayLogMsg = "[55]���Խ��ϴ�.";
                if (displayLogMsg != "")
                {
                    recvStr.append(displayLogMsg.c_str());              // display �޽����� ��� ����ü�� �߰��Ѵ� (append�� ����ü�� �ƴϴ� �ӽÿ�)
                }
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

                if (displayLogMsg != "")
                {
                    // �α׹��ۿ� �߰�
                    stDataFormat logData = { dataset.curTime, 0, 10001, "", 3 };             // [10001]�� �α�, [10002]�� ��������Ʈ
                    logData.fullSize = sizeof(logData.curTime) + sizeof(logData.fullSize) + sizeof(logData.id) + displayLogMsg.size() + sizeof(logData.padding);
                    logData.data.assign(displayLogMsg);

                    m_buffer_log.push(logData);
                }

                if (displayWaypoint != "")
                {
                    stDataFormat waypointData = { dataset.curTime, 0, 10002, "", 3 };             // [10001]�� �α�, [10002]�� ��������Ʈ
                    waypointData.fullSize = sizeof(waypointData.curTime) + sizeof(waypointData.fullSize) + sizeof(waypointData.id) + displayWaypoint.size() + sizeof(waypointData.padding);
                    waypointData.data.assign(displayWaypoint);

                    m_buffer_log.push(waypointData);
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

        //m_saveThread = 
        std::thread([this] {
            this->fileSaver();
            }).detach();
            //m_saveThread.detach();
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
    std::queue<stDataFormat> m_buffer_log;
    std::queue<stDataFormat>* mp_currentBuffer;                 // ������� ��ü�ϰ� ���Ͽ��� ä�����´� (������)
    std::queue<stDataFormat>* mp_processingBuffer;         // fileLoad ���
    std::string m_scenarioContent;                                  // �ó����� ������, �ð�������
    long long m_startTime;
    long long m_endTime;
    long long m_totalPlayTime;                       // ������ header (�� �����ð� ms����, ���� �Ǿտ� ����)
    long long m_curPlayTime;                              // ����ð� (ms)
    std::atomic<int> m_endFlag;                        // ���� �÷���,, [0]����, [1]����, [2]�Ͻ�����
    std::mutex m_mutex;                             // ���� ������ ���� ���ý�
    std::condition_variable m_cv;
    std::string m_filePath;                         // ��� ���ϰ��
    std::string m_filePath_log;                         // ��� ���
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

    // �ó����� �̸��� �ݿ��Ѵ�
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
        if (!m_buffer_log.empty())
        {
            std::queue<stDataFormat> temp;
            m_buffer_log.swap(temp);
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
            m_curPlayTime += (32 * m_speed);
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

            //m_timerThread = 
            std::thread([this] {
                this->startReplayTimer();
                }).detach();
                //m_timerThread.detach();
                //m_useBufThread = 
                std::thread([this] {
                    this->useBuffer();
                    }).detach();
                    //m_useBufThread.detach();
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
                convertMsToTime(m_curHeader.endTime - m_curHeader.startTime);           // ������

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
            //m_loadThread = 
            std::thread([this] {
                this->fillBuffer();
                }).detach();
                //m_loadThread.detach();
        }


        if (mp_processingBuffer->empty() && mp_currentBuffer->empty())
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
            receiver->endSave();            // �Ʒ� ����� ���νó����� �̸��� �ݿ��ؾ��ҵ�...
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
