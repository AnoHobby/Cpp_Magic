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
	consteval auto getEnumName() {
#define toString(code) #code
		const std::string_view nullptr_string(toString(nullptr));
		const auto func = receieve_this_function_name<nullptr>();
		const auto nullptr_pos = func.find_last_of(nullptr_string);//Calling "find" every time is inefficient
		auto target = receieve_this_function_name<Target>();
		target.remove_prefix(nullptr_pos - nullptr_string.size() + 1);
		target.remove_suffix(func.size() - nullptr_pos - 1);
		return target;
#undef toString
	}
};
#endif