#pragma once
#ifndef COBEK_3RDPARTY_MONO_H
#define COBEK_3RDPARTY_MONO_H
#include "../../headers.h"
#if defined(COBEK_ENABLE_3RDPARTY_MONO)
#pragma comment (lib, "mono-2.0-sgen.lib")
namespace cobek {
	namespace Plugin {
		namespace Mono {
			enum class MonoMemberVisibility
			{
				Unknown = -1,
				Private,
				Protected,
				Internal,
				ProtectedInternal,
				Public
			};

			namespace __internal {
				class _monoBase {
				protected:
					void* m_handler = nullptr;
					void _free();
				public:
					//~_monoBase();
					virtual void* get_internalHandler() { return m_handler; }
					virtual bool isEmpty() { return m_handler == nullptr; }
					virtual void Free() { return _free(); }
				};
			}

			namespace __impl {
				class Class;
				class Domain;

				class Type : public __internal::_monoBase {
				private:
					friend class Field;
					friend class Method;
					friend class Property;
					friend class Class;
					friend class Image;
					friend class Assembly;
					friend class Manager;
				};

				class Object : public __internal::_monoBase {
				private:
					friend class Method;
					friend class Class;
					friend class Property;
					friend class Field;
				public:
					void* value = nullptr;
				};

				class String : public __internal::_monoBase {
				public:
					String(void* stringHandler);
					String(const std::string& str);
					String(const std::wstring& str);
					void set(const std::string& str);
					void setW(const std::wstring& str);
					std::string get();
					std::wstring getW();
				};

				class Field : public __internal::_monoBase {
				private:
					std::string m_name = std::string();
					Type m_type = Type();
					Class* m_class = nullptr;
					MonoMemberVisibility m_visibility = MonoMemberVisibility::Unknown;
					friend class Class;
				public:
					
					std::string get_name();
					MonoMemberVisibility get_visibility();
					void set_value(Object& obj);
					void set_valueDirect(void* obj);
					bool get_isStatic();
					void* get_value(Class* klass);
					Class* get_class();
					Type* get_type();
				};

				class Method : public __internal::_monoBase {
				private:
					void* m_signature = nullptr;
					std::string m_name = std::string();
					std::vector<Type> m_cachedParams = std::vector<Type>();
					Type m_cachedRetVal = Type();
					MonoMemberVisibility m_visibility = MonoMemberVisibility::Unknown;
					Class* m_class = nullptr;

					byte m_paramCount;
					bool m_hasCachedParams;
					friend class Property;
					friend class Class;
				public:
					std::string get_name();
					std::vector<Type*> get_params();
					MonoMemberVisibility get_visibility();
					bool get_isStatic();
					bool get_isVirtual();
					Type* get_retType();
					Class* get_class();
					Object Invoke(Class* obj, void** Params);
				};

				class Property : public __internal::_monoBase {
				private:
					std::string m_name = std::string();
					MonoMemberVisibility m_visibility = MonoMemberVisibility::Unknown;
					Method m_getMethod = Method();
					Method m_setMethod = Method();
					Class* m_class;
					friend class Class;
				public:
					std::string get_name();
					Method* get_getMethod();
					Method* get_setMethod();
					Class* get_class();
				};

				class Class : public __internal::_monoBase {
				private:
					std::string m_name = std::string();
					std::string m_namespace = std::string();
					MonoMemberVisibility m_visibilty = MonoMemberVisibility::Unknown;
					Type m_type = Type();
					std::vector<Field> m_cachedFields = std::vector<Field>();
					std::vector<Method> m_cachedMethods = std::vector<Method>();
					std::vector<Property> m_cachedProperties = std::vector<Property>();
					bool m_hasCachedMethods = false;
					bool m_hasCachedFields = false;
					bool m_hasCachedProperties = false;
					bool m_hasInitialized = false;
					friend class Image;
					friend class Manager;
				public:
					std::string get_name();
					std::string get_namespace();
					Field* get_field(std::string name);
					Property* get_property(std::string name);
					std::vector<Field*> get_fields();
					std::vector<Property*> get_properties();
					std::vector<Method*> get_method(std::string name = std::string());
					Type* get_type();

					void Init();
					void Init(void** params);
					Class CreateInstance(bool initialize = false);
					Class CreateInstance(void** params);
				};

				class Image : public __internal::_monoBase {
				private:
					std::vector<Class> m_Classes;
					friend class Manager;
					friend class Assembly;
				public:
					std::string get_name();
					std::string get_Filename();
					std::string get_Guid();
					Class* get_class(std::string namespaceName, std::string name);
					bool is_Dynamic();
				};

				class Assembly : public __internal::_monoBase {
				private:
					Domain* m_dom = nullptr;
					Image m_image;
					void* m_asmName = nullptr;
				public:
					Assembly(Domain* domain) {
						m_dom = domain;
						m_handler = nullptr;
					}
					Assembly(Domain* domain, std::string asmFile) {
						m_dom = domain;
						Open(asmFile);
					}
					~Assembly() {
						m_dom = nullptr;
						m_handler = nullptr;
					}

					bool is_Loaded() { return m_handler != nullptr; }

					Image* get_Image() { return &m_image; }
					std::string get_name();
					std::string get_Culture();

					int ExecEntryPoint(int argc, char** argv);
					int Open(std::string asmFile);
					int Close();
				};

				class Domain : public __internal::_monoBase {
				private:
					std::string m_name = std::string();
					friend class Manager;
				};

				class Manager final {
				private:
					static Domain m_domain;
				public:
					static const char* CheckCorlibVersion();
					static void SetAssemblyPath(std::string path);
					static void SetDirectory(std::string asmPath);
					static void Init(std::string domainName, std::string assemblyVersion = std::string());
					static Domain* GetDomain();
					static void Close();
				};

				class Utility final {
				public: 
					static void ThrowException(void* exception);
				};
			}
		}
	}
}
#endif
#endif