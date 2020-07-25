#if defined(SAMBAL_ENABLE_3RDPARTY_MONO)
#include "include\plugin\mono\mono.h"
namespace sambal {
	namespace Plugin {
		namespace Mono {
			//---MonoAsm---
			int MonoAsm::LoadAsm(const char* asmFile) {
				auto newAsm = mono_domain_assembly_open(m_dom, asmFile);
				if (newAsm != nullptr) {
					if (m_asm != nullptr)
						mono_assembly_close(m_asm);
					m_asm = newAsm;
					return true;
				}
				return false;
			}
			int MonoAsm::UnloadAsm() {
				mono_assembly_close(m_asm);
				return true;
			}

			//---MonoMgr---
			std::unordered_map <std::string, MonoDomain*> MonoMgr::m_domainList = std::unordered_map <std::string, MonoDomain*>();
			
			void MonoMgr::SetAssemblyPath(const char* path) {
				
			}
			MonoDomain* MonoMgr::CreateDomain(const char* domainName) {
				if (m_domainList.find(domainName) != m_domainList.end())
					return false;
				auto domain = mono_jit_init(domainName);
				if (!domain)
				{
					printf("mono_jit_init failed");
					system("pause");
					return nullptr;
				}
				m_domainList[domainName] = domain;
				return domain;
			}
			MonoDomain* MonoMgr::GetDomain(const char* domainName) {
				try {
					return m_domainList.at(domainName);
				}
				catch (const std::out_of_range&) {
					return nullptr;
				}
			}
			int MonoMgr::CloseDomain(const char* domainName) {
				try {
					auto dom = m_domainList.at(domainName);
					mono_jit_cleanup(dom);
					m_domainList.erase(domainName);
					return true;
				}
				catch (const std::out_of_range&) {
					return false;
				}
			}
		}
	}
}
#endif
