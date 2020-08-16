#ifndef UNICODE
#define UNICODE
#endif
#include "include\time.h"
#include "include\utility\file.h"
#include "include\plugin\mono\mono.h"

using namespace cobek;
using namespace cobek::Plugin::Mono;
using namespace cobek::Utility::FileSystem;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
//	MonoMgr::CreateDomain("Test");
//	printf("Stop Here");
//	return 0;
//}

int main(int argc, char* argv[]) {
	auto path = DirectoryInfo(Directory::CurrentPath().c_str());
	auto parent = path.get_parent();
	auto testDir = parent + "\\test-lib";
	MonoManager::SetDirectory((testDir + "\\mono").c_str());
	auto dom = MonoManager::CreateDomain("Test","v6.10.0.104");
	//auto netStandard = MonoAssembly(dom, (testDir + "\\netstandard.dll").c_str());
	auto assembly = MonoAssembly(dom, (testDir + "\\Designer.dll").c_str());
	if (assembly.is_Loaded()) {
		printf("Assembly Opened: %s \n", assembly.get_name().c_str());
		auto asmImage = assembly.get_Image();
		printf("Image name: %s \n", asmImage->get_Filename().c_str());
		auto asmClass = asmImage->get_class("editor", "App");
		if (asmClass != nullptr) {
			printf("Class name: %s \n", asmClass->get_name().c_str());
			auto methodList = asmClass->get_method("Main");
			if (methodList.size() > 0) {
				printf("Method Name: %s \n", methodList[0]->get_name().c_str());
				methodList[0]->Invoke(nullptr, nullptr);
			}
		}
	}
	return 0;
}