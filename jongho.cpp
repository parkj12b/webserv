#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>

void	func()
{
	system("leaks a.out");
}

int main(void)
{
	// atexit(func);
	std::vector<char *> sibal;
	sibal.push_back(const_cast<char *>("SIBAL=noma"));
	sibal.push_back(const_cast<char *>("NOMA=sibal"));
	char **envp = new char*[sibal.size() + 1];
	for (int i=0; i<sibal.size(); i++)
		envp[i] = sibal[i];
	envp[sibal.size()] = 0;
	char *argv[] = {const_cast<char *>("/usr/local/bin/python3"), const_cast<char *>("test.py"), NULL};
	execve("/usr/local/bin/python3", argv, envp);
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
		return (1);
	std::string scriptName = url.substr(0, cgiFilePos + cgiExtension.size());
	size_t parentDirReservedPos = url.find("..");
	if (parentDirReservedPos != std::string::npos)
		return (2);
	size_t cgiLocPos = url.find(location);
	if (cgiLocPos == std::string::npos || cgiLocPos != 0)
		return (3);
	
	return (0);
}