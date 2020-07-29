#pragma once
#ifndef COBEK_3RDPARTY_MONO_H
#define COBEK_3RDPARTY_MONO_H
#include "../../headers.h"
#if defined(COBEK_ENABLE_3RDPARTY_MONO)
#pragma comment (lib, "mono-2.0-sgen.lib")
namespace cobek {
	namespace Plugin {
		namespace Mono {
			class MonoImage {
			private:
				void* m_img = nullptr;
				friend class MonoManager;
				friend class MonoAssembly;
			public:
				void* get_Internal() { return m_img; }
				const char* get_Name();
				const char* get_Filename();
				const char* get_Guid();
				bool isDynamic();
			};

			class MonoDomain {
			private:
				const char* m_name = nullptr;
				void* m_dom = nullptr;
				friend class MonoManager;
			public:
				void* get_Internal() { return m_dom; }
			};

			class MonoAssembly {
			private:
				MonoDomain* m_dom;
				MonoImage m_image;
				void* m_asm = nullptr;
				void* m_asmName = nullptr;
			public:
				MonoAssembly(MonoDomain* domain) {
					m_dom = domain;
					m_asm = nullptr;
				}
				MonoAssembly(MonoDomain* domain, const char* asmFile) {
					m_dom = domain;
					OpenAsm(asmFile);
				}
				~MonoAssembly() {
					m_dom = nullptr;
					m_asm = nullptr;
				}

				bool isLoaded() { return m_asm != nullptr; }

				void* get_Internal() { return m_dom; }
				MonoImage* get_Image() { return &m_image; }
				const char* get_Name();
				const char* get_Culture();

				int OpenAsm(const char* asmFile);
				int CloseAsm();
			};

			class MonoManager final {
			private:
				static std::vector<MonoDomain> m_domainList;
			public:
				static void SetAssemblyPath(const char* path);
				static void SetDirectory(const char* asmPath, const char* configPath);
				static MonoDomain* CreateDomain(const char* domainName, const char* assemblyVersion = nullptr);
				static MonoDomain* GetDomain(const char* domainName);
				static int CloseDomain(MonoDomain* domain);
			};
		}
	}
}
#endif
#endif