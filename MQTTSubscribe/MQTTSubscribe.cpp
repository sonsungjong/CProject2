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

// ������

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

// Subscribe�� ���� �̺�Ʈ�� �����ϴ� Ŭ���� (�������̵�)
class MyEventCallback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        const char* topic_name = msg->get_topic().c_str();
        const char* payload = msg->get_payload_str().c_str();
        printf("%s���� ���� �����޽��� : %s\n", topic_name, payload);
        size_t len = strlen(payload);
        if (len >= 20000000) {
            len = 20000000;
        }

        if (strcmp(TOPIC1, topic_name) == 0) {
            // ����1
            flag = 1;
            memset(g_msg, 0, len);
            memcpy(g_msg, payload, len);
            g_msg[len] = '\0';

            // �����޽��� ���� ����
            FILE* p_file;
            fopen_s(&p_file, "C:\\log\\topic1_sensor.txt", "at");
            if (NULL != p_file) {
                fprintf(p_file, payload);
                fprintf(p_file, "\n");
                fclose(p_file);
            }
        }
        else if (strcmp(topic_name, TOPIC2) == 0) {
            // ����2
            flag = 1;
            memset(g_msg, 0, len);
            memcpy(g_msg, payload, len);
            g_msg[len] = '\0';

            // �����޽��� ���� ����
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
            printf("���� �߻�\n");
        }
    }
};

// ���� (������)
int main()
{
    const char* BROKER_IP_PORT = "tcp://localhost:1883";
    const char* CLIENT_ID = "my_id";
    const int QOS = 2;

    g_msg = new char[20000000];

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);       // �񵿱�� ���Ŀ�� IP�� PORT�� ID�� ����
    MyEventCallback cb;
    client.set_callback(cb);

    mqtt::connect_options conn;             // ���Ŀ ������ ���� ��ü
    conn.set_keep_alive_interval(20);             // ���� Ȯ�ο� Ping�� 20�� �ֱ�� ����
    conn.set_clean_session(false);                   // true : ���ο� ������ ����, false : ���� ���ǿ� �����ؼ� �̾���� (������ clientID �� ���)

    client.connect(conn)->wait();               // Ŭ���̾�Ʈ ��ü�� ���Ŀ�� ����
    printf("[������]�� ���Ŀ�� ����Ǿ����ϴ�.\n");

    client.subscribe(TOPIC1, QOS)->wait();              // ����1(����) �����Ѵ�
    client.subscribe(TOPIC2, QOS)->wait();              // ����2(������ġ) �����Ѵ�

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));           // 1�� ���
        if (flag != 0) {
            int size = MultiByteToWideChar(CP_ACP, 0, g_msg, -1, NULL, 0);
            wchar_t* wstr = new wchar_t[size];
            MultiByteToWideChar(CP_ACP, 0, g_msg, -1, wstr, size);

            wchar_t wsz[50];
            swprintf_s(wsz, 50, L"���ڿ� ���� : %d", size);

            MessageBox(NULL, wstr, wsz, MB_OK);
            delete[] wstr;
            flag = 0;
        }
    }

    delete[] g_msg;
    return 0;
}