#pragma once
#ifndef COBEK_UTILITY_FILE_H
#define COBEK_UTILITY_FILE_H

#include "include\headers.h"
namespace cobek {
	namespace FileSystem {
		class DirectoryInfo {
		private:
			std::string m_path;
		public:
			DirectoryInfo(const char* path);
			std::string get_parent();
			std::string get_name();
			std::string get_root();
			std::vector<std::string> get_files();
			std::vector<std::string> get_directories();
			std::vector<std::string> get_filesAndDirectories();
			bool isExist();
		};

		class FileInfo {
		private:
			std::string m_path;
		public:
			FileInfo(const char* path);
			std::string get_name();
			std::string get_directory();
			std::string get_extention();
			bool isExist();
		};

		class Path final {
		public:
			static bool isExist(const char* path);
			static bool isDirectory(const char* path);
			static bool isFile(const char* path);
		};

		class Directory final {
		public:
			static int Create(const char* path);
			static int Delete(const char* path);
			static std::string TempDirectory();
			static std::string CurrentPath();
			static std::vector<std::string> GetFilesAndDirectories(const char* path);
		};

		class File final {
		public:
			static int Delete(const char* path);
			static int Copy(const char* srcPath, const char* dstPath);
			static ullong GetSize(const char* path);
			static int ReadAllBytes(const char* path, const char** fileData, ullong* fileSize = nullptr);
			static int WriteAllBytes(const char* path, const char* data);
			static int WriteAllBytes(const char* path, const char* data, ullong size);
			static int WriteAllBytesToEnd(const char* path, const char* data);
			static int WriteAllBytesToEnd(const char* path, const char* data, ullong size);
		};
	}
}

#endif