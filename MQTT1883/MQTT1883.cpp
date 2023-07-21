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

const char* BROKER_IP_PORT = "tcp://localhost:1883";               // ���Ŀ�� IP�� ��Ʈ
const char* CLIENT_ID = "my_id";                                     // Ŭ���̾�Ʈ ID
const char* TOPIC = "my_topic";                           // ���� �ڵ�
const int QOS = 2;
char* g_recv_msg = NULL;

// Subscribe�� ���� �̺�Ʈ�� �����ϴ� Ŭ���� (�������̵�)
class MyEventCallback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        const char* recv_msg = msg->get_payload_str().c_str();
        printf("�����Ͽ� ���� �޽��� : %s\n", recv_msg);

        int len = strlen(recv_msg);
        if (len >= 1000000) {
            len = 1000000;
        }
        memcpy(g_recv_msg, recv_msg, len);
        g_recv_msg[len] = '\0';
    }
};

int main() {
    // ���ȸ� my_topic
    g_recv_msg = new char[1000000];
    char flag[1024] = { 0, };
    char publish_msg[1024] = { 0, };

    mqtt::async_client client(BROKER_IP_PORT, CLIENT_ID);           // [�񵿱� ���] ���Ŀ�� IP�� PORT �� Ŭ���̾�ƮID �� ����
    MyEventCallback cb;                 // Subscribe �̺�Ʈ ����
    client.set_callback(cb);             // Subscribe �̺�Ʈ ����
    
    mqtt::connect_options conn;                    // ���Ŀ�� ������ ���� ��ü
    conn.set_keep_alive_interval(20);             // ���� Ȯ�ο� Ping�� 20�� �ֱ�� ����
    conn.set_clean_session(false);                   // true : ���ο� ������ ����, false : ���� ���ǿ� �����ؼ� �̾���� (������ clientID �� ���)

    try {
        client.connect(conn)->wait();                   // Ŭ���̾�Ʈ ��ü�� ���Ŀ�� �����Ų��
        printf("���Ŀ�� �����\n");

        // ����
        client.subscribe(TOPIC, QOS)->wait();               // �ش� ����(my_topic)�� �����Ѵ�
        printf("�ش� TOPIC�� �����߽��ϴ�: %s \n", TOPIC);

        while (true) {
            memset(flag, 0, 1024);
            printf("%s�� �Խ��� �޽���(�Խ��Ϸ��� 0 �Է�) >>\n", TOPIC);
            rewind(stdin);
            scanf_s("%[^\n]s", flag, 1024);

            if (strcmp(flag, "-1") == 0) {
                client.disconnect()->wait();    // ���Ŀ�� ������ ����
                printf("���Ŀ���� ������ �����߽��ϴ�.");
                delete[] g_recv_msg;
                return 0;
            }

            if (strcmp(flag, "0") != 0) {
                // 0�� �ƴϸ� �޽��� ����
                memset(publish_msg, 0, 1024);
                memcpy(publish_msg, flag, 1024);
            }
            else {
                // �ش� ����(my_topic)�� �޽����� �Խ��Ѵ�
                mqtt::message_ptr send_msg = mqtt::make_message(TOPIC, publish_msg);
                send_msg->set_qos(QOS);
                client.publish(send_msg)->wait_for(std::chrono::seconds(5));
                printf("���� �޽����� ���Ŀ�� �Խõ�: %s\n", publish_msg);
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
