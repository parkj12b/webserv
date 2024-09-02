#include <unistd.h>
#include <iostream>
using namespace std;

int main()
{
    int fd[2];

    pipe(fd);
    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        char *const argv[] = {(char *)"/usr/local/bin/python3", (char *)"test.py", NULL};
        char *const envp[] = {NULL};
        if (execve(argv[0], argv, envp) < 0)
        {
            cout << "execve error" << endl;
            exit(1);
        }
    }
    else
    {
        close(fd[1]);
        close(fd[0]);
        char buf[1024];
        int len = 0;
        
        do {
            len = read(fd[0], buf, 1024);
            buf[len] = '\0';
            cout << buf << endl;
        } while (len > 0);
        wait(NULL);
        while (1)
        {
            cout << "hehe" << endl;
        }
    }
}