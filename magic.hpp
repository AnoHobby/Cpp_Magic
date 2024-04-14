#ifndef INCLUDE_GUARD_MAGIC_HPP
#define INCLUDE_GUARD_MAGIC_HPP

import <string_view>;
import <source_location>;
import <tuple>;
import <functional>;
import <iterator>;
import <array>;
namespace magic {
	template <auto Target>
	consteval auto receieve_this_function_name() {
		std::string_view function_name(std::source_location::current().function_name());
		return function_name;
	}
	template <auto Target>
	consteval auto get_enum_name() {
#define toString(code) #code
		const std::string_view nullptr_string(toString(nullptr));
		const auto nullptr_func = receieve_this_function_name<nullptr>();
		const auto nullptr_pos = nullptr_func.find_last_of(nullptr_string);
		auto target = receieve_this_function_name<Target>();
		target.remove_prefix(nullptr_pos - nullptr_string.size() + 1);
		target.remove_suffix(nullptr_func.size() - nullptr_pos - 1);
		return target;
#undef toString
	}
	template <class F, class... T, std::size_t... I>
	auto visit_tuple(std::tuple<T...>& tuple, const std::size_t& index, const F& function, std::index_sequence<I...>) {
		std::function<void()> functions[] = {
			([&] {function(std::get<I>(tuple)); })...
		};
		functions[index]();
	}
	template <class F, class... T>
	auto visit_tuple(std::tuple<T...>& tuple, const std::size_t index, const F function) {
		visit_tuple(tuple, index, function, std::make_index_sequence<sizeof...(T)>());
	}
	template <class F, class... T>
	auto visit_tuple(std::tuple<T...>&& tuple, const std::size_t index, const F function) {
		visit_tuple(tuple, index, function, std::make_index_sequence<sizeof...(T)>());
	}
	template <class F, class... T>
	auto loop_tuple(std::tuple<T...>& tuple, const F function) {
		std::apply([&](auto&&... t) {
			(function(std::forward<decltype(t)>(t)), ...);
			}, tuple);
	}
	template <class F, class... T>
	auto loop_tuple(std::tuple<T...>&& tuple, const F function) {
		std::apply([&](auto&&... t) {
			(function(std::forward<decltype(t)>(t)), ...);
			}, tuple);
	}
	template <class T>
	inline constexpr auto make_array(auto&&... args) {
		return std::move(std::array<T, sizeof...(args)>{std::forward<decltype(args)>(args)...});
	}
	//inline constexpr auto auto_make_array(auto&&... args) {
	//	return std::move(std::array<, sizeof...(args)>{std::forward<decltype(args)>(args)...});
	//}
};

#endif