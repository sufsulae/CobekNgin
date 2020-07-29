#include "include\utility\fileUtil.h"
#include "include\utility\inlineUtil.h"
#include "filesystem\include\ghc\filesystem.hpp"


#if _WIN32
	#undef CreateDirectory
#endif

namespace fs = ghc::filesystem;
namespace cobek {
	namespace Utility {
		//---Directory---
		bool Directory::CreateDirectory(const char* path) {
			return fs::create_directory(path);
		}
		bool Directory::CreateDirectory(const wchar_t* path) {
			return fs::create_directory(wchar2mbchar(path));
		}

		std::string Directory::GetCurrentPath() {
			return fs::current_path();
		}
		std::string Directory::GetAbsolutePath(const char* path) {
			return fs::absolute(fs::path(path)).c_str();
		}
		std::wstring Directory::GetAbsolutePath(const wchar_t* path) {
			return fs::absolute(fs::path(wchar2mbchar(path))).generic_wstring();
		}

		//---DirectoryInfo---
		DirectoryInfo::DirectoryInfo(const char* path) {
			this->m_path = mbchar2wchar(path);
		}
		DirectoryInfo::DirectoryInfo(const wchar_t* path) {
			this->m_path = path;
		}

		bool DirectoryInfo::isExist() {
			return fs::exists(fs::path(m_path));
		}
		bool DirectoryInfo::hasParent() {
			return fs::path(m_path).has_parent_path();
		}
		bool DirectoryInfo::hasRoot() {
			return fs::path(m_path).has_root_directory();
		}
		bool DirectoryInfo::hasRelativePath() {
			return fs::path(m_path).has_relative_path();
		}

		std::string DirectoryInfo::getPath() {
			return wchar2mbchar(m_path.c_str()); 
		}
		std::wstring DirectoryInfo::getPathUni() {
			return m_path.c_str();
		}
		std::string DirectoryInfo::getName() {
			return fs::path(m_path).filename().c_str();
		}

		DirectoryInfo DirectoryInfo::getParent() {
			auto path = fs::path(m_path);
			if (path.has_parent_path())
				return DirectoryInfo(path.parent_path().c_str());;
			return *this;
		}
		std::vector<FileInfo> DirectoryInfo::getFiles() {
			std::vector<FileInfo> res;
			for (auto& f : fs::directory_iterator(m_path)) {
				if (!f.is_directory())
					res.push_back(FileInfo(f.path().c_str()));
			}
			return res;
		}
		std::vector<DirectoryInfo> DirectoryInfo::getDirectories() {
			std::vector<DirectoryInfo> res;
			for (auto& f : fs::directory_iterator(m_path)) {
				if (f.is_directory())
					res.push_back(DirectoryInfo(f.path().c_str()));
			}
			return res;
		}

		//---FileInfo---
		FileInfo::FileInfo(const char* path) {
			auto p = fs::path(path);
			if (!fs::is_directory(p)) {
				this->m_path = mbchar2wchar(path);
			}
		}
		FileInfo::FileInfo(const wchar_t* path) {
			auto c = wchar2mbchar(path);
			auto p = fs::path(c);
			if (!fs::is_directory(p)) {
				this->m_path = path;
			}
		}

		bool FileInfo::isExist() {
			return fs::exists(fs::path(m_path));
		}

		DirectoryInfo FileInfo::getDirectory() {
			return DirectoryInfo(fs::path(m_path).relative_path().c_str());
		}
		std::string FileInfo::getName() {
			return fs::path(m_path).filename();
		}
		std::string FileInfo::getExtention() {
			return fs::path(m_path).extension();
		}
		std::string FileInfo::getFullName() {
			return fs::path(m_path).root_path();
		}
	}
}