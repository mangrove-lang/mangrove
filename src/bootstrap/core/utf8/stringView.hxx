// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_STRING_VIEW_HXX
#define CORE_UTF8_STRING_VIEW_HXX

#include <cstddef>
#include <string_view>

namespace mangrove::core::utf8
{
	struct StringView
	{
	private:
		std::string_view _data;
		size_t _length;

	public:
		constexpr StringView(std::string_view data, size_t codeUnits) noexcept : _data{data}, _length{codeUnits} { }
	};
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_STRING_VIEW_HXX*/
