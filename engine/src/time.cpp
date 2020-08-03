#include "include\time.h"
#include <chrono>
using namespace std::chrono;
namespace cobek {
	namespace Time {
		struct _timeDiff {
			time_point<high_resolution_clock> min;
			time_point<high_resolution_clock> max;
		};
		std::unordered_map<Stopwatch*, _timeDiff> _pool = std::unordered_map<Stopwatch*, _timeDiff>();

		//---Stopwatch---
		Stopwatch::Stopwatch() {
			m_running = false;
			m_reset = true;
			_pool[this] = _timeDiff();
		}
		Stopwatch::~Stopwatch() {
			_pool.erase(this);
		}
		void Stopwatch::Start() {
			if (m_running)
				return;

			auto& data = _pool.at(this);
			if (!m_reset) {
				data.min = high_resolution_clock::now();
				m_reset = false;
			}
			m_running = true;
		}
		void Stopwatch::Pause() {
			m_running = false;
		}
		void Stopwatch::Stop() {
			auto& data = _pool.at(this);
			data.max = high_resolution_clock::now();
			m_running = false;
		}
		void Stopwatch::Reset() {
			auto& data = _pool.at(this);
			data.min = high_resolution_clock::now();
			data.max = high_resolution_clock::now();
			m_running = false;
			m_reset = true;
		}
		bool Stopwatch::isRunning() {
			return m_running;
		}
		llong Stopwatch::get_nanoseconds() {
			auto& data = _pool.at(this);
			if (m_running)
				data.max = high_resolution_clock::now();
			return duration_cast<nanoseconds>(data.max - data.min).count();
		}
		llong Stopwatch::get_microseconds() {
			auto& data = _pool.at(this);
			if (m_running)
				data.max = high_resolution_clock::now();
			return duration_cast<microseconds>(data.max - data.min).count();
		}
		llong Stopwatch::get_milliseconds() {
			auto& data = _pool.at(this);
			if (m_running)
				data.max = high_resolution_clock::now();
			return duration_cast<milliseconds>(data.max - data.min).count();
		}
		llong Stopwatch::get_seconds() {
			auto& data = _pool.at(this);
			if (m_running)
				data.max = high_resolution_clock::now();
			return duration_cast<seconds>(data.max - data.min).count();
		}
	}
}