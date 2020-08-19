#pragma once
#ifndef COBEK_UTILITY_DEBUG_H
#define COBEK_UTILITY_DEBUG_H

#include "include\common.h"

#ifndef COBEK_UTILITY_FILE_H
#include "include\file.h"
#endif

#ifndef COBEK_UTILITY_INLINE_H
#include "include\utility\inline.h"
#endif

namespace cobek {
	class Debug final {
	private:
		static std::string m_logFilePath;
	public:
		static Delegate<void(std::string)> LogCallback;
		static void SetLogFile(const char* logFile);
		template<typename ... Args> static void Log(const char* fmt, Args... args) {
			auto str = Utility::strformat(fmt, args...) + "\n";
			printf("%s", str.c_str());
			if (!m_logFilePath.empty())
				FileSystem::File::WriteAllBytesToEnd(m_logFilePath.c_str(), str.c_str());
			LogCallback.invoke(str);
		}
	};
}

#endif