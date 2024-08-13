#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fcntl.h>
#include <fstream> 
#include <unistd.h>


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
        std::cout<<buffer<<std::endl;
    }
    wait(NULL);
}