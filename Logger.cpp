#include "pch.h"
#include "Logger.h"
#include "LogTargetManager.h"
#include <sstream>

#include <winapifamily.h>
#include <time.h>
#include <chrono>
#include <cwchar>


using namespace CLogger;

Logger::Logger()
{
}

std::unique_ptr<Logger> Logger::get(const std::string className)
{
	std::unique_ptr<Logger> instance(new Logger());
	instance->m_className = className;
	return instance;
}



void CLogger::Logger::OutStringW(const wchar_t * str)
{
	LogTargetManager::WriteToAllTargets(str);
}

void CLogger::Logger::WritePrefixW(CLogger::LoggerLevel level, std::wstring& tmp)
{
	typedef std::chrono::system_clock Clock;

	Clock::time_point tp = Clock::now();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
	auto fraction = tp.time_since_epoch() - seconds;
	auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

	time_t t = Clock::to_time_t(tp);

	struct tm timeinfo;

	//TODO wcsftime to C++11 approach
	wchar_t wstr[40];
	if (localtime_s(&timeinfo, &t)==0 && std::wcsftime(wstr, 40, L"%Y-%m-%d %H:%M:%S.", &timeinfo)) {
		tmp.append(wstr);
		tmp.append(std::to_wstring(miliseconds.count()));
	}


	switch (level)
	{
	case CLogger::LoggerLevel::LOGGER_LEVEL_TRACE:
		tmp.append(L" [T] "); break;
	case CLogger::LoggerLevel::LOGGER_LEVEL_DEBUG:
		tmp.append(L" [D] "); break;
	case CLogger::LoggerLevel::LOGGER_LEVEL_INFO:
		tmp.append(L" [I] "); break;
	case CLogger::LoggerLevel::LOGGER_LEVEL_WARN:
		tmp.append(L" [W] "); break;
	case CLogger::LoggerLevel::LOGGER_LEVEL_ERROR:
		tmp.append(L" [E] "); break;
	case CLogger::LoggerLevel::LOGGER_LEVEL_FATAL:
		tmp.append(L" [F] "); break;
	}
	tmp.append(L"[");
	tmp.append(std::wstring(m_className.begin(), m_className.end()));
	tmp.append(L"] ");
}


void CLogger::Logger::WriteLine(CLogger::LoggerLevel level, const std::wstring & str)
{
	std::wstring strOut;
	WritePrefixW(level, strOut);
	strOut.append(str);
	strOut.append(L"\n");
	OutStringW(strOut.c_str());
}

void CLogger::Logger::WriteLine(CLogger::LoggerLevel level, const std::string & str)
{
	std::wstring strOut;
	WritePrefixW(level, strOut);
	strOut.append(std::wstring(str.begin(), str.end()));
	strOut.append(L"\n");
	OutStringW(strOut.c_str());
}

void CLogger::Logger::WriteLine(CLogger::LoggerLevel level, const char * str)
{
	std::wstring strOut;
	std::string strIn(str);
	WritePrefixW(level, strOut);
	strOut.append(std::wstring(strIn.begin(), strIn.end()));
	strOut.append(L"\n");
	OutStringW(strOut.c_str());
}
 

DLLEXPORT void CLogger::Logger::WriteLineFormat(CLogger::LoggerLevel level, const std::wstring & format, va_list args)
{
	std::wstringstream out;

	enum formatTypeEnum
	{
		UNKNOWN,
		INT,
		INT64,
		STRING,
		WSTRING
	};

	enum formatStageEnum
	{
		FORMAT_STRING,
		INDEX,
		FORMAT_TYPE
	};

	int paramIndex = -1;
	formatTypeEnum formatType = formatTypeEnum::UNKNOWN;
	formatStageEnum stage = formatStageEnum::FORMAT_STRING;

	for (unsigned int i = 0; i < format.size(); i++)
	{
		wchar_t ch = format.at(i);

		if (stage == formatStageEnum::INDEX)
		{
			if (ch >= '0' && ch <= '9')
			{
				paramIndex *= 10;
				paramIndex += (ch - '0');
			}
			else if (ch == ':')
			{
				stage = formatStageEnum::FORMAT_TYPE;
			}
			else
			{
				stage = formatStageEnum::FORMAT_STRING;
			}
		}
		else if (stage == formatStageEnum::FORMAT_TYPE)
		{
			stage = formatStageEnum::FORMAT_STRING;
			switch (ch)
			{
			case 'd' :
				formatType = formatTypeEnum::INT; continue;
			case 'q':
				formatType = formatTypeEnum::INT64; continue;
			case 's':
				formatType = formatTypeEnum::STRING; continue;
			case 'w':
				formatType = formatTypeEnum::WSTRING; continue;
			}
		}
		
		if (stage == formatStageEnum::FORMAT_STRING)
		{
			if (ch == '{')
			{
				stage = formatStageEnum::INDEX;
				formatType = formatTypeEnum::UNKNOWN;
				paramIndex = 0;
			}
			else if (ch == '}')
			{
				switch (formatType)
				{
				case formatTypeEnum::INT :
					out << va_arg(args, int); break;
				case formatTypeEnum::INT64:
					out << va_arg(args, int64_t); break;
				case formatTypeEnum::STRING:
					out << va_arg(args, const char *); break;
				case formatTypeEnum::WSTRING:
					out << va_arg(args, const wchar_t *); break;
				}
			}
			else
			{
				out << ch;
			}
		}

	}

	WriteLine(level, out.str());
}

DLLEXPORT void CLogger::Logger::WriteLineFormat(CLogger::LoggerLevel level, const std::wstring format, ...)
{
	va_list args;
	va_start(args, format);
	WriteLineFormat(level, format, args);
}