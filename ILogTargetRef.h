#pragma once


#ifndef __cplusplus_winrt
#error LoggerRef.h requires the /ZW compiler option.
#endif


namespace CLogger
{
	namespace WinRT
	{

		//WinRT Store apps does not allow expose Platform::IntPtr, so we must use this ugly thing (its some better way?)
#ifdef _WIN64
		typedef int64 MyIntPtr;
#else
		typedef int32 MyIntPtr;
#endif

		public interface class ILogTarget
		{
			MyIntPtr TransferTarget();
		};

	}
}

