#pragma once
#ifndef COBEK_TIME_H
#define COBEK_TIME_H

#include "headers.h"
namespace cobek {
	namespace Time {
		class Stopwatch {
		private:
			bool m_running;
			bool m_reset;
		public:
			Stopwatch();
			~Stopwatch();
			void Start();
			void Pause();
			void Stop();
			void Reset();
			bool isRunning();
			llong get_nanoseconds();
			llong get_microseconds();
			llong get_milliseconds();
			llong get_seconds();
		};

		class TimeSpan {
		public:

		};

		class DateTime {
		public:

		};
	}
}
#endif // !COBEK_TIME_H
