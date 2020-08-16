#include "include\file.h"
#include "filesystem\include\ghc\filesystem.hpp"

namespace fs = ghc::filesystem;
using namespace std;
namespace cobek {
	namespace FileSystem {
		//---DirectoryInfo---
		DirectoryInfo::DirectoryInfo(const char* path) {
			if (fs::is_directory(path)) {
				m_path = path;
			}
			else
				throw new std::exception("Path is not directory!");
		}
		bool DirectoryInfo::isExist() {
			return fs::exists(m_path);
		}
		std::string DirectoryInfo::get_parent() {
			auto path = fs::path(m_path);
			if (path.has_parent_path()) {
				return path.parent_path();
			}
			return std::string();
		}
		std::string DirectoryInfo::get_name() {
			auto path = fs::path(m_path);
			return path.filename();
		}
		std::string DirectoryInfo::get_root() {
			auto path = fs::path(m_path);
			if (path.has_root_path())
				return path.root_path();
			return std::string();
		}
		std::vector<std::string> DirectoryInfo::get_files() {
			auto path = fs::path(m_path);
			std::vector<std::string> paths;
			for (auto d : fs::directory_iterator(path)) {
				if (!d.is_directory()) {
					paths.push_back(d.path());
				}
			}
			return paths;

		}
		std::vector<std::string> DirectoryInfo::get_directories() {
			auto path = fs::path(m_path);
			std::vector<std::string> paths;
			for (auto d : fs::directory_iterator(path)) {
				if (d.is_directory()) {
					paths.push_back(d.path());
				}
			}
			return paths;
		}
		std::vector<std::string> DirectoryInfo::get_filesAndDirectories() {
			auto path = fs::path(m_path);
			std::vector<std::string> paths;
			for (auto d : fs::directory_iterator(path)) {
				paths.push_back(d.path());
			}
			return paths;
		}

		//---FileInfo---
		FileInfo::FileInfo(const char* path) {
			if (!fs::is_directory(path))
				m_path = path;
			else
				throw new std::exception("Path is not file!");
		}
		bool FileInfo::isExist() {
			return fs::exists(m_path);
		}
		std::string FileInfo::get_name() {
			auto path = fs::path(m_path);
			return path.filename();
		}
		std::string FileInfo::get_directory() {
			auto path = fs::path(m_path);
			return path.root_directory();
		}
		std::string FileInfo::get_extention() {
			auto path = fs::path(m_path);
			return path.extension();
		}

		//---Path---
		bool Path::isExist(const char* path) {
			return fs::exists(path);
		}
		bool Path::isDirectory(const char* path) {
			return fs::is_directory(path);
		}
		bool Path::isFile(const char* path) {
			auto p = fs::path(path);
			return fs::is_block_file(p) || fs::is_character_file(path) || fs::is_regular_file(path);
		}

		//---Directory---
		int Directory::Create(const char* path) {
			if (path != nullptr)
				return false;
			return fs::create_directories(path);
		}
		int Directory::Delete(const char* path) {
			if (path != nullptr)
				return false;
			if (fs::is_directory(path))
				return fs::remove(path);
			return false;
		}
		std::string Directory::CurrentPath() {
			return fs::current_path();
		}
		std::string Directory::TempDirectory() {
			return fs::temp_directory_path();
		}
		std::vector<std::string> Directory::GetFilesAndDirectories(const char* path) {
			std::vector<std::string> paths;
			for (auto d : fs::directory_iterator(path)) {
				paths.push_back(d.path());
			}
			return paths;
		}

		//---File---
		ullong File::GetSize(const char* path) {
			return fs::file_size(path);
		}
		int File::Delete(const char* path) {
			return fs::remove(path);
		}
		int File::Copy(const char* from, const char* to) {
			return fs::copy_file(from, to);
		}
		int File::ReadAllBytes(const char* path, const char** fileData, ullong* fileSize) {
			if (path == nullptr || fileData == nullptr)
				return false;
			auto f = new fs::ifstream(path);
			if (f->is_open()) {
				f->seekg(0, ios::end);
				auto fSize = f->tellg();

				auto fBuff = new char[fSize];
				f->seekg(0, ios::beg);
				f->read(fBuff, fSize);
				*fileData = fBuff;
				if (fileSize != nullptr)
					*fileSize = fSize;
				f->close();
				delete f;
				return true;
			}
			delete f;
			return false;
		}
		int File::WriteAllBytes(const char* path, const char* data, ullong size) {
			if (path == nullptr)
				return false;
			auto f = new fs::ofstream(path);
			f->write(data, size);
			f->close();
			delete f;
			return true;
		}
		int File::WriteAllBytes(const char* path, const char* data) {
			if (path == nullptr)
				return false;
			auto f = new fs::ofstream(path);
			*f << data;
			f->close();
			delete f;
			return true;
		}
	}
}