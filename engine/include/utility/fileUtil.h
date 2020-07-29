#pragma once
#ifndef COBEK_UTILITY_FILE_H
#define COBEK_UTILITY_FILE_H

#include "include\headers.h"
#if _WIN32
	#undef CreateDirectory
#endif
namespace cobek {
	namespace Utility {
		struct FileStream;
		struct FileInfo;

		enum class FileOpenMode {
			READ = 1,
			WRITE,
			READWRITE,
			ATEND,
			APPEND = 4,
			TRUNCATE = 8,
			BINARY = 32,
			NOCREATE = 64,
			NOREPLACE = 128
		};

		struct DirectoryInfo {
		private:
			std::wstring m_path;
		public:
			DirectoryInfo(const char* path);
			DirectoryInfo(const wchar_t* path);

			bool isExist();
			bool hasParent();
			bool hasRoot();
			bool hasRelativePath();

			std::string getPath();
			std::wstring getPathUni();
			std::string getName();

			std::vector<FileInfo> getFiles();
			std::vector<DirectoryInfo> getDirectories();
			DirectoryInfo getParent();
		};

		struct FileInfo {
		private:
			std::wstring m_path;
		public:
			FileInfo(const char* filePath);
			FileInfo(const wchar_t* filePath);
			bool isExist();
			DirectoryInfo getDirectory();
			std::string getName();
			std::string getExtention();
			std::string getFullName();
		};

		class Directory final {
		public:
			static bool CreateDirectory(const char* dir);
			static bool CreateDirectory(const wchar_t* dir);

			static std::string GetCurrentPath();
			static std::string GetAbsolutePath(const char* path);
			static std::wstring GetAbsolutePath(const wchar_t* path);
		};
	}
}
#if _WIN32
	#ifdef UNICODE
		#define CreateDirectory CreateDirectoryW
	#else
		#define CreateDirectory CreateDirectoryA
	#endif // !UNICODE
#endif
#endif // !COBEK_UTILITY_FILE_H
