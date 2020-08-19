#pragma once
#ifndef COBEK_WINDOW_H
#define COBEK_WINDOW_H

#include "include\common.h"

#ifndef COBEK_GRAPHIC_H
#include "include\graphic.h"
#endif

namespace cobek {
	namespace Window {
		struct WinPtr {
		public:
		#if defined(COBEK_PLATFORM_WIN)
			HDC     DeviceContext;
			HBITMAP PixelMap;
			HWND    Handle;
		#elif defined(COBEK_PLATFORM_WEBGL)
			int     DeviceContext;
			int		PixelMap;
			int		Handle;
		#elif defined(COBEK_PLATFORM_ANDROID)
			void*                DeviceContext;
			egl_native_pixmap_t* PixelMap;
			ANativeWindow*       Handle;
		//Linux With X11 Enabled
		#elif defined(COBEK_PLATFORM_LINUX) || defined(COBEK_PLATFORM_APPLE)
			Display*	DeviceContext;
			Pixmap		PixelMap;
			Window		Handle;
		#else
			void* DeviceContext;
			void* PixelMap;
			void* Handle;
		#endif
		};
		class WindowMgr final {
		private:
			static std::unordered_map<void*, WinPtr*> m_registeredCtx;
		public:
			static void* InitilizeGraphicWindow(WinPtr* winPtr, Graphic::GraphicBackend backend);
			static WinPtr* GetGraphicWindow(void* graphicHandler);
			static void ReleaseGraphicWindow(void* graphicHandler);
			static void SwapBufferGraphicWindow(void* graphicHandler);
		};
	}
}

#endif