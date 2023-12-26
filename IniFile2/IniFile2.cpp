// ini 파일을 읽고 쓰는 예제
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class IniFile
{
public:
    IniFile() : m_username(""), m_password(""), m_db_path("")
    {
        m_ini_path = ".\\Config.ini";
    }

    bool readINI()
    {
        std::ifstream file(m_ini_path);
        std::string line = "";
        std::string section = "";

        if (file.is_open() == false)
        {
            std::cerr << "Unable to open file" << std::endl;
            return false;
        }

        while (std::getline(file, line))
        {
            std::istringstream is_line(line);
            std::string key = "";

            if (line.empty() == false && line[0] == '[') {
                // Read section name
                section = line.substr(1, line.find(']') - 1);
            }
            else if (std::getline(is_line, key, '=')) {
                std::string value = "";
                if (std::getline(is_line, value)) {
                    // key-value pair based on section
                    if (section == "Account") {
                        if (key == "UserId") {
                            m_username = value;
                        }
                        else if (key == "PassWd") {
                            m_password = value;
                        }
                    }
                    else if (section == "DB") {
                        if (key == "Path") {
                            m_db_path = value;
                        }
                    }
                    else {
                        /* no actions */
                    }
                }
            }
            else {
                /* no actions */
            }
        }
        file.close();
        return true;
    }

    bool writeINI()
    {
        std::ofstream file(m_ini_path);
        if (!file.is_open()) {
            std::cerr << "Unable to open file for writing" << std::endl;
            return false;
        }

        file << "[Account]" << std::endl;
        file << "UserId=" << m_username << std::endl;
        file << "PassWd=" << m_password << std::endl;
        file << std::endl;
        file << "[DB]" << std::endl;
        file << "Path=" << m_db_path << std::endl;

        file.close();
        return true;
    }

    void setUsername(const std::string& username) 
    {
        m_username = username;
        writeINI();
    }

    void setPassword(const std::string& password)
    {
        m_password = password;
        writeINI();
    }

    void setDBPath(const std::string& db_path)
    {
        m_db_path = db_path;
        writeINI();
    }

    std::string getUsername()
    {
        return m_username;
    }

    std::string getPassword()
    {
        return m_password;
    }

    std::string getDBPath() 
    {
        return m_db_path;
    }

private:
    std::string m_ini_path;
    std::string m_username;
    std::string m_password;
    std::string m_db_path;
};

int main()
{
    IniFile* ini_file = new IniFile;
    ini_file->readINI();

    std::cout << ini_file->getUsername() << std::endl;
    std::cout << ini_file->getPassword() << std::endl;
    std::cout << ini_file->getDBPath() << std::endl;

    // 쓰기
    ini_file->setUsername("admin");
    ini_file->setPassword("1234");
    ini_file->setDBPath("C:\\UITCC\\Data\\mes.mdb");

    // 변경 후
    std::cout << ini_file->getUsername() << std::endl;
    std::cout << ini_file->getPassword() << std::endl;
    std::cout << ini_file->getDBPath() << std::endl;

    delete ini_file;
}
