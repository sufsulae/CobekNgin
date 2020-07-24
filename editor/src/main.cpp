#ifndef UNICODE
#define UNICODE
#endif
#include "include\engine.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	sambal::Vec2F vec = sambal::Vec2F(10, 20);
	vec /= 2.0f;
	auto vecRes = vec;
	printf("Stop Here");
	return 0;
}