#pragma once
#include <vector>
#include <mutex>
#include "ILogTarget.h"

#if !defined(DLLEXPORT)
#if defined(CompactLogger_EXPORTS)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#endif


namespace CLogger
{
	class LogTargetManager
	{
		static std::vector<std::shared_ptr<ILogTarget>> m_targets;
		static std::mutex m_mtx;
	public:

		DLLEXPORT static void AddLogTarget(std::shared_ptr<ILogTarget> logTarget);

		DLLEXPORT static void WriteToAllTargets(const wchar_t * str);

		DLLEXPORT static void ReleaseAllTargets();
	};


}

