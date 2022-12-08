// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_STRING_VIEW_HXX
#define CORE_UTF8_STRING_VIEW_HXX

#include <cstddef>
#include <string_view>
#include "helpers.hxx"

namespace mangrove::core::utf8
{
	struct StringView
	{
	private:
		std::string_view _data;
		size_t _length;

	public:
		constexpr StringView(const std::string_view &data) noexcept : _data{data}, _length{helpers::countUnits(_data)} { }
		constexpr StringView(const std::string_view &data, const size_t codeUnits) noexcept :
			_data{data}, _length{codeUnits} { }

		constexpr auto data() noexcept { return _data.data(); }
		constexpr const auto *data() const noexcept { return _data.data(); }
		constexpr auto length() const noexcept { return _length; }
		constexpr auto size() const noexcept { return _length; }
		constexpr auto byteLength() const noexcept { return _data.size(); }

		constexpr auto begin() noexcept { return iterator::StringIterator{_data}; }
		constexpr auto begin() const noexcept { return iterator::StringIterator{_data}; }
		constexpr auto end() noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }
		constexpr auto end() const noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }

		constexpr auto rbegin() noexcept { return std::reverse_iterator{end()}; }
		constexpr auto rbegin() const noexcept { return std::reverse_iterator{end()}; }
		constexpr auto rend() noexcept { return std::reverse_iterator{begin()}; }
		constexpr auto rend() const noexcept { return std::reverse_iterator{begin()}; }
	};
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_STRING_VIEW_HXX*/
