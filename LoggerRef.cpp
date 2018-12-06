#include "pch.h"

#include "LoggerRef.h"
#include <string>

using namespace CLogger;

void CLogger::WinRT::Logger::WriteLine(CLogger::WinRT::LoggerLevel level, Platform::String ^ str)
{
	std::wstring strW(str->Begin());

	m_logger->WriteLine(ConvertLevel(level), strW);
}

void CLogger::WinRT::Logger::WriteLine(CLogger::WinRT::LoggerLevel level, Platform::String ^ str, Windows::Foundation::Collections::IVector<Platform::Object^>^ params)
{
	std::wstring strW(str->Begin());

	std::wstring strOut;

	bool inFormat = false;
	int paramIndex = 0;
	for (unsigned int i = 0; i < strW.size(); i++)
	{
		wchar_t ch = strW.at(i);

		if (ch == '{')
		{
			inFormat = true;
			paramIndex = 0;
		}

		if (inFormat)
		{
			if (ch >= '0' && ch <= '9')
			{
				paramIndex *= 10;
				paramIndex += (ch - '0');
			}
		}
		else
		{
			strOut.push_back(ch);
		}


		if (ch == '}')
		{
			inFormat = false;
			Platform::String^ param = params->GetAt(paramIndex)->ToString();

			std::wstring strParam(param->Begin());
			strOut.append(strParam);
		}
	}

	m_logger->WriteLine(ConvertLevel(level), strOut);
}
