// ini 파일을 읽고 쓰는 예제
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class IniFile
{
public:
    IniFile() : m_username(""), m_password("")
    {
        m_ini_path = ".\\Config.ini";
    }

    void readINI()
    {
        std::ifstream file(m_ini_path);
        std::string line;

        if (file.is_open() == false)
        {
            std::cerr << "Unable to open file" << std::endl;
            return;
        }

        while (std::getline(file, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (std::getline(is_line, value))
                {
                    if (key == "username") { 
                        m_username = value; 
                    }
                    else if (key == "password") {
                        m_password = value;
                    }
                    else {
                        /* no actions */
                    }
                }
            }
        }
        file.close();
    }

    std::string getUsername()
    {
        return m_username;
    }

    std::string getPassword()
    {
        return m_password;
    }

private:
    std::string m_ini_path;
    std::string m_username;
    std::string m_password;
};

int main()
{
    IniFile* ini_file = new IniFile;
    ini_file->readINI();

    std::cout << ini_file->getUsername() << std::endl;
    std::cout << ini_file->getPassword() << std::endl;

    delete ini_file;
}
