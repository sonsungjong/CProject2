/*
<Jsoncpp ���̺귯�� ���� ���(Release x64), static_lib>
Git Bash �� ��ġ�Ѵ�.
CMake�� ��ġ�Ѵ�.
Visual Studio�� ��ġ�Ѵ� (Visual Studio 17 2022, Visual Studio 16 2019 ��)
C����̺꿡 git bash�� ������ �������� �����ϰ�
git clone https://github.com/open-source-parsers/jsoncpp.git
�� �Է��Ͽ� ������Ʈ ��ġ

cd jsoncpp
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release ..
sln ������Ʈ�� �����ϰ� jsoncpp_static �� �����Ѵ�

����, 64��Ʈ�� �����ϰ� 32��Ʈ�� �����ϰ��� �ϸ� ���� ���� ���ϰ� CMake ĳ�ø� �����ؾ��Ѵ�.
cd C:/jsoncpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

<���� ���丮>
C:\jsoncpp\include

<���̺귯�� ���丮 : ������ >
C:\jsoncpp\build\lib\Release

<Linker -> Input>
jsoncpp.lib
*/


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

<MQTT IP����>
C:\Program Files\mosquitto\mosquitto.conf
������ ��� �Ʒ� �α����� �����ؾ� IP�� ������ �����ϴ� (���ϸ� localhost �� ����)

<listener ��Ʈ�Ҵ� �� �ּ�����(#listener �κи� ����)>
# listener port-number [ip address/host name/unix socket path]
listener 1883

<allow_anonymous �ּ� ���� �� false�� true�� ���� (#allow_anonymous false �κи� ����)>
# Defaults to false, unless there are no listeners defined in the configuration
# file, in which case it is set to true, but connections are only allowed from
# the local machine.
allow_anonymous true

<���� �� mosquitto �� ���񽺿��� �ٽ� ���� �Ѵ�>
*/

// �Խ���
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

// �Խ� : �Ϸù�ȣ, �ð�, �µ�, ����, �������ӵ�, ����, �浵

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

// ���ϼ�
void PublishSensor()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "sensor_id";
    const int QOS = 2;

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);
    mqtt::connect_options conn;
    conn.set_keep_alive_interval(20);             // ���� Ȯ�ο� Ping�� 20�� �ֱ�� ����
    conn.set_clean_session(false);                   // true : ���ο� ������ ����, false : ���� ���ǿ� �����ؼ� �̾���� (������ clientID �� ���)

    client.connect(conn)->wait();
    printf("[�Խ���]�� ���Ŀ�� ����Ǿ����ϴ�.\n");

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
    writer.settings_["indentation"] = "";           // �鿩���� ����
    std::string payload = Json::writeString(writer, json);


    mqtt::message_ptr publish_msg = mqtt::make_message(TOPIC1, payload);
    publish_msg->set_qos(QOS);

    client.publish(publish_msg)->wait();                // �Խ�
    printf("�޽����� �Խ��߽��ϴ�. ����: %s\n", TOPIC1);

    client.disconnect()->wait();
}

// �ǽð���
void PublishLinker()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "linker_id";
    const int QOS = 2;

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);
    mqtt::connect_options conn;
    conn.set_keep_alive_interval(20);             // ���� Ȯ�ο� Ping�� 20�� �ֱ�� ����
    conn.set_clean_session(false);                   // true : ���ο� ������ ����, false : ���� ���ǿ� �����ؼ� �̾���� (������ clientID �� ���)

    client.connect(conn)->wait();
    printf("[�Խ���]�� ���Ŀ�� ����Ǿ����ϴ�.\n");

    while (true) {
        // �ǽð� ���� �޽��� ����
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

        client.publish(publish_msg)->wait();                // �Խ�
        printf("�޽����� �Խ��߽��ϴ�. ����: %s\n", TOPIC2);

        std::this_thread::sleep_for(std::chrono::seconds(3));           // 3�� ���
    }

    client.disconnect()->wait();
}

// �Խ���
int main()
{
    int num = 0;
    printf("����: 0, ������ġ: 1 >>>");
    scanf_s("%d", &num);

    if (num == 0) {
        PublishSensor();
    }
    else {
        PublishLinker();
    }

    return 0;
}
