// 모스키토 송신 코드

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

cd C:/paho.mqtt.cpp/build
rm -rf CMakeCache.txt CMakeFiles/

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
#include <cJSON.h>
#include <mqtt/async_client.h>

const char* ADDRESS = "tcp://localhost:1883";
const char* CLIENTID = "A";
const char* TOPIC = "test_topic";
const int QOS = 1;
const int TIMEOUT = 10000L;

int main(int argc, char* argv[])
{
    cJSON* root;
    cJSON* data;
    char* payload;

    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sensor", cJSON_CreateString("gps"));
    cJSON_AddItemToObject(root, "time", cJSON_CreateString("1351824120"));

    data = cJSON_CreateArray();
    cJSON_AddItemToArray(data, cJSON_CreateNumber(48.75608));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(2.302038));
    cJSON_AddItemToObject(root, "data", data);

    payload = cJSON_Print(root);
    cJSON_Delete(root);

    mqtt::async_client cli(ADDRESS, CLIENTID);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        printf("Connecting...\n");
        cli.connect(connOpts)->wait();
        printf("Connected\n");

        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, payload);
        pubmsg->set_qos(QOS);
        printf("Publishing message...\n");
        cli.publish(pubmsg)->wait_for(TIMEOUT);
        printf("Message published\n");

        printf("Disconnecting...\n");
        cli.disconnect()->wait();
        printf("Disconnected\n");
    }
    catch (const mqtt::exception& exc) {
        fprintf(stderr, "Error: %s\n", exc.what());
        exit(EXIT_FAILURE);
    }

    cJSON_free(payload);

    return EXIT_SUCCESS;
}