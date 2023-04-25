// ��Ű�� �۽� �ڵ�

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

cd C:/paho.mqtt.cpp/build
rm -rf CMakeCache.txt CMakeFiles/

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