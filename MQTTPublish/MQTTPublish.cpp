/*
<Jsoncpp 라이브러리 빌드 방법(Release x64), static_lib>
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고
git clone https://github.com/open-source-parsers/jsoncpp.git
를 입력하여 프로젝트 설치

cd jsoncpp
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release ..
sln 프로젝트를 실행하고 jsoncpp_static 을 빌드한다

만약, 64비트로 빌드하고 32비트도 빌드하고자 하면 이전 빌드 파일과 CMake 캐시를 제거해야한다.
cd C:/jsoncpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

<포함 디렉토리>
C:\jsoncpp\include

<라이브러리 디렉토리 : 릴리즈 >
C:\jsoncpp\build\lib\Release

<Linker -> Input>
jsoncpp.lib
*/


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

// 게시자
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <json/json.h>
#include <mqtt/async_client.h>

#pragma comment(lib, "paho-mqtt3a.lib")
#pragma comment(lib, "paho-mqtt3c.lib")
#pragma comment(lib, "paho-mqttpp3-static.lib")
#pragma comment(lib, "jsoncpp.lib")

#define TOPIC1              "sensor"
#define TOPIC2              "linker"

// 게시 : 일련번호, 시간, 온도, 습도, 진동가속도, 위도, 경도

struct Sensor
{
    char serial[20];
    char time[20];
    float temp;
    int humi;
    float vib;
    float lat;
    float lon;
};

struct Linker
{
    char time[20];
    char bit1[20];
    int bit2;
    float bit3;
};

// 단일성
void PublishSensor()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "sensor_id";
    const int QOS = 2;

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);
    mqtt::connect_options conn;
    conn.set_keep_alive_interval(20);             // 연결 확인용 Ping을 20초 주기로 설정
    conn.set_clean_session(false);                   // true : 새로운 세선에 연결, false : 기존 세션에 연결해서 이어받음 (동일한 clientID 일 경우)

    client.connect(conn)->wait();
    printf("[게시자]가 브로커에 연결되었습니다.\n");

    const int count_data = 10000;
    Sensor* sensor = new Sensor[count_data];
    Sensor* p = sensor;
    for (int i = 0; i < count_data; i++, p++) {
        sprintf_s(p->serial, 20, "SN123456");
        sprintf_s(p->time, 20, "2023-08-21 12:03:44");
        p->temp = 25.3f;
        p->humi = 60;
        p->vib = 9.8f;
        p->lat = 37.7f;
        p->lon = -122.4f;
    }

    p = sensor;
    Json::Value json;
    for (int i = 0; i < count_data; i++, p++)
    {
        json["serial"].append(p->serial);
        json["time"].append(p->time);
        json["temp"].append(p->temp);
        json["humi"].append(p->humi);
        json["vib"].append(p->vib);
        json["lat"].append(p->lat);
        json["lon"].append(p->lon);
    }

    Json::StreamWriterBuilder writer;
    writer.settings_["indentation"] = "";           // 들여쓰기 없음
    std::string payload = Json::writeString(writer, json);


    mqtt::message_ptr publish_msg = mqtt::make_message(TOPIC1, payload);
    publish_msg->set_qos(QOS);

    client.publish(publish_msg)->wait();                // 게시
    printf("메시지를 게시했습니다. 토픽: %s\n", TOPIC1);

    client.disconnect()->wait();
}

// 실시간성
void PublishLinker()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "linker_id";
    const int QOS = 2;

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);
    mqtt::connect_options conn;
    conn.set_keep_alive_interval(20);             // 연결 확인용 Ping을 20초 주기로 설정
    conn.set_clean_session(false);                   // true : 새로운 세선에 연결, false : 기존 세션에 연결해서 이어받음 (동일한 clientID 일 경우)

    client.connect(conn)->wait();
    printf("[게시자]가 브로커에 연결되었습니다.\n");

    while (true) {
        // 실시간 단일 메시지 생성
        Linker linker = { "SN1111", "2023-08-21 12:03:44", 60, -9.8f };

        Json::Value json;
        json["time"] = linker.time;
        json["bit1"] = linker.bit1;
        json["bit2"] = linker.bit2;
        json["bit3"] = linker.bit3;

        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, json);


        mqtt::message_ptr publish_msg = mqtt::make_message(TOPIC2, payload);
        publish_msg->set_qos(QOS);

        client.publish(publish_msg)->wait();                // 게시
        printf("메시지를 게시했습니다. 토픽: %s\n", TOPIC2);

        std::this_thread::sleep_for(std::chrono::seconds(3));           // 3초 대기
    }

    client.disconnect()->wait();
}

// 게시자
int main()
{
    int num = 0;
    printf("센서: 0, 연동장치: 1 >>>");
    scanf_s("%d", &num);

    if (num == 0) {
        PublishSensor();
    }
    else {
        PublishLinker();
    }

    return 0;
}
