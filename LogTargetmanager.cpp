#include "pch.h"
#include "LogTargetManager.h"
#include <string>
#include <memory>
#include <cstdarg>

namespace CLogger
{

	std::vector<std::shared_ptr<ILogTarget>> LogTargetManager::m_targets;
	std::mutex LogTargetManager::m_mtx;

	DLLEXPORT void LogTargetManager::AddLogTarget(std::shared_ptr<ILogTarget> logTarget)
	{
		m_targets.push_back(logTarget);
	}
	DLLEXPORT void LogTargetManager::WriteToAllTargets(const wchar_t * str)
	{
		m_mtx.lock();
		for (auto target : m_targets)
		{
			target->OutStringW(str);
		}
		m_mtx.unlock();
	}
	DLLEXPORT void LogTargetManager::ReleaseAllTargets()
	{
		m_mtx.lock();
		for (auto target : m_targets)
		{
			target.reset();
		}
		m_targets.clear();
		m_mtx.unlock();
	}
}