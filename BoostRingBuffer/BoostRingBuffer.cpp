#include <iostream>
#include <boost/circular_buffer.hpp>

struct STPacketRS485
{
    int header_sync;
    unsigned short header_bodysize;                // id + msg 크기
    unsigned short body_id;
    char* body_msg;
    unsigned short footer_chk;                      // 정상메시지 체크용
};

struct STPacketTCP
{
    unsigned short header_sender;
    unsigned short header_receiver;
    int header_bodysize;                // id + msg 크기
    int body_id;
    char* body_msg;
};

void sendTestTCP();
void recvTCP();
void recvSerial();
void sendTestSerial();

int main()
{
    boost::circular_buffer<int> nCB(3);

    for (int i = 0; i < 10; i++) {
        nCB.push_back(i + 1);
        printf("%d\n", nCB.back());
    }

    int buf_size = 1 * 1024 * 1024;                         // 1MB = 1 (MB) * 1024 (KB) * 1024 (Byte)
    boost::circular_buffer<char> recvCB(buf_size);

    // 송신테스트는 STPacketTCP를 기반으로 char* 를 리턴한다

    // 수신받은 쪽에서 링버퍼로 받고 (헤더의 바디사이즈 먼저 확인하고 남은 크기 받을 수 있게)

    // 수신받은 내용을 출력한다 (출력은 세이프큐에 push하는 것을 대체하여 테스트)
    


    return 0;
}



void recvTCP()
{

}

void sendTestTCP()
{

}

// 시리얼송신 테스트는 STPacketRS485 를 기반으로 char* 리턴
// 수신받은 쪽에서 불안정한 시리얼통신을 받는 것처럼 수신링버퍼에 마구 쌓는다
// 체크링버퍼에서 쌓인만큼 가져와서 헤더와 푸터를 검사한다
// 정상적인 메시지는 세이프큐에 담는다 (size, id, msg)
// 세이프큐는 wait_pop으로 출력한다

void recvSerial()
{

}

void sendTestSerial()
{

}