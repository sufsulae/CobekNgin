#ifndef UNICODE
#define UNICODE
#endif
#include "include\engine.h"

using namespace cobek::Plugin::Mono;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
//	MonoMgr::CreateDomain("Test");
//	printf("Stop Here");
//	return 0;
//}

int main(int argc, char* argv[]) {
	MonoMgr::CreateDomain("Test");
	printf("Stop Here");
	return 0;
}