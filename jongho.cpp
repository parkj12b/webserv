#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <deque>
#include <fcntl.h>

void func()
{
    system("leaks a.out");
}

std::string executeCGI(const std::string &file)
{
    int pipefd[2];
    std::string res;

    if (pipe(pipefd) < 0)
    {
        std::cout << "pipe() error\n";
        exit(1);
    }

    // 파이프를 비차단 모드로 설정
    fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    for (int i = 0; i < 5; i++)
    {
        int pid = fork();
        if (pid == -1)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            std::cout << "fork() error\n";
            exit(2);
        }
        if (pid != 0)
        {
            close(pipefd[1]);
            char buf[65537];
            int size;
            while ((size = read(pipefd[0], buf, 65536)) > 0)
            {
                buf[size] = 0;
                res.append(std::string(buf));
            }
            close(pipefd[0]);
            return (res);
        }
        else
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
			dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[1]);

            std::string pythonCmd = "/usr/local/bin/python3";
            std::deque<char *> sibal;
            sibal.push_back(const_cast<char *>("QUERY_STRING=name=hihihiihi&age=28"));
            char *argv[] = {const_cast<char *>(&pythonCmd[0]), const_cast<char *>(&file[0]), NULL};
            char **envp = new char *[sibal.size() + 1];
            for (int i = 0; i < sibal.size(); i++)
                envp[i] = sibal[i];
            envp[sibal.size()] = 0;

            execve(&pythonCmd[0], argv, envp);
            exit(0); // execve가 실패한 경우 자식 프로세스가 종료되도록 함
        }
    }
    for (int i = 0; i < 5; i++)
        waitpid(-1, 0, WNOHANG);

    return (res);
}

int main(void)
{
    // atexit(func);
    std::string path = "cgi-bin/script.py";
    std::cout << executeCGI(path);

    return (0);
}
