#pragma once
#ifndef COBEK_UTILITY_DEBUG_H
#define COBEK_UTILITY_DEBUG_H

#include "include\common.h"
namespace cobek {
	class Debug final {
	private:
		static const char* m_logFilePath;
	public:
		static Delegate<void(std::string)> LogCallback;
		static void Log(const char* fmt, ...);
		static void SetLogFile(const char* logFile);
	};
}

#endif