#ifndef INCLUDE_GUARD_MAGIC_HPP
#define INCLUDE_GUARD_MAGIC_HPP

import <string_view>;
import <source_location>;
import <tuple>;
import <functional>;
import <iterator>;
import <array>;
import <type_traits>;
namespace magic {
	template <auto Target>
	consteval auto receieve_this_function_name() {
		std::string_view function_name(std::source_location::current().function_name());
		return function_name;
	}
	template <auto Target>
	consteval auto get_enum_name() {
#define to_string(code) #code
		const std::string_view nullptr_string(to_string(nullptr));
		const auto nullptr_func = receieve_this_function_name<nullptr>();
		const auto nullptr_pos = nullptr_func.find_last_of(nullptr_string);
		auto target = receieve_this_function_name<Target>();
		target.remove_prefix(nullptr_pos - nullptr_string.size() + 1);
		target.remove_suffix(nullptr_func.size() - nullptr_pos - 1);
		return target;
#undef to_string
	}
	template <auto... I>
	auto visit_tuple(auto&& tuple, const std::size_t& index, const auto&& function, std::index_sequence<I...>) {
		std::function<void()> functions[] = {
			([&] {function(std::get<I>(tuple)); })...
		};
		functions[index]();
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
		return std::move(std::array<decltype(std::initializer_list{std::forward<decltype(args)>(args)...})::value_type, sizeof...(args) > {std::forward<decltype(args)>(args)...});
	}
};

#endif