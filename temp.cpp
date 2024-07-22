#include <iostream>
#include <map>
#include <sstream>
#include <vector>

enum    NUM
{
    ONE = 1, 
    TWO,
    THREE,
    FOUR
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
    // while (std::getline(test, str, ' '))
    // {
    //     if (m[str] == "\0")
    //         std::cout<<"here";
    //     std::cout<<str<<": "<<m[str]<<std::endl;
    // }
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
    std::string test4 = "HI Hello World";
    std::cout<<test4.substr(0, 4)<<std::endl;
    std::cout<<test4.substr(4)<<std::endl;
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
        return (0);
    }
    return 0;
}



trailer: A, B, C, D, E

afdasfadsklj
asfjldfjsa
asdj;

A: 
B: 
C: adjkshfkjs
