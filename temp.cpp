#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fcntl.h>
#include <fstream> 
#include <unistd.h>

enum    NUM
{
    ONE = 1, 
    TWO,
    THREE,
    FOUR
};

class Test
{
    private:
        int *a;
    public:
        Test() {
            a = new int[2];
        }
        ~Test()
        {
            delete[] a;
        }
};

class Client
{
    private:
        std::string msg;
    public:
        Client()
        {
            msg = "Hi everyone.\n";
        }
        const char* getMsg()
        {
            int index = 5;

            return (msg.c_str() + index);
        }
};


int main()
{
    std::istringstream test("ONE   TWO  THREE  hi  THREE");
    std::string         str;
    NUM num;
    std::map<std::string, std::string> m;
    m["ONE"] = "ONE";
    m["TWO"] = "TWO";
    m["THREE"] = "THREE";
    while (std::getline(test, str, ' '))
    {
        if (m[str] == "\0")
            std::cout<<"here";
        std::cout<<str<<": "<<m[str]<<std::endl;
    }
    std::cout<<"\n\n======test2=======\n";
    std::istringstream  test2("hi   ");
    while (std::getline(test2, str, ':'))
    {
        std::cout<<str<<"\n";
    }
    std::cout<<"\n\n=======test3=======\n";
    std::string test3 = "HI,";
    test3.erase(2);
    std::cout<<test3<<std::endl;
    test3.clear();
    std::cout<<test3<<"here"<<std::endl;
    std::cout<<"\n\n=======test4=======\n";
    std::string test4 = "HI HellPo World";
    size_t  pos = test4.find('P');
    std::cout<<"find: P"<<std::endl;
    std::cout<<test4.substr(0, pos)<<std::endl;
    std::cout<<test4.substr(pos)<<std::endl;
    while (test>>str)
    {
        std::cout<<str<<std::endl;
    }
    std::cout<<"\n\n=======test5=======\n";
    std::string test5 = "Hi Hello\r";
    size_t  test5_f = test5.find("\r\n");
    if (test5_f == std::string::npos)
        std::cout<<"here\n";
    std::cout<<test5_f<<std::endl;
    std::cout<<"\n\n=======test6=======\n";
    std::vector<int>    v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.pop_back();
    std::cout<<v.front()<<"\n";
    std::cout<<"\n\n=======test7=======\n";
    int temp;
    while ((temp = 3) == 3)
    {
        std::cout<<"here\n";
        break ;
    }
    std::cout<<"\n\n=========test8=========\n";
    temp = 10;
    std::string str1;
    str1 = std::to_string(temp);
    std::cout<<str1<<" "<<str1.size()<<std::endl;
    std::cout<<"\n\n=========test9=========\n";
    std::string str9 = "hI, My naMe is HwanG inGyUU. s";
    std::cout<<str9<<std::endl;
    for (std::string::iterator it = str9.begin(); it != str9.end(); it++)
        *it = std::tolower(*it);
    std::cout<<str9<<std::endl;

    std::cout<<"\n\n=========test9=========\n";
    Test    *TTT = new Test();
    delete TTT;

    std::cout<<"\n\n=========test10=========\n";
    std::string test10 = "\"Good,,,,\", \"Hi\", good, ";
    std::string ans;
    int comma = true;

    for (std::string::iterator it = test10.begin(); it != test10.end(); it++)
    {
        if (*it == '"')
            comma = !comma;
        else if (comma && *it == ',')
            ans += '\0';
        else
            ans += *it;
    }
    if (!comma)
        std::cout<<"ERROR\n";
    for (std::string::iterator it = ans.begin(); it != ans.end(); it++)
    {
        if (*it != '\0')
            std::cout<<*it;
        else
            std::cout<<"EOF"<<std::endl;
    }
    std::cout<<"\ncomma parsing"<<std::endl;
    std::istringstream  strStream(ans);
    while (std::getline(strStream, str, '\0'))
    {
        std::cout<<str<<std::endl;
    }
    std::cout<<"\n\n=========test11=========\n";
    std::string file_path = "goodtester";
    std::ifstream goodTester(file_path);
    if (!goodTester)
    {
        std::cerr << "Error: File '" << file_path << "' could not be opened." << std::endl;
        return 1; // 프로그램을 비정상적으로 종료합니다.
    }
    std::string line;
    while (std::getline(goodTester, line)) {
        std::cout << line << std::endl;
    }
    goodTester.close();
    std::cout<<"\n\n=========test12=========\n";
    Client  test12;
    const char* strstr = test12.getMsg();
    std::cout<<strstr;
    std::cout<<"\n\n=========test13=========\n";
    std::string test13Str = "       Hi        ";

    pos = test13Str.find_first_not_of(' ');
    test13Str.erase(0, pos);
    pos = test13Str.find_last_not_of(' ');
    test13Str.erase(pos + 1);
    std::cout<<"test13Str: "<<test13Str;
    std::cout<<"\n\n=========test14=========\n";
    int test14Fd = open("./favico.ico", O_RDONLY);
    char    test14Buffer[4096];
    std::cout<<test14Fd<<std::endl;
    read(test14Fd, test14Buffer, sizeof(test14Buffer));
    std::cout<<test14Buffer<<std::endl;
    std::cout<<"\n\n=========test15=========\n";
    return 0;
}

