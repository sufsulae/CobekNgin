#if defined(COBEK_ENABLE_3RDPARTY_MONO)
#include "include\plugin\mono\mono.h"
namespace cobek {
	namespace Plugin {
		namespace Mono {
			namespace __internal {
				#include "mono\jit\jit.h"
				#include "mono\metadata\metadata.h"
				#include "mono\metadata\assembly.h"
				#include "mono\metadata\mono-config.h"
				#include "mono\metadata\environment.h"
			}

			//---MonoImg---
			const char* MonoImage::get_Name() {
				if (m_img == nullptr)
					return nullptr;
				return __internal::mono_image_get_name((__internal::MonoImage*)m_img);
			}
			const char* MonoImage::get_Filename() {
				return __internal::mono_image_get_filename((__internal::MonoImage*)m_img);
			}
			const char* MonoImage::get_Guid() {
				return __internal::mono_image_get_guid((__internal::MonoImage*)m_img);
			}
			bool MonoImage::isDynamic() {
				return __internal::mono_image_is_dynamic((__internal::MonoImage*)m_img);
			}

			//---MonoAsm---
			int MonoAssembly::OpenAsm(const char* asmFile) {
				auto newAsm = __internal::mono_domain_assembly_open((__internal::MonoDomain*)m_dom->get_Internal(), asmFile);
				if (newAsm != nullptr) {
					if (m_asm != nullptr)
						CloseAsm();

					auto newImage = __internal::mono_assembly_get_image(newAsm);
					auto newAsmName = __internal::mono_assembly_get_name(newAsm);

					if (newImage != nullptr)
						m_image.m_img = newImage;
					m_asm = newAsm;
					m_asmName = newAsmName;
					return true;
				}
				else {
					printf("Failed to open Assembly: %s", asmFile);
				}
				return false;
			}
			int MonoAssembly::CloseAsm() {
				__internal::mono_image_close((__internal::MonoImage*)m_image.m_img);
				__internal::mono_assembly_name_free((__internal::MonoAssemblyName*)m_asmName);
				__internal::mono_assembly_close((__internal::MonoAssembly*)m_asm);

				m_image.m_img = nullptr;
				return true;
			}
			const char* MonoAssembly::get_Name() {
				if (m_asmName == nullptr)
					return nullptr;
				return __internal::mono_assembly_name_get_name((__internal::MonoAssemblyName*)m_asmName);
			}
			const char* MonoAssembly::get_Culture() {
				if (m_asmName == nullptr)
					return nullptr;
				return __internal::mono_assembly_name_get_culture((__internal::MonoAssemblyName*)m_asmName);
			}

			//---MonoMgr---
			std::vector<MonoDomain> MonoManager::m_domainList = std::vector<MonoDomain>();
			
			void MonoManager::SetAssemblyPath(const char* path) {
				__internal::mono_set_assemblies_path(path);
			}
			void MonoManager::SetDirectory(const char* asmPath, const char* configPath) {
				__internal::mono_config_parse(NULL);
				/*mono_set_dirs(asmPath, configPath);*/
				__internal::mono_set_assemblies_path(asmPath);
			}
			MonoDomain* MonoManager::CreateDomain(const char* domainName, const char* assemblyVersion) {
				for (auto& a : m_domainList) {
					if (domainName == a.m_name)
						return false;
				}
				__internal::MonoDomain* domain = nullptr;
				if (assemblyVersion != nullptr)
					domain = __internal::mono_jit_init_version(domainName, assemblyVersion);
				else
					domain = __internal::mono_jit_init(domainName);
				if (!domain)
				{
					printf("mono_jit_init failed");
					system("pause");
					return nullptr;
				}
				auto dom = MonoDomain();
				dom.m_dom = domain;
				dom.m_name = domainName;
				m_domainList.push_back(dom);
				return &m_domainList[m_domainList.size() - 1];
			}
			MonoDomain* MonoManager::GetDomain(const char* domainName) {
				for (auto& d : m_domainList) {
					if (d.m_name == domainName)
						return &d;
				}
				return nullptr;
			}
			int MonoManager::CloseDomain(MonoDomain* domain) {
				auto len = m_domainList.size();
				for (size_t i = 0; i < len; i++)
				{
					if (&m_domainList[i] == domain) {
						__internal::mono_jit_cleanup((__internal::MonoDomain*)domain->m_dom);
						m_domainList.erase(m_domainList.begin() + i);
						return true;
					}
				}
				return false;
			}
		}
	}
}
#endif
