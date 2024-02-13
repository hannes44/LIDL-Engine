#pragma once
#include <string>
#include <Windows.h>
#include <atlstr.h>

#include <chrono>

namespace engine
{
	class Utils
	{
	public:
		static long long getTimestampNS();

		// [in] command to execute
		static 	CStringA ExecCmd(const wchar_t* cmd);
	};
}