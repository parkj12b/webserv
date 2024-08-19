#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <deque>

void	func()
{
	system("leaks a.out");
}

std::string executeCGI(const std::string& file)
{
	int	pipefd[2];
	std::string	res;
	if (pipe(pipefd) < 0)
	{
		std::cout << "pipe() error\n";
		exit(1);
	}
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
		close(pipefd[1]);
		std::string pythonCmd = "/usr/local/bin/python3";
		std::deque<char *> sibal;
		sibal.push_back(const_cast<char *>("QUERY_STRING=name=hihihiihi&age=28"));
		char *argv[] = {const_cast<char *>(&pythonCmd[0]), const_cast<char *>(&file[0]), NULL};
		char **envp = new char*[sibal.size() + 1];
		for (int i=0; i<sibal.size(); i++)
			envp[i] = sibal[i];
		envp[sibal.size()] = 0;
		execve(&pythonCmd[0], argv, envp);
	}
	return (res);
}

int main(void)
{
	// atexit(func);
	std::string path = "cgi-bin/script.py";
	std::cout << executeCGI(path);
	// std::string url = "/cgi-bin/script.php";
	// std::string location = "/cgi-bin";
	// const std::string	availCgiExtensions[2] = {".py", ".php"};
	// std::string			cgiExtension;
	// int					cgiFilePos;
	// for (int i=0; i<2; i++)
	// {
	// 	cgiExtension = availCgiExtensions[i];
	// 	cgiFilePos = url.find(cgiExtension);
	// 	if (cgiFilePos != std::string::npos)
	// 		break ;
	// }
	// if (cgiFilePos == std::string::npos)
	// 	return (1);
	// std::string scriptName = url.substr(0, cgiFilePos + cgiExtension.size());
	// size_t parentDirReservedPos = url.find("..");
	// if (parentDirReservedPos != std::string::npos)
	// 	return (2);
	// size_t cgiLocPos = url.find(location);
	// if (cgiLocPos == std::string::npos || cgiLocPos != 0)
	// 	return (3);
	
	return (0);
}