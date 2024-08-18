#include <iostream>
#include <unistd.h>
#include <cstdlib>

void	func()
{
	system("leaks a.out");
}

int main(void)
{
	// atexit(func);
	for (int i=0; i<3; i++)
	{
		int pid = fork();
		if (pid == -1)
		{
			std::cout << "sibal\n";
			exit(2);
		}
		if (pid == 0)
		{
			char *argv[] = {"/bin/sleep", "10", 0};
			execve("/bin/sleep", argv, 0);
			exit(0);
		}
		else
		{
			std::cout << "I'm Father \n";
		}
	}
	for (int i=0; i<3; i++)
		waitpid(-1, 0, WNOHANG);
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
	// {
	// 	std::cout << "병신" << std::endl;
	// 	return (1);
	// }
	// std::string scriptName = url.substr(0, cgiFilePos + cgiExtension.size());
	// std::cout << scriptName << '\n';
	// size_t parentDirReservedPos = url.find("..");
	// if (parentDirReservedPos != std::string::npos)
	// {
	// 	std::cout << "병신" << std::endl;
	// 	return (2);
	// }
	// size_t cgiLocPos = url.find(location);
	// if (cgiLocPos == std::string::npos || cgiLocPos != 0)
	// {
	// 	std::cout << "개병신" << std::endl;
	// 	return (3);
	// }
	
	return (0);
}