#ifndef UNICODE
#define UNICODE
#endif
#include "include\common.h"
#include "include\debug.h"
#include "include\time.h"
#include "include\file.h"
#include "include\graphic.h"
#include "include\plugin\mono\mono.h"

using namespace cobek;
namespace Mono = ::Plugin::Mono;
namespace fs = ::FileSystem;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
//
//	return 0;
//}

//Load Library
Mono::MonoAssembly assembly;
 Mono::MonoImage* LoadCSharpLibrary() {
	//Library Path
	auto currentDir = fs::Path::Parent(fs::Path::Current());
	auto libDir = currentDir + "\\test-lib";
	auto monoLib = libDir + "\\mono";
	Mono::MonoManager::SetMonoPath(monoLib);
	Mono::MonoManager::Init("Cobek");
	assembly = Mono::MonoAssembly(Mono::MonoManager::GetDomain());
	assembly.Open(libDir + "\\Designer.dll");
	return assembly.getImage();
}


int main(int argc, char* argv[]) {
	auto asmbly = LoadCSharpLibrary();
	auto klass = asmbly->getClass("App", "designer");
	auto method = klass->getMethod("Run");
	if (method.size() > 0) {
		method[0]->Invoke(nullptr, nullptr);
		while (true) {
			Sleep(1);
		}
		//auto form = klass->getProperty("Form");
		//if (form != nullptr) {
		//	auto FormData = form->getGetMethod()->Invoke(klass, nullptr);
		//	if (!FormData.value)
		//		printf("FormData is NULL");
		//	/*auto winHandler = FormKlass.getProperty("NativeHandle")->getGetMethod()->Invoke(&FormKlass, nullptr).value;
		//	printf("Handler %i", winHandler);*/
		//}
	}
	else {
		
	}
}

