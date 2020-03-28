#include "myftp.h"
#include <atlconv.h>
#include <sstream>
#define _CRT_SECURE_NO_WARNINGS


myftp::myftp()
{
	char buf[120];
	getcwd(buf, sizeof(buf));
	std::string t = buf;
	commandFileP = t + "/myftp";

}


myftp::~myftp()
{
}


bool myftp::upload(std::string dir,std::string name)
{
	if (count == 0) {
		count_f++;
		std::stringstream ss;
		ss << commandFileP << count_f<<".tmp";

		commandFilePath = ss.str();
		
		comfile = fopen(commandFilePath.c_str(), "w");
		if (comfile)
		{
			fprintf(comfile, "open %s %s\n"
				"USER %s\n"
				"%s\n"
				"binary\n"
				"prompt n\n",
				FTPServer, FTPPort, FTPUsername, FTPPassword);
		}
	}
	count++;
	std::string thisdirectory = uploadDirectory + dir;
	fprintf(comfile, "cd %s\n"
			"mput %s\n", thisdirectory.c_str(), ("../../"+dir+name).c_str());

	if (count == 1) {
		//printf("chuanshu");
		fprintf(comfile, "quit\n");
		fclose(comfile);
		FILE *sfile = fopen(ShScript, "w");
		//printf("%s\n", commandFilePath.c_str());
			if (sfile)
			{
				count = 0;
				std::string orig = "../" + dir;
				fprintf(sfile, "ftp.exe -n -s:%s\n"
					//"del %s\n"
					, commandFilePath.c_str()
					);
				fclose(sfile);
				// 调用ftp.exe，执行批量脚本，然后删除txt文件
				// 通过ShellExecuteA函数执行脚本
				ShellExecute(NULL, L"open", script, NULL, NULL, SW_HIDE);
			}
			else
			{
				printf("[Error] create shell script .bat:  %s failed!\n", ShScript);
				return false;
			}
	}
		
	return 0;
}
