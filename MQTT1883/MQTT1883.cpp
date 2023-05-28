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

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID("mqtt_cpp_client");
const std::string TOPIC("test/topic");

int main() {
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        std::cout << "Connecting to the MQTT server..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "Connected" << std::endl;

        std::cout << "Publishing a message..." << std::flush;
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, "Hello MQTT from C++!");
        pubmsg->set_qos(1);
        client.publish(pubmsg)->wait_for(std::chrono::seconds(5));
        std::cout << "Published" << std::endl;

        std::cout << "Disconnecting from the MQTT server..." << std::flush;
        client.disconnect()->wait();
        std::cout << "Disconnected" << std::endl;

    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
