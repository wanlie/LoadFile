#pragma once
#include <windows.h>
#include <winnt.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

//#include <iomanip>



class PELib
{
public:
	PELib();
	~PELib();
public:
	bool load_pe_file(std::string file_name);
	bool load_pe_image(PVOID buffer, SIZE_T buffer_size);
private:
	std::vector<BYTE> file_buffer;

	bool b_loaded;
	bool b_image64;

public:
	
	bool load_pe_image64(PVOID buffer, SIZE_T buffer_size);
};

