#ifndef UTILITY_STRING_H
#define UTILITY_STRING_H

#include <string>
#include <vector>

namespace utility
{
	std::vector<std::string> split(const std::string& str, const std::string& delimiter);

	template <typename T>
	std::string toString(const T& v);

	template <typename T>
	T fromString(const std::string& v);

	template <>
	float fromString(const std::string& v);
}


template <typename T>
std::string utility::toString(const T& v)
{
	return std::to_string(v);
}


#endif // UTILITY_STRING_H
