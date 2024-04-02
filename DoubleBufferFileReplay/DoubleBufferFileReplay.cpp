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
    int maxBufferSize = 819200;             // 200배
    stDataFormat buffer1[200];
    stDataFormat buffer2[200];
    stDataFormat* pReplayBuffer;            // 비워지면 교체하고 파일에서 채워놓는다
    stDataFormat* pFileLoadBuffer;          // 미리 채워놓는다
    time_t playTime;                       // 파일의 header (총 재현시간 ms단위, 파일 맨앞에 있음)
    time_t timerTime = 0;               // 타이머의 경과시간

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

    }
};

class DoubleBufferRecvAndSaveFile
{
public:
    int maxBufferSize = 819200;             // 200배
    stDataFormat buffer1[200];
    stDataFormat buffer2[200];
    stDataFormat* pRecvBuffer;            // 800ms마다 채워넣는다, 전부 채워지면 교체한다
    stDataFormat* pFileLoadBuffer;          // 전부 채워졌으니 파일에 기록한다
    time_t playTime;                       // 파일에 기록할 총시간, 마지막에 맨 앞에 기록한다, ms 단위
    time_t timerTime = 0;               // 타이머의 경과시간


    DoubleBufferRecvAndSaveFile() {
        // 현재시간을 파일에 startTime 기록
        // 
        // 800ms 마다 recvTimer 쓰레드
        // 2000ms 마다 printTimer 쓰레드
    }

    ~DoubleBufferRecvAndSaveFile() {

    }

    void endSave() {
        // recv 타이머를 멈추고 pRecvBuffer를 파일에 기록한다음
        // end_time을 파일에 기록한다
        
    }

    void printTimer() {
        // 별도의 쓰레드에서 2000ms마다 pRecvBuffer의 이전 시간의 값들을 printf 하고
        // 80%이상 채워졌다면 swap한다

    }

    void recvTimer() {
        // 별도의 쓰레드에서 800ms 마다 버퍼를 채운다

    }

    void fileSaver() {
        // 가득차서 swap된 pFileLoadBuffer버퍼는 별도의 쓰레드를 통해 temp에 옮긴 후 비우고 temp를 파일에 기록한다
        stDataFormat* temp = new stDataFormat[200];



        delete[] temp;
    }

    void swapBuffer() {
        // 80%이상 채워진 pRecvBuffer를 temp를 통해 교체한다
        stDataFormat* temp = new stDataFormat[200];


        delete[] temp;

    }
};

int main()
{
    DoubleBufferRecvAndSaveFile* receiver = new DoubleBufferRecvAndSaveFile;


    while (true) {
        std::string input = "";
        printf("0.프로그램 종료, 1.Recv중지, 2.Replay중지\n");
        std::getline(std::cin, input);

    }

    delete receiver;
}
