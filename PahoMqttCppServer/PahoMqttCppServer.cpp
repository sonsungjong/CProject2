// ��Ű�� ���� �ڵ�

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

PATH=C:\paho.mqtt.c\build\src\Release;
%PATH%;

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

// < cJSON ���̺귯�� >
// 1. cJSON�� ��ġ�ϱ� ���ؼ� ���� ��ġ ���α׷��� �ʿ��ϴ� [ cmake, git bash, visual studio ]
// 2. git clone https://github.com/DaveGamble/cJSON.git �� ���� Ŭ���� �����Ѵ�.
// 3. cd cJSON ���� ����.
// 4. mkdir build �� �Է��� ������ �����
// 5. cd build �� ����
// 6. cmake ..
// 7. build ���丮 �ȿ� �ִ� cJSON.sln ������ �����Ѵ�.
// 8. cjson ������Ʈ�� �����Ѵ�. (������ ����״� ����)
// 9. cjson.h ��θ� �߰��ϰ�, lib�� dll�� �߰��Ͽ� ����Ѵ�.


#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <cJSON.h>

const char* ADDRESS = "tcp://localhost:1883";
const char* CLIENTID = "B";
const char* TOPIC = "test_topic";
const int QOS = 1;
const int TIMEOUT = 10000L;

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
    mqtt::async_client cli(ADDRESS, CLIENTID);
    Callback cb;
    cli.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        printf("Connecting...\n");
        cli.connect(connOpts)->wait();
        printf("Connected\n");

        printf("Subscribing to topic...\n");
        cli.subscribe(TOPIC, QOS)->wait();
        printf("Subscribed\n");

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const mqtt::exception& exc) {
        fprintf(stderr, "Error: %s\n", exc.what());
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}