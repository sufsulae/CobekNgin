#ifndef UNICODE
#define UNICODE
#endif
#include "include\common.h"
#include "include\time.h"
#include "include\file.h"
#include "include\plugin\mono\mono.h"

using namespace cobek;
namespace Mono = ::Plugin::Mono;
namespace fs = ::FileSystem;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
//	MonoMgr::CreateDomain("Test");
//	printf("Stop Here");
//	return 0;
//}

float testFunc() {
	printf("Testing!!!");
	return 10;
}

int main(int argc, char* argv[]) {
	auto delegate = Delegate<float()>();
	delegate += testFunc;
	auto num = std::vector<float>();
	delegate.invoke(&num);
}

