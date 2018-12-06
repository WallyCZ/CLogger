#include "../pch.h"
#include "FileOutput.h"
#include <codecvt>
#include <string>


using namespace CLogger;

FileOutput::FileOutput(const wchar_t * path, bool trunc)
{
	if(trunc)
		myfile.open(path, std::ofstream::out | std::ofstream::trunc, _SH_DENYWR);
	else
		myfile.open(path, std::ofstream::out | std::ofstream::app, _SH_DENYWR);
}


FileOutput::FileOutput(const std::wstring& path, bool trunc)
{
	if (trunc)
		myfile.open(path.c_str(), std::ofstream::out | std::ofstream::trunc, _SH_DENYWR);
	else
		myfile.open(path.c_str(), std::ofstream::out | std::ofstream::app, _SH_DENYWR);
}


FileOutput::~FileOutput()
{
	myfile.close();
}


void FileOutput::OutStringW(const wchar_t * str)
{
	std::wstring wstr(str);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	myfile << myconv.to_bytes(str);
	myfile.flush();
}
