#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(void)
{
	// char *argv[] = {"/usr/local/bin/python3", "printEnv.py", NULL};
	// char *envp[] = {"SCRIPT_NAME=\"/cgi-bin/script.py\"", NULL};
	// char *envp2[] = {"SCRIPT_NAME=\"/cgi-bin/script.py\"", NULL};
	// int pid = fork();
	// if (pid == 0)
	// {
	// 	execve("/usr/local/bin/python3", argv, envp);
	// 	exit(0);
	// }
	// else
	// {
	// 	int pid2 = fork();
	// 	if (pid2 == 0)
	// 	{
	// 		execve("/usr/local/bin/python3", argv, envp2);
	// 		exit(0);
	// 	}
	// }
	std::string url = "/cgi-bin/script.php";
	std::string location = "/cgi-bin";
	const std::string	availCgiExtensions[2] = {".py", ".php"};
	std::string			cgiExtension;
	int					cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiExtension = availCgiExtensions[i];
		cgiFilePos = url.find(cgiExtension);
		if (cgiFilePos != std::string::npos)
			break ;
	}
	if (cgiFilePos == std::string::npos)
	{
		std::cout << "병신" << std::endl;
		return (1);
	}
	std::string scriptName = url.substr(0, cgiFilePos + cgiExtension.size());
	std::cout << scriptName << '\n';
	size_t parentDirReservedPos = url.find("..");
	if (parentDirReservedPos != std::string::npos)
	{
		std::cout << "병신" << std::endl;
		return (2);
	}
	size_t cgiLocPos = url.find(location);
	if (cgiLocPos == std::string::npos || cgiLocPos != 0)
	{
		std::cout << "개병신" << std::endl;
		return (3);
	}
	
	return (0);
}