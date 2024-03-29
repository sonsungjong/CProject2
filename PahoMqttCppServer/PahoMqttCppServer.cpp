// 모스키토 수신 코드

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

PATH=C:\paho.mqtt.c\build\src\Release;
%PATH%;

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

// < cJSON 라이브러리 >
// 1. cJSON을 설치하기 위해선 사전 설치 프로그램이 필요하다 [ cmake, git bash, visual studio ]
// 2. git clone https://github.com/DaveGamble/cJSON.git 을 통해 클론을 진행한다.
// 3. cd cJSON 으로 들어간다.
// 4. mkdir build 를 입력해 폴더를 만든다
// 5. cd build 로 들어간다
// 6. cmake ..
// 7. build 디렉토리 안에 있는 cJSON.sln 파일을 실행한다.
// 8. cjson 프로젝트를 빌드한다. (릴리즈 디버그는 선택)
// 9. cjson.h 경로를 추가하고, lib와 dll을 추가하여 사용한다.


#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <cJSON.h>

// 서버 아님,, Subscriber (구독자)
const char* ADDRESS = "tcp://localhost:1883";               // 브로커의 IP와 포트
const char* CLIENTID = "B";                                         // 클라이언트 아이디
const char* TOPIC = "test_topic";                                   // 토픽
const int QOS = 2;                                                  // 단계 ( 0 ~ 2)
const int TIMEOUT = 10000L;

// public virtual mqtt::callback 을 상속받은 클래스는 비동기로 수신메시지를 처리할 수 있다
// 메시지가 도착했을 때 동작하기 위한 이벤트 기반 처리(콜백 함수)
class Callback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        cJSON* root = cJSON_Parse(msg->get_payload_str().c_str());

        if (root) {
            cJSON* sensor = cJSON_GetObjectItem(root, "sensor");
            cJSON* time = cJSON_GetObjectItem(root, "time");
            cJSON* data = cJSON_GetObjectItem(root, "data");

            if (sensor && time && data) {
                printf("sensor: %s\n", sensor->valuestring);
                printf("time: %s\n", time->valuestring);

                cJSON* lat = cJSON_GetArrayItem(data, 0);
                cJSON* lon = cJSON_GetArrayItem(data, 1);

                if (lat && lon) {
                    printf("data: [%f, %f]\n", lat->valuedouble, lon->valuedouble);
                }
            }
            cJSON_Delete(root);
        }
    }
};

int main(int argc, char* argv[]) {
    mqtt::async_client cli(ADDRESS, CLIENTID);                  // [비동기] 브로커의 IP 와 PORT 및 클라이언트의 ID를 통해 객체 생성
    Callback cb;                // public virtual mqtt::callback 을 상속받아 이벤트기반이 된 클래스
    cli.set_callback(cb);           // 이벤트 콜백함수 등록(메시지 수신시 동작을 위한)

    mqtt::connect_options connOpts;             // 브로커 연결을 위한 객체 생성
    connOpts.set_keep_alive_interval(20);           // 연결 객체 설정1
    connOpts.set_clean_session(true);                   // 연결 객체 설정2

    try {
        printf("Connecting...\n");
        cli.connect(connOpts)->wait();                  // 브로커에 연결
        printf("Connected\n");

        printf("Subscribing to topic...\n");
        cli.subscribe(TOPIC, QOS)->wait();              // [비동기] 구독할 주제와 품질단계(0~2)를 브로커에 요청
        printf("Subscribed\n");

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));        // 잠깐 대기?
            printf("메인쓰레드 종료를 방지하기 위한 임시코드\n");
        }
    }
    catch (const mqtt::exception& exc) {
        fprintf(stderr, "Error: %s\n", exc.what());
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}