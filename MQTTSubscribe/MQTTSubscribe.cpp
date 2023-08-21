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
*/

// 구독자

#define NOMINMAX
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <Windows.h>
// #include <json/json.h>
#include <mqtt/async_client.h>

#pragma comment(lib, "paho-mqtt3a.lib")
#pragma comment(lib, "paho-mqtt3c.lib")
#pragma comment(lib, "paho-mqttpp3-static.lib")
// #pragma comment(lib, "jsoncpp.lib")

#define TOPIC1              "sensor"
#define TOPIC2              "linker"


char flag = 0;
char* g_msg;

// Subscribe시 수신 이벤트로 설정하는 클래스 (오버라이딩)
class MyEventCallback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        const char* topic_name = msg->get_topic().c_str();
        const char* payload = msg->get_payload_str().c_str();
        printf("%s에서 받은 구독메시지 : %s\n", topic_name, payload);
        size_t len = strlen(payload);
        if (len >= 20000000) {
            len = 20000000;
        }

        if (strcmp(TOPIC1, topic_name) == 0) {
            // 토픽1
            flag = 1;
            memset(g_msg, 0, len);
            memcpy(g_msg, payload, len);
            g_msg[len] = '\0';

            // 구독메시지 파일 저장
            FILE* p_file;
            fopen_s(&p_file, "C:\\log\\topic1_sensor.txt", "at");
            if (NULL != p_file) {
                fprintf(p_file, payload);
                fprintf(p_file, "\n");
                fclose(p_file);
            }
        }
        else if (strcmp(topic_name, TOPIC2) == 0) {
            // 토픽2
            flag = 1;
            memset(g_msg, 0, len);
            memcpy(g_msg, payload, len);
            g_msg[len] = '\0';

            // 구독메시지 파일 저장
            FILE* p_file;
            fopen_s(&p_file, "C:\\log\\topic2_linker.txt", "at");
            if (NULL != p_file) {
                fprintf(p_file, payload);
                fprintf(p_file, "\n");
                fclose(p_file);
            }
        }
        else {
            /* error */
            printf("에러 발생\n");
        }
    }
};

// 서버 (구독자)
int main()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "my_id";
    const int QOS = 2;

    g_msg = new char[20000000];

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);       // 비동기로 브로커의 IP와 PORT와 ID로 생성
    MyEventCallback cb;
    client.set_callback(cb);

    mqtt::connect_options conn;             // 브로커 연결을 위한 객체
    conn.set_keep_alive_interval(20);             // 연결 확인용 Ping을 20초 주기로 설정
    conn.set_clean_session(false);                   // true : 새로운 세선에 연결, false : 기존 세션에 연결해서 이어받음 (동일한 clientID 일 경우)

    client.connect(conn)->wait();               // 클라이언트 객체를 브로커에 연결
    printf("[구독자]가 브로커에 연결되었습니다.\n");

    client.subscribe(TOPIC1, QOS)->wait();              // 토픽1(센서) 구독한다
    client.subscribe(TOPIC2, QOS)->wait();              // 토픽2(연동장치) 구독한다

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));           // 1초 대기
        if (flag != 0) {
            int size = MultiByteToWideChar(CP_ACP, 0, g_msg, -1, NULL, 0);
            wchar_t* wstr = new wchar_t[size];
            MultiByteToWideChar(CP_ACP, 0, g_msg, -1, wstr, size);

            wchar_t wsz[50];
            swprintf_s(wsz, 50, L"문자열 길이 : %d", size);

            MessageBox(NULL, wstr, wsz, MB_OK);
            delete[] wstr;
            flag = 0;
        }
    }

    delete[] g_msg;
    return 0;
}