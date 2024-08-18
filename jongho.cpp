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
	std::string url = "http://localhost/cgi-bin/script.py";
	const std::string	availCgiExtensions[2] = {".py", ".php"};
	std::string			cgiExtension;
	int					cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiExtension = availCgiExtensions[i];
		std::cout << cgiExtension << std::endl;
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
	
	return (0);
}