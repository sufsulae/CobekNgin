#ifndef UNICODE
#define UNICODE
#endif
#include "include\utility\fileUtil.h"
#include "include\utility\inlineUtil.h"
#include "include\plugin\mono\mono.h"

using namespace cobek;
using namespace cobek::Utility;
using namespace cobek::Plugin::Mono;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
//	MonoMgr::CreateDomain("Test");
//	printf("Stop Here");
//	return 0;
//}

int main(int argc, char* argv[]) {
	auto path = DirectoryInfo(Directory::GetCurrentPath().c_str());
	auto parent = path.getParent();
	auto testDir = parent.getPath() + "\\test-lib";
	MonoManager::SetDirectory(testDir.c_str(), testDir.c_str());
	auto dom = MonoManager::CreateDomain("Test");
	auto assembly = MonoAssembly(dom,(testDir + "\\EditorGUI.dll").c_str());
	if (assembly.isLoaded()) {
		printf("Assembly Created: %s \n", assembly.get_Name());
		auto asmImage = assembly.get_Image();
		printf("Image name: %s \n", asmImage->get_Filename());
	}
	return 0;
}