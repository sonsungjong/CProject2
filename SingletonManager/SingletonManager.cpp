#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <queue>
#include <condition_variable>

class SingletonManager
{
public:
    static SingletonManager& getInstance() {
        static SingletonManager instance;
        return instance;
    }

    void sendData(const std::string& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sharedData = data;
    }

    std::string receiveData() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sharedData;
    }

private:
    SingletonManager() {}
    std::string m_sharedData;
    std::mutex m_mutex;
};

class ClientA 
{
public:
    ClientA(const std::string& initialData, int initTime) : m_data(initialData), m_time(initTime) {}

    void sendToManager() {
        while (true) {
            SingletonManager::getInstance().sendData(m_data);
            std::this_thread::sleep_for(std::chrono::microseconds(m_time));
        }
    }

private:
    std::string m_data = "";
    int m_time = 1000;
};

class ClientB
{
public:
    ClientB(const std::string& object) : m_objectName(object){}
    void recvData() {
        while (true) {
            std::string data = SingletonManager::getInstance().receiveData();

            if (data == "A1") {
                functionOne();
            }
            else if (data == "A2") {
                functionTwo();
            }
            else {
                defaultFunction();
            }
        }
    }

    void functionOne() {
        std::this_thread::sleep_for(std::chrono::microseconds(4000));
        m_data = "A1";
    }

    void functionTwo() {
        std::this_thread::sleep_for(std::chrono::microseconds(3000));
        m_data = "A2";
    }

    void defaultFunction() {
        m_data = "default";
    }

    void printData() {
        while (true) {
            std::lock_guard<std::mutex> lock(m_mutex);
            printf("%s - %s\n", m_objectName.c_str(), m_data.c_str());
            std::this_thread::sleep_for(std::chrono::microseconds(2500));
        }
    }

private:
    std::string m_data = "";
    std::string m_objectName = "";
    std::mutex m_mutex;
};

int main()
{
    ClientA clientA1("A1", 500);
    ClientA clientA2("A2", 600);
    ClientB clientB1("B1");
    ClientB clientB2("B2");

    std::thread threadA1(&ClientA::sendToManager, &clientA1);
    std::thread threadA2(&ClientA::sendToManager, &clientA2);
    std::thread threadB1Recv(&ClientB::recvData, &clientB1);
    std::thread threadB2Recv(&ClientB::recvData, &clientB2);
    std::thread threadB1Print(&ClientB::printData, &clientB1);
    std::thread threadB2Print(&ClientB::printData, &clientB2);

    threadA1.join();
    threadA2.join();
    threadB1Recv.join();
    threadB2Recv.join();
    threadB1Print.join();
    threadB2Print.join();

    return 0;
}
