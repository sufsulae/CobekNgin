#include "include\debug.h"
namespace cobek {
	std::string Debug::m_logFilePath = std::string();
	Delegate<void(std::string)> Debug::LogCallback = Delegate<void(std::string)>();
	void Debug::SetLogFile(const char* logFile) {
		m_logFilePath = logFile;
	}
}