#include "stdafx.h"
#include "PELib.h"


PELib::PELib()
{
}


PELib::~PELib()
{
	file_buffer.clear();
	file_buffer.resize(1);
}


bool PELib::load_pe_file(std::string file_name)
{
	try{
		std::ifstream file_stream(file_name, std::ios::binary);
		file_stream.seekg(0, std::ios::end);
		auto length = std::size_t(file_stream.tellg());
		file_stream.seekg(0, std::ios::beg);
		auto buff = new BYTE[length];
		file_buffer.resize(length);
		file_stream.read(reinterpret_cast<char *>(buff), length);
		RtlCopyMemory(&file_buffer[0], buff, length);
		delete[] buff;
		//file_buffer = std::vector<BYTE>((std::istream_iterator<BYTE>(file_stream)), (std::istream_iterator<BYTE>()));
		file_stream.close();
		return load_pe_image(file_buffer.data(), file_buffer.size());
		//return 1;
	}
	catch (const std::exception&)
	{
		std::cout << "faild" << std::endl;
	}
	return false;
}


bool PELib::load_pe_image(PVOID buffer, SIZE_T buffer_size)
{

	return false;
}
