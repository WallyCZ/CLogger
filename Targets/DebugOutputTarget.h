#pragma once
#include "../ILogTarget.h"
#include <Windows.h>


namespace CLogger
{

	class DebugOutputTarget : public ILogTarget
	{
	public:
		virtual void OutStringW(const wchar_t * str)
		{
			::OutputDebugStringW(str);
		}
	};



}

