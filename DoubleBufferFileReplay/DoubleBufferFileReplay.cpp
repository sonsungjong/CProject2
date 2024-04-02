#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

struct stDataFormat
{
    long long time;                             // 8
    int id;                             // 4
    int bodySize;                   // 4
    char bodyData[4080];        // 4080
};          // 4096

class DoubleBufferFileLoadAndReplay
{
public:
    int maxBufferSize = 819200;             // 200��
    stDataFormat buffer1[200];
    stDataFormat buffer2[200];
    stDataFormat* pReplayBuffer;            // ������� ��ü�ϰ� ���Ͽ��� ä�����´�
    stDataFormat* pFileLoadBuffer;          // �̸� ä�����´�
    time_t playTime;                       // ������ header (�� �����ð� ms����, ���� �Ǿտ� ����)
    time_t timerTime = 0;               // Ÿ�̸��� ����ð�

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

    }
};

class DoubleBufferRecvAndSaveFile
{
public:
    int maxBufferSize = 819200;             // 200��
    stDataFormat buffer1[200];
    stDataFormat buffer2[200];
    stDataFormat* pRecvBuffer;            // 800ms���� ä���ִ´�, ���� ä������ ��ü�Ѵ�
    stDataFormat* pFileLoadBuffer;          // ���� ä�������� ���Ͽ� ����Ѵ�
    time_t playTime;                       // ���Ͽ� ����� �ѽð�, �������� �� �տ� ����Ѵ�, ms ����
    time_t timerTime = 0;               // Ÿ�̸��� ����ð�


    DoubleBufferRecvAndSaveFile() {
        // ����ð��� ���Ͽ� startTime ���
        // 
        // 800ms ���� recvTimer ������
        // 2000ms ���� printTimer ������
    }

    ~DoubleBufferRecvAndSaveFile() {

    }

    void endSave() {
        // recv Ÿ�̸Ӹ� ���߰� pRecvBuffer�� ���Ͽ� ����Ѵ���
        // end_time�� ���Ͽ� ����Ѵ�
        
    }

    void printTimer() {
        // ������ �����忡�� 2000ms���� pRecvBuffer�� ���� �ð��� ������ printf �ϰ�
        // 80%�̻� ä�����ٸ� swap�Ѵ�

    }

    void recvTimer() {
        // ������ �����忡�� 800ms ���� ���۸� ä���

    }

    void fileSaver() {
        // �������� swap�� pFileLoadBuffer���۴� ������ �����带 ���� temp�� �ű� �� ���� temp�� ���Ͽ� ����Ѵ�
        stDataFormat* temp = new stDataFormat[200];



        delete[] temp;
    }

    void swapBuffer() {
        // 80%�̻� ä���� pRecvBuffer�� temp�� ���� ��ü�Ѵ�
        stDataFormat* temp = new stDataFormat[200];


        delete[] temp;

    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;


    while (true) {
        std::string input = "";
        printf("0.���α׷� ����, 1.Recv����, 2.Replay����\n");
        std::getline(std::cin, input);

    }

    delete receiver;
}
