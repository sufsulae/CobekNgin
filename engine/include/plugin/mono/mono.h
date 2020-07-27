#pragma once
#ifndef COBEK_3RDPARTY_MONO_H
#define COBEK_3RDPARTY_MONO_H
#include "../../headers.h"
#if defined(COBEK_ENABLE_3RDPARTY_MONO)
#include "mono\jit\jit.h"
#include "mono\metadata\metadata.h"
#include "mono\metadata\assembly.h"
#pragma comment (lib, "mono-2.0-sgen.lib")
namespace cobek {
	namespace Plugin {
		namespace Mono {
			class MonoAsm {
				MonoDomain* m_dom = nullptr;
				MonoAssembly* m_asm = nullptr ;
			public:
				MonoAsm(MonoDomain* domain) {
					m_dom = domain;
					m_asm = nullptr;
				}
				MonoAsm(MonoDomain* domain, const char* asmFile) {
					m_dom = domain;
					LoadAsm(asmFile);
				}
				~MonoAsm() {
					m_dom = nullptr;
					m_asm = nullptr;
				}

				int LoadAsm(const char* asmFile);
				MonoAssembly* GetAsm();
				int UnloadAsm();

				MonoImage* GetImage();
			};
			class MonoMgr final {
			private:
				static std::unordered_map<std::string, MonoDomain*> m_domainList;
			public:
				static void SetAssemblyPath(const char* path);
				static MonoDomain* CreateDomain(const char* domainName);
				static MonoDomain* GetDomain(const char* domainName);
				static int CloseDomain(const char* domainName);
			};
		}
	}
}
#endif
#endif