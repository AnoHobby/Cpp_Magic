#ifndef INCLUDE_GUARD_MAGIC_HPP
#define INCLUDE_GUARD_MAGIC_HPP

#if defined(__INTELLISENSE__)
#include <string_view>;
#include <source_location>;
#include <tuple>;
#include <iterator>;
#include <array>;
#include <type_traits>;

#else
import <string_view>;
import <source_location>;
import <tuple>;
import <iterator>;
import <array>;
import <type_traits>;
#endif

import <string_view>;
import <source_location>;
import <tuple>;
import <iterator>;
import <array>;
import <type_traits>;
namespace magic {
	template <class T,int Size>
	class String {
	private:
		constexpr auto init(const auto* str) {
			for (int i = 0; i < Size; i++) buffer[i] = str[i];
			buffer[Size] = '\0';
		}
	public:
		T buffer[Size + 1];
		constexpr String(const T(&str)[Size]){
			init(str);
		}
		template <class Pointer>requires std::is_pointer<Pointer>::value
		constexpr String(const Pointer str) {
			init(str);
		}
	};
	template <auto Target>
	consteval auto receieve_this_function_name() {
		std::string_view function_name(std::source_location::current().function_name());
		return function_name;
	}
	template <class Target>
	consteval auto receieve_this_function_name() {
		std::string_view function_name(std::source_location::current().function_name());
		return function_name;
	}
	template <auto Target>
	consteval auto get_value_name() {
#define TO_STRING(code) #code
		constexpr std::string_view nullptr_string(TO_STRING(nullptr));
#undef TO_STRING
		constexpr auto nullptr_func = receieve_this_function_name<nullptr>();
		constexpr auto nullptr_pos = nullptr_func.find_last_of(nullptr_string);
		auto target = receieve_this_function_name<Target>();
		target.remove_prefix(nullptr_pos - nullptr_string.size() + 1);
		target.remove_suffix(nullptr_func.size() - nullptr_pos - 1);
		return target;
	}
	template <auto Enum_Value>
	consteval auto get_enum_value_name() {
		auto name = get_value_name<Enum_Value>();
		const auto scope=name.find_last_of(":");
		if (scope != std::string_view::npos) {
			name.remove_prefix(scope+1);//for msvc.this is uncool.
		}
		return name;
	}
	template <auto... I>
	auto visit_tuple(auto&& tuple, const std::size_t& index, const auto&& function, std::index_sequence<I...>) {
		void (*functions[])(decltype(function), decltype(tuple)) = {
			([](const auto&& function,auto&& tuple) {function(std::get<I>(tuple)); })...
		};
		functions[index](std::forward<decltype(function)>(function), std::forward<decltype(tuple)>(tuple));
	}
	auto visit_tuple(auto&& tuple, const std::size_t index, const auto&& function) {
		visit_tuple(std::forward<decltype(tuple)>(tuple), index, std::forward<decltype(function)>(function), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<decltype(tuple)> > >());
	}
	auto loop_tuple(auto&& tuple, const auto&& function) {
		std::apply([&](auto&&... t) {
			(function(std::forward<decltype(t)>(t)), ...);
			}, tuple);
	}
	template <class T>
	inline constexpr auto make_array(auto&&... args) {
		return std::move(std::array<T, sizeof...(args)>{std::forward<decltype(args)>(args)...});
	}
	inline constexpr auto auto_make_array(auto&&... args) {
		return std::move(std::array<decltype(std::initializer_list{ std::forward<decltype(args)>(args)... })::value_type, sizeof...(args) > {std::forward<decltype(args)>(args)...});
	}
	template <class... T>
	struct Overload :T...{
		Overload(T...) {}
		using T::operator()...;
	};
};

#endif