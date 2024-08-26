#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fcntl.h>
#include <fstream> 
#include <unistd.h>

class Client
{
    private:
        int a;
    public:
        Client()
        {}
        int getA()
        {
            return (a);
        }
};

int main(void)
{
    pid_t   child;
    int fd = open("testCase", O_RDONLY);

    if (fd < 0)
        return (0);
    child = fork();
    if (child == 0)
    {
        char    buffer[10];
        buffer[read(fd, buffer, 10)] = '\0';
        LOG(std::cout<<buffer<<std::endl);
        return (0);
    }
    wait(NULL);
    LOG(std::cout<<"\n\n======test2=======\n");
    std::map<int, Client*>   data;
    data[1] = new Client();
    LOG(std::cout<<data[2]->getA()<<std::endl);
    return (0);
}