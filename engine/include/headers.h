#pragma once
#ifndef COBEK_HEADERS_H
#define COBEK_HEADERS_H

#if defined(_WIN32) || defined(_WIN64) || defined(__VC32__) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAD_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#define COBEK_PLATFORM_WIN 1
#elif defined(__ANDROID__) || defined(ANDROID)
#include <jni.h>
#include <native_app_glue\android_native_app_glue.h>
#if defined (__GNUC__) && __GNUC__ >= 4 || defined (__SUNPRO_C) && __SUNPRO_C == 0x590
#define COBEK_API(type) __attribute__((visibility("default"))) type
#endif
#define COBEK_PLATFORM_ANDROID 1
#define COBEK_API(type) JNIEXPORT type JNICALL
#elif defined(__linux__) || defined (__unix__) || defined(__APPLE__)
#include <X11.h>
#define COBEK_API(type) type
#define COBEK_PLATFORM_LINUX 1
#if defined(__APPLE__)
#define COBEK_PLATFORM_APPLE 1
#else
#define COBEK_API(type) type
#endif
#elif defined(__EMSCRIPTEN__)
#if defined (__GNUC__) && __GNUC__ >= 4 || defined (__SUNPRO_C) && __SUNPRO_C == 0x590
#define COBEK_API(type) __attribute__((visibility("default"))) type
#endif
#define COBEK_PLATFORM_WEBGL 1
#else
#define COBEK_API(type) type
#define COBEK_PLATFORM_UNKNOWN 1
#endif

#if defined(_DEBUG)
#define COBEK_ENABLE_DEBUG 1
#else
#define COBEK_ENABLE_DEBUG 0
#endif

#define COBEK_ENABLE_LOG 0
#define UNICODE 1

//include graphic
#if COBEK_PLATFORM_ANDROID || COBEK_PLATFORM_WEBGL || COBEK_ENABLE_WINGLES || COBEK_PLATFORM_WIN
#ifdef COBEK_INCLUDE_GRAPHIC_GL
#define COBEK_GL 1
#include "glad\glad_gl.h"
#pragma comment (lib, "OpenGL32.lib")
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_GLES
#define COBEK_GLES 4
#define GL_GLEXT_PROTOTYPES 1
#include "glad\glad_egl.h"
#include "glad\glad_gles.h"
#pragma comment (lib, "libEGL.lib")
#pragma comment (lib, "libGLESv2.lib")
#endif
#endif

#if COBEK_PLATFORM_WIN
#ifdef COBEK_INCLUDE_GRAPHIC_DX9
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#define COBEK_DX9 1
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_DX11
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#define COBEK_DX11 2
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_DX12
#include <d3d12.h>
#pragma comment (lib, "d3d12.lib")
#define COBEK_DX12 3
#endif
#endif

#if COBEK_INCLUDE_GRAPHIC_VULKAN
#define COBEK_VULKAN
#endif

//include std library
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

namespace cobek {
#define byte unsigned char
#define ulong unsigned long
#define uint unsigned int
#define ullong unsigned long long
#define llong long long
typedef size_t id_t;
#define SAFE_DELETE(var) if(var != NULL) delete var
#define SAFE_DELETE_ARR(var) if(var != NULL) delete[] var;
}
#endif

