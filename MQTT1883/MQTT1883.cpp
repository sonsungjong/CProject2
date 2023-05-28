/*
<���̺귯�� ���� ���(Release x64)>
Git Bash �� ��ġ�Ѵ�.
CMake�� ��ġ�Ѵ�.
Visual Studio�� ��ġ�Ѵ� (Visual Studio 17 2022, Visual Studio 16 2019 ��)
C����̺꿡 git bash�� ������ �������� �����ϰ�
git clone https://github.com/eclipse/paho.mqtt.c.git
git clone https://github.com/eclipse/paho.mqtt.cpp.git
�� ���� �Է��Ͽ� ������Ʈ�� ��ġ�Ѵ�.

CMake�� ���� sln ������ �����Ѵ�.
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

����, 64��Ʈ�� �����ϰ� 32��Ʈ�� �����ϰ��� �ϸ� ���� ���� ���ϰ� CMake ĳ�ø� �����ؾ��Ѵ�.
cd C:/paho.mqtt.c/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

cd C:/paho.mqtt.cpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 -DPAHO_MQTT_C_LIBRARIES="C:/paho.mqtt.c/build/src/Release" -DPAHO_MQTT_C_INCLUDE_DIRS="C:/paho.mqtt.c/src" ..

<���� ���丮>
C:\paho.mqtt.c\src
C:\paho.mqtt.cpp\src

<���̺귯�� ���丮 : ������ ����>
C:\paho.mqtt.c\build\src\Release
C:\paho.mqtt.cpp\build\src\Release

<Linker �� Input : ������ ����>
paho-mqtt3a.lib
paho-mqtt3c.lib
paho-mqttpp3-static.lib

<dll ����>
Project -> Properties -> Configuration Properties -> Debugging -> Environment ���ٰ�

PATH=C:\paho.mqtt.c\build\src\Release
;%PATH%;

<MQTT���̵�>
https://mosquitto.org/download/ ���� mosquitto ����(���Ŀ) �� ��ġ�Ѵ�.
C:\Program Files\mosquitto �� �ý��� ������ Path �� �����Ѵ�.
�۾������� -> ���� -> mosquitto �� ��Ŭ���ϰ� '����'
cmd ����
mosquitto -p ��Ʈ��ȣ
�� �Է��Ͽ� ���Ŀ�� �����Ѵ�.
C++ �Ǵ� Python�� Ŭ���̾�Ʈ �ڵ带 �����Ͽ� �����Ѵ�.
���Ŀ�� �⺻ ��Ʈ�� 1883
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
