#pragma once
#ifndef COBEK_UTILITY_SYS_H
#define COBEK_UTILITY_SYS_H

#include "include\headers.h"

namespace cobek {
	namespace Utility {
		namespace Sys {
			inline const wchar_t* LastErr() {
			#if defined(_WIN32)
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf,
					0, NULL);
				return (const wchar_t*)lpMsgBuf;
			#else
				return std::wstring();
			#endif
			}
		}
	}
}
#endif // !COBEK_UTILITY_SYS_H
