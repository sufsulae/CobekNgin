#include "include\debug.h"

namespace cobek {
	const char* Debug::m_logFilePath = nullptr;
	Delegate<void(std::string)> Debug::LogCallback = Delegate<void(std::string)>();

	void Debug::Log(const char* fmt, ...) {
		
	}
	void Debug::SetLogFile(const char* logFile) {
		
	}
}