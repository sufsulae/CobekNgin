#pragma once
#ifndef COBEK_UTILITY_DEBUG_H
#define COBEK_UTILITY_DEBUG_H

#include "include\headers.h"

namespace cobek {
	namespace Utility {
		class Debug final {
		private:

		public:
			static void Log(const char* fmt, ...);
		};
	}
}

#endif