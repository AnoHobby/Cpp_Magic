#ifndef INCLUDE_GUARD_ENUMERATE_HPP
#define INCLUDE_GUARD_ENUMERATE_HPP
import <string_view>;
import <source_location>;
namespace enumerate {
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
};
#endif
