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

		auto data() noexcept { return _data.data(); }
		const auto *data() const noexcept { return _data.data(); }
		auto length() const noexcept { return _length; }
		auto size() const noexcept { return _length; }
		auto byteLength() const noexcept { return _data.size(); }

		auto begin() noexcept { return iterator::StringIterator{_data}; }
		const auto begin() const noexcept { return iterator::StringIterator{_data}; }
		auto end() noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }
		const auto end() const noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }

		auto rbegin() noexcept { return std::reverse_iterator{end()}; }
		const auto rbegin() const noexcept { return std::reverse_iterator{end()}; }
		auto rend() noexcept { return std::reverse_iterator{begin()}; }
		const auto rend() const noexcept { return std::reverse_iterator{begin()}; }
	};
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_STRING_VIEW_HXX*/
