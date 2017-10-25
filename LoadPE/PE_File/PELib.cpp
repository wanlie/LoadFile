#include "stdafx.h"
#include "PELib.h"

//大于对齐就+1
#define ALIGN_VALUE_UP(value, alignment)    (((value) + (alignment) - 1) & ~((alignment) - 1))

PELib::PELib()
{
	b_loaded = false;
	b_image64 = false;
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
	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(buffer);
	do 
	{
		if (b_loaded)
		{
			return false;
		}
		if (buffer_size<sizeof(IMAGE_DOS_HEADER)) //??
		{
			std::cout << "failed size dos_header"<<std::endl;
			break;
		}
		if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
		{
			std::cout << "failed dos sig" << std::endl;
			break;
		}
		if (buffer_size<dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS32))
		{
			std::cout << "failed size nt size" << std::endl;
			break;
		}
		auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS32>(reinterpret_cast<char *>(dos_header)+dos_header->e_lfanew);
		if (nt_header->Signature != IMAGE_NT_SIGNATURE)
		{
			std::cout << "failed nt sig " << nt_header->Signature << std::endl;
			break;
		}
		if (nt_header->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
		{
			b_image64 = true;
			return load_pe_image64(buffer, buffer_size);
		}
		if (nt_header->OptionalHeader.SectionAlignment & 1) //??
		{
			std::cout << "alignment size is 1" << std::endl;
			break;
		}
		SYSTEM_INFO sysInfo;
		GetNativeSystemInfo(&sysInfo);
		auto image_size = ALIGN_VALUE_UP(nt_header->OptionalHeader.SizeOfImage, sysInfo.dwPageSize);
		image_size = ALIGN_VALUE_UP(image_size, nt_header->OptionalHeader.SectionAlignment);
		auto section_header = IMAGE_FIRST_SECTION(nt_header);
		for (int i = 0; i < nt_header->FileHeader.NumberOfSections;i++)
		{
			if ((section_header[i].PointerToRawData + section_header[i].SizeOfRawData)>(DWORD)buffer_size)
			{
				std::cout << "failed size" << std::endl;
				return false;
			}			
		}
		for (int i = 0; i < nt_header->FileHeader.NumberOfSections; ++i)
		{
			//得到该节的大小
			auto nCodeSize = section_header[i].Misc.VirtualSize;
			auto nLoadSize = section_header[i].SizeOfRawData;
			auto nMaxSize = (nLoadSize > nCodeSize) ? (nLoadSize) : (nCodeSize);
			auto nSectionSize = ALIGN_VALUE_UP(section_header[i].VirtualAddress + nMaxSize, sysInfo.dwPageSize);

			if (image_size < nSectionSize)
			{
				image_size = nSectionSize;  //Use the Max;
			}
		}


	} while (0);
	

	return false;
}


bool PELib::load_pe_image64(PVOID buffer, SIZE_T buffer_size)
{

	return false;
}
