#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

int main()
{
    pid_t fd = fork();

    if (fd == 0)
    {
        while (1);
    }
    waitpid(-1, NULL, WNOHANG);
}