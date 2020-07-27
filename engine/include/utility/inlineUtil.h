#pragma once
#ifndef COBEK_UTILITY_INLINE_H
#define COBEK_UTILITY_INLINE_H

#include "include\headers.h"
namespace cobek {
	namespace Utility {
		inline std::string strprintf(const char* fmt, ...) {
			char buff[4096];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buff, sizeof(buff), fmt, args);
			va_end(args);
			return std::string(buff);
		}
		inline std::wstring strwprintf(const wchar_t* fmt, ...) {
			wchar_t buff[4096];
			va_list args;
			va_start(args, fmt);
#if COBEK_PLATFORM_WIN
			_vsnwprintf_s(buff, sizeof(buff), fmt, args);
#else
			_vsnwprintf(buff, sizeof(buff), fmt, args);
#endif
			va_end(args);
			return std::wstring(buff);
		}
		inline bool strcontains(const char* src, const char* tData) {
			size_t srcLen = strlen(src);
			size_t tDataLen = strlen(tData);
			if (tData == NULL || src == NULL || srcLen < tDataLen)
				return false;
			size_t mLen = 0;
			for (size_t i = 0; i < srcLen; i++)
			{
				if (src[i] == tData[mLen]) {
					mLen++;
					if (mLen >= tDataLen)
						return true;
				}
				else {
					mLen = 0;
				}
			}
			return false;
		}
		inline bool strcontainsLast(const char* src, const char* tData) {
			size_t srcLen = strlen(src);
			size_t tDataLen = strlen(tData);
			if (tData == NULL || src == NULL)
				return false;
			for (size_t i = 0; i < tDataLen; i++) {
				size_t srcP = srcLen - (tDataLen - i);
				if (i >= 0 && src[srcP] != tData[i])
					return false;
			}
			return true;
		}
		inline const char* wchar2mbchar(const wchar_t* string) {
			size_t count = 0;
			size_t size = (wcslen(string) + 1) * 2;
			char* newString = new char[size];
#if COBEK_PLATFORM_WIN
			wcstombs_s(&count, newString, size, string, _TRUNCATE);
#else
			wcstombs(newString, string, size);
#endif
			return newString;
		}
		inline const wchar_t* mbchar2wchar(const char* string) {
			size_t count = 0;
			size_t size = strlen(string) + 1;
			wchar_t* newString = new wchar_t[size];
#if COBEK_PLATFORM_WIN
			mbstowcs_s(&count, newString, size, string, _TRUNCATE);
#else
			mbstowcs(newString, string, size);
#endif
			return newString;
		}
	}
}

#endif // !COBEK_UTILITY_INLINE_H
