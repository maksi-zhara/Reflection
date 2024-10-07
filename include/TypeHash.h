#pragma once 
#include<string_view>
#include<functional>
namespace Reflection 
{
	template<typename T> constexpr std::string_view TypeName();
	template<> constexpr std::string_view TypeName<void>() { return "void"; }
	namespace detail
	{
		using type_name_prober=void;
		template<typename T> constexpr std::string_view wrapped_type_name() 
		{
#ifdef __clang__
			return __PRETTY_FUNCION__;
#elif defined(__GNUC__) 
			return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
			return __FUNCSIG__;
#else 
#error "Unsupported compiler";
#endif
		}
		constexpr size_t get_prefix_length()
		{
			return wrapped_type_name<type_name_prober>().find(TypeName<type_name_prober>());
		}
		constexpr size_t get_suffix_length() 
		{
			return wrapped_type_name<type_name_prober>().length() - get_prefix_length() - TypeName<type_name_prober>().length();
		}
		
	}

	template<typename T> constexpr std::string_view TypeName() 
	{
		std::string_view full_name = detail::wrapped_type_name<T>();
		size_t startPos = detail::get_prefix_length();
		size_t typeLength = detail::wrapped_type_name<T>().length() - detail::get_prefix_length() - detail::get_suffix_length();
		return full_name.substr(startPos, typeLength);
	}
constexpr uint64_t hash(std::string_view str)
	{
		std::uint64_t hash_value = 0xcbf29ce484222325ULL;
		constexpr std::uint64_t prime = 0x100000001b3ULL;
		for (char c : str)
		{
			hash_value ^= static_cast<std::uint64_t>(c);
			hash_value *= prime;
		}
		return hash_value;
	}

	/**
	* TYPE ID
	*/

	template <typename Type>
	constexpr uint64_t TypeId()
	{
		return hash(TypeName<Type>());
	}
}
