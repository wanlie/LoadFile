// PE_File.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PELib.h"


int _tmain(int argc, _TCHAR* argv[])
{
	PELib pe;
	if (argc!=2)
	{
		if (pe.load_pe_file(std::string("E:\\11.exe")))
		{
			std::cout << "11.exe" << std::endl;
		}
		return 1;
	}
	
	size_t   i;
	char pe_name[MAX_PATH]={0};
	wcstombs_s(&i, reinterpret_cast<char *>(pe_name), (size_t)(sizeof(argv[1]) + 2),
		argv[1], (size_t)(sizeof(argv[1])+1));
	std::cout << pe_name << std::endl;
	if (pe.load_pe_file(std::string(reinterpret_cast<char *>(pe_name))))
	{
		std::cout << "load ok" << std::endl;
	}
	getchar();
	getchar();
	return 0;
}

