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

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "mqtt/async_client.h"

// "tcp://172.16.1.157:8888"
const char* BROKER_IP_PORT = "tcp://localhost:1883";               // 브로커의 IP와 포트
const char* CLIENT_ID = "server_id";                                     // 클라이언트 ID
const char* TOPIC = "ai";                           // 토픽 코드
const int QOS = 2;                                          // 품질 (0~2단계)
char* g_recv_msg = NULL;

// Subscribe시 수신 이벤트로 설정하는 클래스 (오버라이딩)
class MyEventCallback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        const char* topic_name = msg->get_topic().c_str();
        const char* payload = msg->get_payload_str().c_str();
        printf("%s에서 받은 구독메시지 : %s\n", topic_name, payload);

        if (strcmp(TOPIC, topic_name) == 0) {
            int len = strlen(payload);
            if (len >= 1000000) {
                len = 1000000;
            }
            memcpy(g_recv_msg, payload, len);               // 받은 메시지 저장
            g_recv_msg[len] = '\0';
        }
        else if (strcmp(topic_name, "topic2") == 0) {
            printf("topic2 에 게시된 메시지는 별도로 처리합니다.\n");
        }
        else if (strcmp(topic_name, "topic3") == 0) {
            printf("topic3 에 게시된 메시지는 여기서 처리합니다.\n");
        }
        else {
            printf("그 외의 토픽에 게시된 메시지는 여기로 처리합니다.\n");
        }
    }
};

int main() {
    // 토픽명 my_topic
    g_recv_msg = new char[1000000];
    char flag[1024] = { 0, };
    char publish_msg[1024] = { 0, };
    
    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);           // [비동기 기반] 브로커의 IP와 PORT 및 클라이언트ID 로 생성
    MyEventCallback cb;                 // Subscribe 이벤트 객체
    client.set_callback(cb);             // Subscribe 이벤트 설정
    
    mqtt::connect_options conn;                    // 브로커에 연결을 위한 객체
    conn.set_keep_alive_interval(20);             // 연결 확인용 Ping을 20초 주기로 설정
    conn.set_clean_session(false);                   // true : 새로운 세선에 연결, false : 기존 세션에 연결해서 이어받음 (동일한 clientID 일 경우)

    try {
        client.connect(conn)->wait();                   // 클라이언트 객체를 브로커에 연결시킨다
        printf("브로커에 연결됨\n");

        // 구독
        client.subscribe(TOPIC, QOS)->wait();               // 해당 토픽(my_topic)을 구독한다
        client.subscribe("topic2", QOS)->wait();             // 해당 토픽(topic2)을 구독한다
        printf("해당 TOPIC을 구독했습니다: %s \n", TOPIC);
        printf("해당 TOPIC을 구독했습니다: %s \n", "topic2");

        while (true) {
            memset(flag, 0, 1024);
            printf("%s에 게시할 메시지(게시하려면 0 입력) >>\n", TOPIC);
            rewind(stdin);
            scanf_s("%[^\n]s", flag, 1024);

            if (strcmp(flag, "-1") == 0) {
                client.disconnect()->wait();    // 브로커와 연결을 해제
                printf("브로커와의 연결을 해제했습니다.");
                delete[] g_recv_msg;
                return 0;
            }

            if (strcmp(flag, "0") != 0) {
                // 0이 아니면 메시지 변경
                memset(publish_msg, 0, 1024);
                memcpy(publish_msg, flag, 1024);
            }
            else {
                // 해당 토픽(my_topic)에 메시지를 게시한다 (PUBLISHER)
                mqtt::message_ptr send_msg = mqtt::make_message(TOPIC, publish_msg);
                send_msg->set_qos(QOS);
                client.publish(send_msg)->wait_for(std::chrono::seconds(5));
                printf("다음 메시지가 브로커에 게시됨: %s\n", publish_msg);
            }
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        delete[] g_recv_msg;
        return EXIT_FAILURE;
    }

    delete[] g_recv_msg;
    return EXIT_SUCCESS;
}
