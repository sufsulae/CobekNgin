#pragma once
#include "mono_impl.h"

namespace cobek {
	namespace Plugin {
		namespace Mono {
			using MonoType = Mono::__impl::Type;
			using MonoObject = Mono::__impl::Object;
			using MonoString = Mono::__impl::String;
			using MonoField = Mono::__impl::Field;
			using MonoMethod = Mono::__impl::Method;
			using MonoProperty = Mono::__impl::Property;
			using MonoClass = Mono::__impl::Class;
			using MonoAssembly = Mono::__impl::Assembly;
			using MonoDomain = Mono::__impl::Domain;
			using MonoManager = Mono::__impl::Manager;
			using MonoUtil = Mono::__impl::Utility;
		}
	}
}