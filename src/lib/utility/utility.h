#ifndef UTILITY_H
#define UTILITY_H

#include <vector>

namespace utility
{
	template<typename T>
	bool isPermutation(const std::vector<T>& a, const std::vector<T>& b)
	{
		return (
			a.size() == b.size() &&
			std::is_permutation(a.begin(), a.end(), b.begin())
		);
	}
}

#endif // UTILITY_H
