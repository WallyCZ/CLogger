#pragma once
#include "../ILogTarget.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
using namespace std;

namespace CLogger
{
	class FileOutput : public ILogTarget
	{
		ofstream myfile;

	public:
		FileOutput(const wchar_t * path, bool trunc = true);
		FileOutput(const std::wstring& path, bool trunc = true);
		virtual ~FileOutput();

		virtual void OutStringW(const wchar_t * str);

	};
}

