#pragma once

#include <string>
#include <Windows.h>
#include<direct.h>
class myftp
{
public:
	myftp();
	~myftp();

private:
	char* FTPUsername = "fania";
	char* FTPServer = "167.179.77.155";
	char* FTPPort = "21";
	char* FTPPassword = "zhouyangfan";
	std::string uploadDirectory = "/usr/fania/apache-tomcat-8.5.28/webapps/trafficVideo/assets/module/img/";
	std::string commandFileP;
	std::string commandFilePath;
	char* ShScript = ".\\shell\\myftp.bat";
	wchar_t* script = L".\\shell\\myftp.bat";
	int count = 0;
	FILE *comfile;
	int count_f = 0;

public:
	bool upload(std::string dir,std::string name);
};

