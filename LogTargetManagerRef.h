#pragma once
#include "ILogTarget.h"
#include "ILogTargetRef.h"
#include "LogTargetManager.h"

namespace CLogger
{
	namespace WinRT
	{
		public ref class LogTargetManager sealed
		{
		public:

			/*
			* Please note, that if you use this method, you can't use logTarget instance second time,
			* internal object in logTarget is transfered to C++ part and is no more avaiable for this method
			*/
			static void AddLogTarget(ILogTarget^ logTarget)
			{
				CLogger::ILogTarget* obj = reinterpret_cast<CLogger::ILogTarget*>((void*)logTarget->TransferTarget());


				CLogger::LogTargetManager::AddLogTarget(std::shared_ptr<CLogger::ILogTarget>(obj));
			}

			static void ReleaseAllTargets()
			{
				CLogger::LogTargetManager::ReleaseAllTargets();
			}
		};
	}
}
