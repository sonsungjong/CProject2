/*
<라이브러리 빌드 방법(Release x64)>
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고
git clone https://github.com/eclipse/paho.mqtt.c.git
git clone https://github.com/eclipse/paho.mqtt.cpp.git
를 각각 입력하여 프로젝트를 설치한다.

CMake를 통해 sln 파일을 생성한다.
mkdir paho.mqtt.c/build
cd paho.mqtt.c/build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --install .

cd paho.mqtt.cpp
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DPAHO_MQTT_C_LIBRARIES="C:/paho.mqtt.c/build/src/Release" -DPAHO_MQTT_C_INCLUDE_DIRS="C:/paho.mqtt.c/src" ..
cmake --build . --config Release
cmake --install .

만약, 64비트로 빌드하고 32비트도 빌드하고자 하면 이전 빌드 파일과 CMake 캐시를 제거해야한다.
cd C:/paho.mqtt.c/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

cd C:/paho.mqtt.cpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 -DPAHO_MQTT_C_LIBRARIES="C:/paho.mqtt.c/build/src/Release" -DPAHO_MQTT_C_INCLUDE_DIRS="C:/paho.mqtt.c/src" ..

<포함 디렉토리>
C:\paho.mqtt.c\src
C:\paho.mqtt.cpp\src

<라이브러리 디렉토리 : 릴리즈 기준>
C:\paho.mqtt.c\build\src\Release
C:\paho.mqtt.cpp\build\src\Release

<Linker 의 Input : 릴리즈 기준>
paho-mqtt3a.lib
paho-mqtt3c.lib
paho-mqttpp3-static.lib

<dll 설정>
Project -> Properties -> Configuration Properties -> Debugging -> Environment 에다가

PATH=C:\paho.mqtt.c\build\src\Release
;%PATH%;

<MQTT가이드>
https://mosquitto.org/download/ 에서 mosquitto 서버(브로커) 를 설치한다.
C:\Program Files\mosquitto 를 시스템 변수의 Path 에 저장한다.
작업관리자 -> 서비스 -> mosquitto 를 우클릭하고 '시작'
cmd 에서
mosquitto -p 포트번호
를 입력하여 브로커를 실행한다.
C++ 또는 Python의 클라이언트 코드를 실행하여 연결한다.
브로커의 기본 포트는 1883

<MQTT IP셋팅>
C:\Program Files\mosquitto\mosquitto.conf
파일을 열어서 아래 두군데를 수정해야 IP로 셋팅이 가능하다 (안하면 localhost 만 가능)

<listener 포트할당 후 주석해제(#listener 부분만 변경)>
# listener port-number [ip address/host name/unix socket path]
listener 1883

<allow_anonymous 주석 해제 후 false를 true로 변경 (#allow_anonymous false 부분만 변경)>
# Defaults to false, unless there are no listeners defined in the configuration
# file, in which case it is set to true, but connections are only allowed from
# the local machine.
allow_anonymous true

<저장 후 mosquitto 를 서비스에서 다시 시작 한다>
*/

#define MQTT_ADDRESS                 "tcp://localhost:1883"
#define MY_CLIENT_ID                "client_server"
#define MY_TOPIC                "server"
#define QOS_LEVEL                   2
#define TIMEOUT                     10000L

#include <iostream>
#include <string>
#include <Windows.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <mqtt/MQTTAsync.h>

#ifdef _DEBUG
#pragma comment(lib, "mqtt//paho-mqtt3aD.lib")
#pragma comment(lib, "mqtt//paho-mqtt3cD.lib")
#else
#pragma comment(lib, "mqtt//paho-mqtt3a.lib")
#pragma comment(lib, "mqtt//paho-mqtt3c.lib")
#endif

void onSubscribe(void* context, MQTTAsync_successData* response);
void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
void disconnectMQTT();
int sendPayloadMQTT(const char* _szTopicName, const char* _szPayload, int _nQOS_level);
int subscribeTopicMQTT(const char* _szTopicName, int nQOS_level);
int connectMQTT(const char* _szAddress, const char* _szID);
int msgarrvd(void* context, char* topicName, int topicLen, MQTTAsync_message* message);
void onConnect(void* context, MQTTAsync_successData* response);
void onConnectFailure(void* context, MQTTAsync_failureData* response);
void connlost(void* context, char* cause);
std::string CharToUTF8(const char* ascii);
std::string UTF8ToChar(const char* utf8);

// 추후 멤버변수 처리
MQTTAsync g_client;                             // MQTT 클라이언트 핸들
std::atomic_int g_connected = 0;               // 연결 상태 플래그 (외부 하드웨어가 변경할 수도 있기 때문에 volatile 처리)
std::mutex g_mtxConn;
std::condition_variable g_cvConn;

// 연결 손실 관련 콜백함수
void connlost(void* context, char* cause)
{
    // 도중에 끊겼으니 재연결을 시도한다
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 0;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;

    int rc = MQTTAsync_connect(client, &conn_opts);
    if (rc != MQTTASYNC_SUCCESS)
    {
        Sleep(2000);
        rc = MQTTAsync_connect(client, &conn_opts);          // 2초후 한번 더 시도
        if (rc != MQTTASYNC_SUCCESS)
        {
            MessageBoxA(NULL, cause, "MQTT 연결이 손실되었습니다. [복구실패]", MB_OK);
        }
    }

    if (rc == MQTTASYNC_SUCCESS)
    {
        MessageBoxA(NULL, cause, "MQTT 연결 손실 후 복구되었습니다.", MB_OK);
    }
}

// 연결 성공 시 호출 콜백함수
void onConnect(void* context, MQTTAsync_successData* response)
{
    {
        std::unique_lock<std::mutex> lock(g_mtxConn);
        g_client = (MQTTAsync)context;
        g_connected = 1;
    }
    g_cvConn.notify_all();                      // 처리가 끝났음을 알림
}

// 연결 실패 시 호출 콜백함수
void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    g_connected = 0;
}

// 수신 콜백함수
int msgarrvd(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
    printf("토픽명:%s, 길이:%d, 내용:%s\n", topicName, message->payloadlen, (char*)message->payload);

    // 지역변수로 백업
    std::string strTopicName = std::string(topicName);
    std::string strPayload = std::string((char*)message->payload);

    // 해제
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);

    // 해제 후 string 으로 한번 더 출력
    std::string strDecodingPayload = UTF8ToChar(strPayload.c_str());
    printf("토픽명:%s, 길이:%d, 내용:%s\n", strTopicName.c_str(), (int)(strDecodingPayload.size()), strDecodingPayload.c_str());

    return 1;
}

int connectMQTT(const char* _szAddress, const char* _szID)
{
    int nResult = 0;

    int rc = MQTTAsync_create(&g_client, _szAddress, _szID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc == MQTTASYNC_SUCCESS)
    {
        rc = MQTTAsync_setCallbacks(g_client, g_client, connlost, msgarrvd, NULL);
        if (rc == MQTTASYNC_SUCCESS)
        {
            MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;

            conn_opts.keepAliveInterval = 20;
            conn_opts.cleansession = 1;
            conn_opts.onSuccess = onConnect;
            conn_opts.onFailure = onConnectFailure;
            conn_opts.context = g_client;

            rc = MQTTAsync_connect(g_client, &conn_opts);
            if (rc == MQTTASYNC_SUCCESS)
            {
                nResult = 1;                                // 연결 성공을 리턴
            }
        }
    }

    return nResult;
}

// 해당 토픽명에 가입시도
int subscribeTopicMQTT(const char* _szTopicName, int nQOS_level)
{
    int nResult = 0;

    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

    opts.onSuccess = onSubscribe;
    opts.onFailure = onSubscribeFailure;
    opts.context = g_client;

    if (g_connected == 0)
    {
        std::unique_lock<std::mutex> lock(g_mtxConn);
        g_cvConn.wait(lock, [] { return g_connected.load(); });                    // g_connected 변경시까지 대기
        if (!g_cvConn.wait_for(lock, std::chrono::seconds(5), [] { return g_connected.load(); }))
        {
            printf("MQTT 연결 대기 초과\n");
            return nResult;                                     // 연결 대기 초과 시 실패 반환
        }
    }

    if (g_connected)
    {
        int rc = MQTTAsync_subscribe(g_client, _szTopicName, nQOS_level, &opts);
        if (rc == MQTTASYNC_SUCCESS)
        {
            nResult = 1;                    // 토픽 가입 성공 플래그
            printf("토픽 가입 성공!\n");
        }
        else {
            printf("토픽 가입 실패: %s, return code: %d\n", _szTopicName, rc);
        }
    }

    return nResult;
}

int sendPayloadMQTT(const char* _szTopicName, const char* _szPayload, int _nQOS_level)
{
    int nResult = 0;
    if (g_connected)
    {
        printf("Payload 보내기 전: %s\n", _szPayload); // 디버깅 출력
        MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
        pubmsg.payload = (void*)_szPayload;
        pubmsg.payloadlen = (int)strlen(_szPayload);
        pubmsg.qos = _nQOS_level;
        pubmsg.retained = 0;
        int rc = MQTTAsync_sendMessage(g_client, _szTopicName, &pubmsg, NULL);

        if (rc == MQTTASYNC_SUCCESS)
        {
            nResult = 1;                // 전송 성공 처리
        }
    }

    return nResult;
}

void disconnectMQTT()
{
    if (g_connected) {
        MQTTAsync_disconnect(g_client, NULL);
        g_connected = 0;
    }
    MQTTAsync_destroy(&g_client);
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
    /* no actions */
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    /* no actions */
}

// char* -> UTF-8 변환
std::string CharToUTF8(const char* str) {
    int wlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if (wlen == 0) return "";

    // UTF-16로 변환
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], wlen);

    // UTF-16 -> UTF-8 변환
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (ulen == 0) return "";

    std::string utf8(ulen, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], ulen, NULL, NULL);

    return utf8;
}

// UTF-8 -> char* 변환
std::string UTF8ToChar(const char* utf8) {
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (wlen == 0) return "";

    // UTF-8 -> UTF-16 변환
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &wstr[0], wlen);

    // UTF-16 -> char* 변환
    int alen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (alen == 0) return "";

    std::string str(alen, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], alen, NULL, NULL);

    return str;
}

void main()
{
    char msg[256]{};             // scanf 전용

    int result = connectMQTT(MQTT_ADDRESS, MY_CLIENT_ID);               // MQTT 연결
    if (result)
    {
        subscribeTopicMQTT(MY_TOPIC, QOS_LEVEL);
        while (1)
        {
            memset(msg, 0, 256);
            printf("메시지를 입력하세요>>");
            rewind(stdin);
            scanf_s("%[^\n]s", msg, 256);

            std::string strUTF8 = CharToUTF8(msg);

            sendPayloadMQTT(MY_TOPIC, strUTF8.c_str(), QOS_LEVEL);              // 메시지 게시

            if (strcmp(msg, "exit") == 0)
            {
                break;
            }
        }

        disconnectMQTT();                       // MQTT 연결 해제
    }

    printf("테스트 프로그램 종료");
}