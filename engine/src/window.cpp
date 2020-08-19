#include "include\window.h"
#include "include\debug.h"
#include "include\utility\sys.h"

using namespace cobek::Graphic;
using namespace cobek::Utility;

namespace cobek {
	namespace Window {
		std::unordered_map<void*, WinPtr*> WindowMgr::m_registeredCtx = std::unordered_map<void*, WinPtr*>();
		void* WindowMgr::InitilizeGraphicWindow(WinPtr* winPtr, GraphicBackend backend) {
			//For Now We only support OPENGL
			bool registered = false;
			for (auto& element : m_registeredCtx) {
				if (element.second->Handle == winPtr->Handle &&
					element.second->DeviceContext == winPtr->DeviceContext) {
					registered = true;
					break;
				}
			}
			if (registered || backend != GraphicBackend::OPENGL || winPtr->Handle == nullptr || winPtr->DeviceContext == nullptr)
				return nullptr;
#if defined(COBEK_PLATFORM_WIN)
			switch (backend) {
			case GraphicBackend::OPENGL:
				//For now we hardcoded the parameter
				//Taken Directly from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-choosepixelformat
				PIXELFORMATDESCRIPTOR pixDesc = {
					sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
					1,                     // version number  
					PFD_DRAW_TO_WINDOW |   // support window  
					PFD_SUPPORT_OPENGL |   // support OpenGL  
					PFD_DOUBLEBUFFER,      // double buffered  
					PFD_TYPE_RGBA,         // RGBA type  
					24,                    // 24-bit color depth  
					0, 0, 0, 0, 0, 0,      // color bits ignored
					8,                     // no alpha buffer  
					0,                     // shift bit ignored  
					0,                     // no accumulation buffer  
					0, 0, 0, 0,            // accum bits ignored  
					32,                    // 32-bit z-buffer      
					0,                     // no stencil buffer  
					0,                     // no auxiliary buffer  
					PFD_MAIN_PLANE,        // main layer  
					0,                     // reserved  
					0, 0, 0                // layer masks ignored  
				};
				auto pixFmt = ChoosePixelFormat(winPtr->DeviceContext, &pixDesc);
				SetPixelFormat(winPtr->DeviceContext, pixFmt, &pixDesc);
				auto ctx = wglCreateContext(winPtr->DeviceContext);
				wglMakeCurrent(winPtr->DeviceContext, ctx);
				break;
			}
#endif

		}
		WinPtr* WindowMgr::GetGraphicWindow(void* graphicHandler) {
			if (m_registeredCtx.find(graphicHandler) == m_registeredCtx.end())
				return nullptr;
			
		}
		void WindowMgr::ReleaseGraphicWindow(void* graphicHandler) {
			m_registeredCtx.erase(graphicHandler);
		}

		void WindowMgr::SwapBufferGraphicWindow(void* graphicHandler) {
			if (m_registeredCtx.find(graphicHandler) == m_registeredCtx.end())
				return;
		#if defined(COBEK_PLATFORM_WIN)
			SwapBuffers(m_registeredCtx[graphicHandler]->DeviceContext);
		#endif
		}
	}
}