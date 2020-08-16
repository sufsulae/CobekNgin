#if defined(COBEK_ENABLE_3RDPARTY_MONO)
#include "mono\jit\jit.h"
#include "mono\metadata\metadata.h"
#include "mono\metadata\assembly.h"
#include "mono\metadata\tokentype.h"
#include "mono\metadata\attrdefs.h"
#include "mono\metadata\mono-config.h"
#include "mono\metadata\environment.h"
#include "include\plugin\mono\mono_impl.h"
#include "include\utility\inline.h"
namespace cobek {
	namespace Plugin {
		namespace Mono {
			namespace __internal {
				void _monoBase::_free() {
					mono_free(m_handler);
				}
			}
			namespace __impl {
				//---Field---
				std::string Field::get_name() {
					return m_name;
				}
				MonoMemberVisibility Field::get_visibility() {
					if (m_visibility == MonoMemberVisibility::Unknown) {
						auto flags = mono_field_get_flags((MonoClassField*)m_handler) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;
						if (flags == MONO_FIELD_ATTR_PRIVATE)
							m_visibility = MonoMemberVisibility::Private;
						else if (flags == MONO_FIELD_ATTR_FAM_AND_ASSEM)
							m_visibility = MonoMemberVisibility::ProtectedInternal;
						else if (flags == MONO_FIELD_ATTR_ASSEMBLY)
							m_visibility = MonoMemberVisibility::Internal;
						else if (flags == MONO_FIELD_ATTR_FAMILY)
							m_visibility = MonoMemberVisibility::Protected;
						else if (flags == MONO_FIELD_ATTR_PUBLIC)
							m_visibility = MonoMemberVisibility::Public;
					}
					return m_visibility;
				}
				bool Field::get_isStatic() {
					return (mono_field_get_flags((MonoClassField*)m_handler) & MONO_FIELD_ATTR_STATIC) != 0;
				}
				void Field::set_value(Object& obj) {
					mono_field_set_value((MonoObject*)obj.get_internalHandler(), (MonoClassField*)m_handler, obj.value);
				}
				void* Field::get_value(Class* klass) {
					void* value = nullptr;
					mono_field_get_value((MonoObject*)klass->get_internalHandler(), (MonoClassField*)m_handler, &value);
					return value;
				}
				Class* Field::get_class() {
					return m_class;
				}
				Type Field::get_type() {
					mono_field_get_type((MonoClassField*)m_handler);
				}

				//---Method---
				Object Method::Invoke(Class* obj, void** Params) {
					MonoObject* exc = nullptr;
					void* instance = nullptr;
					if (obj != nullptr) {
						instance = obj->get_internalHandler();
					}
					auto retVal = mono_runtime_invoke((MonoMethod*)m_handler, instance, Params, &exc);
					if (exc != nullptr) {
						Utility::ThrowException(exc);
					}
					auto newObj = Object();
					newObj.m_handler = retVal;
					return newObj;
				}
				std::vector<Type*> Method::get_params() {
					if (m_hasCachedParams) {
						auto res = std::vector<Type*>(m_paramCount);
						for (byte i = 0; i < m_paramCount; i++) {
							res[i] = &m_cachedParams[i];
						}
						return res;
					}
					else {
						m_paramCount = mono_signature_get_param_count((MonoMethodSignature*)m_signature);
						auto res = std::vector<Type*>(m_paramCount);
						if (m_paramCount > 0) {
							m_cachedParams = std::vector<Type>(m_paramCount);
							void* it = nullptr;
							auto param = mono_signature_get_params((MonoMethodSignature*)m_signature, &it);
							while (param != nullptr) {
								auto paramType = Type();
								paramType.m_handler = param;
								m_cachedParams.push_back(paramType);
								res.push_back(&m_cachedParams[m_cachedParams.size() - 1]);
								param = mono_signature_get_params((MonoMethodSignature*)m_signature, &it);
							}
							m_hasCachedParams = true;
						}
						return res;
					}
				}
				MonoMemberVisibility Method::get_visibility() {
					if (m_visibility == MonoMemberVisibility::Unknown) {
						auto flags = mono_method_get_flags((MonoMethod*)m_handler, NULL) & MONO_METHOD_ATTR_ACCESS_MASK;
						if (flags == MONO_METHOD_ATTR_PRIVATE)
							m_visibility = MonoMemberVisibility::Private;
						else if (flags == MONO_METHOD_ATTR_FAM_AND_ASSEM)
							m_visibility = MonoMemberVisibility::ProtectedInternal;
						else if (flags == MONO_METHOD_ATTR_ASSEM)
							m_visibility = MonoMemberVisibility::Internal;
						else if (flags == MONO_METHOD_ATTR_FAMILY)
							m_visibility = MonoMemberVisibility::Protected;
						else if (flags == MONO_METHOD_ATTR_PUBLIC)
							m_visibility = MonoMemberVisibility::Public;
					}
					return m_visibility;
				}
				bool Method::get_isStatic() {
					return (mono_method_get_flags((MonoMethod*)m_handler,NULL) & MONO_METHOD_ATTR_STATIC) != 0;
				}
				bool Method::get_isVirtual() {
					return (mono_method_get_flags((MonoMethod*)m_handler, NULL) & MONO_METHOD_ATTR_VIRTUAL) != 0;
				}
				std::string Method::get_name() {
					if (m_name.empty())
						m_name = mono_method_get_name((MonoMethod*)m_handler);
					return m_name;
				}
				Class* Method::get_class() {
					return m_class;
				}
				Type* Method::get_retType() {
					if (m_cachedRetVal.m_handler = nullptr) {
						m_cachedRetVal.m_handler = mono_signature_get_return_type((MonoMethodSignature*)m_signature);
					}
					return &m_cachedRetVal;
				}

				//---Property---
				std::string Property::get_name() {
					if (m_name.empty()){
						m_name = mono_property_get_name((MonoProperty*)m_handler);
					}
					return m_name;
				}
				Method* Property::get_getMethod() {
					if (m_getMethod.m_handler == nullptr) {
						m_getMethod.m_handler = mono_property_get_get_method((MonoProperty*)m_handler);
					}
					return &m_getMethod;
				}
				Method* Property::get_setMethod() {
					if (m_setMethod.m_handler == nullptr) {
						m_setMethod.m_handler = mono_property_get_set_method((MonoProperty*)m_handler);
					}
					return &m_setMethod;
				}
				Class* Property::get_class() {
					return m_class;
				}

				//---Class---
				std::string Class::get_name() {
					return m_name;
				}
				std::string Class::get_namespace() {
					return m_namespace;
				}
				Field* Class::get_field(std::string name) {
					if (name.empty())
						return nullptr;
					auto fields = get_fields();
					auto fieldsLen = fields.size();
					if (fieldsLen > 0) {
						for (size_t i = 0; i < fieldsLen; i++) {
							if (fields[i]->get_name() == name)
								return fields[i];
						}
					}
					return nullptr;
				}
				Property* Class::get_property(std::string name) {
					if (name.empty())
						return nullptr;
					auto properties = get_properties();
					auto propertiesLen = properties.size();
					if (propertiesLen > 0) {
						for (size_t i = 0; i < propertiesLen; i++) {
							if (properties[i]->get_name() == name)
								return properties[i];
						}
					}
					return nullptr;
				}
				std::vector<Field*> Class::get_fields() {
					if (!m_hasCachedFields) {
						m_cachedFields.clear();
						m_hasCachedFields = false;
						void* iter = nullptr;
						auto field = mono_class_get_fields((MonoClass*)m_handler, &iter);
						while (field != nullptr) {
							auto newField = Field();
							newField.m_handler = field;
							newField.m_class = this;
							m_cachedFields.push_back(newField);
							field = mono_class_get_fields((MonoClass*)m_handler, &iter);
						}
						m_hasCachedFields = true;
					}
					auto len = m_cachedFields.size();
					auto res = std::vector<Field*>(len);
					for (size_t i = 0; i < len; i++) {
						res[i] = &m_cachedFields[i];
					}
					return res;
				}
				std::vector<Property*> Class::get_properties() {
					if (!m_hasCachedProperties) {
						m_cachedProperties.clear();
						void* iter = nullptr;
						auto prop = mono_class_get_properties((MonoClass*)m_handler, &iter);
						while (prop != nullptr) {
							auto newProp = Property();
							newProp.m_handler = prop;
							newProp.m_class = this;
							m_cachedProperties.push_back(newProp);
							prop = mono_class_get_properties((MonoClass*)m_handler, &iter);
						}
						m_hasCachedProperties = true;
					}
					auto len = m_cachedProperties.size();
					auto res = std::vector<Property*>(len);
					for (size_t i = 0; i < len; i++) {
						res[i] = &m_cachedProperties[i];
					}
					return res;
				}
				std::vector<Method*> Class::get_method(std::string name) {
					if (!m_hasCachedMethods) {
						m_cachedMethods.clear();
						void* iteration = nullptr;
						auto method = mono_class_get_methods((MonoClass*)m_handler, &iteration);
						while (method != nullptr) {
							auto methodSignature = mono_method_signature(method);
							auto newMethod = Method();
							newMethod.m_signature = methodSignature;
							newMethod.m_handler = method;
							m_cachedMethods.push_back(newMethod);

							method = mono_class_get_methods((MonoClass*)m_handler, &iteration);
						}
						m_hasCachedMethods = true;
					}
					if (name.empty()) {
						auto ret = std::vector<Method*>(m_cachedMethods.size());
						for (auto& i : m_cachedMethods) {
							ret.push_back(&i);
						}
						return ret;
					}
					else {
						auto ret = std::vector<Method*>();
						for (auto& i : m_cachedMethods) {
							if (i.m_name == name)
								ret.push_back(&i);
						}
						return ret;
					}
				}
				Type* Class::get_type() {
					if(m_type.m_handler == nullptr)
						m_type.m_handler = mono_class_get_type((MonoClass*)m_handler);
					return &m_type;
				}
				void Class::Init() {
					if (!m_hasInitialized) {
						mono_runtime_object_init((MonoObject*)m_handler);
						m_hasInitialized = true;
					}
				}
				void Class::Init(void** params) {
					if (!m_hasInitialized) {
						Init();
						get_method(".ctor")[0]->Invoke(this, params);
					}
				}
				Class Class::CreateInstance(bool initialize) {
					auto newClass = Class();
					newClass.m_handler = mono_object_new((MonoDomain*)Manager::GetDomain()->get_internalHandler(), (MonoClass*)m_handler);
					if (initialize)
						newClass.Init();
					return newClass;
				}
				Class Class::CreateInstance(void** params) {
					auto newClass = Class();
					newClass.m_handler = mono_object_new((MonoDomain*)Manager::GetDomain()->get_internalHandler(), (MonoClass*)m_handler);
					newClass.Init(params);
					return newClass;
				}

				//---Image---
				std::string Image::get_name() {
					if (m_handler == nullptr)
						return nullptr;
					return mono_image_get_name((MonoImage*)m_handler);
				}
				std::string Image::get_Filename() {
					return mono_image_get_filename((MonoImage*)m_handler);
				}
				std::string Image::get_Guid() {
					return mono_image_get_guid((MonoImage*)m_handler);
				}
				Class* Image::get_class(std::string namespaceName, std::string className) {
					for (auto& i : m_Classes) {
						if (i.m_namespace == namespaceName && i.m_name == className)
							return &i;
					}
					auto table = mono_image_get_table_info((MonoImage*)m_handler, MONO_TABLE_TYPEDEF);
					auto tableRowLen = mono_table_info_get_rows(table);
					for (size_t i = 1; i < tableRowLen; i++)
					{
						uint cols[MONO_TYPEDEF_SIZE];
						mono_metadata_decode_row(table, i, cols, MONO_TYPEDEF_SIZE);
						auto name = mono_metadata_string_heap((MonoImage*)m_handler, cols[MONO_TYPEDEF_NAME]);
						if (className == name) {
							auto klass = mono_class_get((MonoImage*)m_handler, (i + 1) | MONO_TOKEN_TYPE_DEF);
							if (klass != nullptr) {
								auto newKlass = Class();
								newKlass.m_name = className;
								newKlass.m_namespace = namespaceName;
								newKlass.m_handler = klass;
								m_Classes.push_back(newKlass);
								return &m_Classes[m_Classes.size() - 1];
							}
						}
					}
					return nullptr;
				}
				bool Image::is_Dynamic() {
					return mono_image_is_dynamic((MonoImage*)m_handler);
				}

				//---Assembly---
				int Assembly::ExecEntryPoint(int argc, char** argv) {
					auto entry = mono_image_get_entry_point((MonoImage*)m_image.m_handler);
					if (!entry) {
						printf("Entry Point Not Found \n");
						return 0;
					}
						
					auto method = mono_get_method((MonoImage*)m_image.m_handler, entry, nullptr);
					if (method == nullptr) {
						printf("Entry Point Method Not Found \n");
						return 0;
					}
					MonoObject* exc = nullptr;
					auto ret = mono_runtime_invoke(method, nullptr, nullptr, &exc);
					return 0;
					/*return mono_jit_exec((MonoDomain*)m_dom->get_internalHandler(), (MonoAssembly*)m_handler, argc -1, argv + 1);*/
				}
				int Assembly::Open(std::string asmFile) {
					auto domHandler = m_dom->get_internalHandler();
					auto newAsm = mono_domain_assembly_open((MonoDomain*)domHandler, asmFile.c_str());
					if (newAsm != nullptr) {
						if (m_handler != nullptr)
							Close();

						auto newImage = mono_assembly_get_image(newAsm);
						auto newAsmName = mono_assembly_get_name(newAsm);

						if (newImage != nullptr)
							m_image.m_handler = newImage;
						m_handler = newAsm;
						m_asmName = newAsmName;
						char* arg = nullptr;
						return true;
					}
					else {
						printf("Failed to open Assembly: %s", asmFile);
					}
					return false;
				}
				int Assembly::Close() {
					mono_image_close((MonoImage*)m_image.m_handler);
					mono_assembly_name_free((MonoAssemblyName*)m_asmName);
					mono_assembly_close((MonoAssembly*)m_handler);

					m_image.m_handler = nullptr;
					return true;
				}
				std::string Assembly::get_name() {
					if (m_asmName == nullptr)
						return nullptr;
					return mono_assembly_name_get_name((MonoAssemblyName*)m_asmName);
				}
				std::string Assembly::get_Culture() {
					if (m_asmName == nullptr)
						return nullptr;
					return mono_assembly_name_get_culture((MonoAssemblyName*)m_asmName);
				}

				//---String---
				String::String(void* stringHandler) {
					m_handler = stringHandler;
				}
				String::String(const std::string& str) {
					setW(str);
				}
				String::String(const std::wstring& str) {
					setW(str);
				}
				void String::setW(const std::wstring& str) {
					if (sizeof(wchar_t) == 2) // Assuming UTF-16
						m_handler = mono_string_from_utf16((mono_unichar2*)str.c_str());
					else // Assuming UTF-32
						m_handler = mono_string_from_utf32((mono_unichar4*)str.c_str());
				}
				void String::set(const std::string& str) {
					setW(std::wstring(cobek::Utility::mbchar2wchar(str.c_str())));
				}
				std::string String::get() {
					return cobek::Utility::wchar2mbchar(getW().c_str());
				}
				std::wstring String::getW() {
					if (m_handler == nullptr)
						return std::wstring();
					auto handler = (MonoString*)m_handler;
					int len = mono_string_length(handler);
					mono_unichar2* monoChars = mono_string_chars(handler);

					std::wstring ret(len, '0');
					for (int i = 0; i < len; i++)
						ret[i] = monoChars[i];
					return ret;
				}

				//---Manager---
				Domain Manager::m_domain = Domain();
				const char* Manager::CheckCorlibVersion() {
					return mono_check_corlib_version();
				}
				void Manager::SetAssemblyPath(std::string path) {
					mono_set_assemblies_path(path.c_str());
				}
				void Manager::SetDirectory(std::string asmPath) {
					mono_config_parse(NULL);
					mono_set_assemblies_path(asmPath.c_str());
				}
				void Manager::Init(std::string domainName, std::string assemblyVersion) {
					MonoDomain* domain = nullptr;
					if (!assemblyVersion.empty())
						domain = mono_jit_init_version(domainName.c_str(), assemblyVersion.c_str());
					else
						domain = mono_jit_init(domainName.c_str());
					m_domain = Domain();
					m_domain.m_handler = domain;
					m_domain.m_name = domainName;
				}
				Domain* Manager::GetDomain() {
					return &m_domain;
				}
				void Manager::Close() {
					mono_jit_cleanup((MonoDomain*)m_domain.m_handler);
					mono_domain_free((MonoDomain*)m_domain.m_handler, false);
					m_domain = Domain();
				}

				//---Utility---
				void Utility::ThrowException(void* exc) {
					auto obj = (MonoObject*)exc;
					auto exceptionClass = mono_object_get_class(obj);
					auto exceptionMsgProp = mono_class_get_property_from_name(exceptionClass, "Message");
					auto exceptionMsgGetter = mono_property_get_get_method(exceptionMsgProp);
					auto exceptionMsg = (MonoString*)mono_runtime_invoke(exceptionMsgGetter, obj, nullptr, nullptr);

					auto exceptionStackProp = mono_class_get_property_from_name(exceptionClass, "StackTrace");
					auto exceptionStackGetter = mono_property_get_get_method(exceptionStackProp);
					auto exceptionStackTrace = (MonoString*)mono_runtime_invoke(exceptionStackGetter, obj, nullptr, nullptr);

					// Note: If you modify this format make sure to also modify Debug.ParseExceptionMessage in managed code.
					std::string msg = "Managed exception: " + String(exceptionMsg).get() + "\n" + String(exceptionStackTrace).get();
					printf("%s \n", msg.c_str());
				}
			}
		}
	}
}
#endif
