// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_STRING_VIEW_HXX
#define CORE_UTF8_STRING_VIEW_HXX

#include <cstddef>
#include <string_view>
#include <fmt/format.h>
#include <substrate/index_sequence>
#include "helpers.hxx"
#include "iterator.hxx"

namespace mangrove::core::utf8
{
	struct StringView final
	{
	private:
		std::string_view _data;
		size_t _length;

	public:
		constexpr StringView(const std::string_view &data) noexcept : _data{data}, _length{helpers::countUnits(_data)} { }
		constexpr StringView(const std::string_view &data, const size_t codeUnits) noexcept :
			_data{data}, _length{codeUnits} { }

		[[nodiscard]] constexpr auto data() noexcept { return _data.data(); }
		[[nodiscard]] constexpr const auto *data() const noexcept { return _data.data(); }
		[[nodiscard]] constexpr auto length() const noexcept { return _length; }
		[[nodiscard]] constexpr auto size() const noexcept { return _length; }
		[[nodiscard]] constexpr auto byteLength() const noexcept { return _data.size(); }
		[[nodiscard]] auto isEmpty() const noexcept { return _length == 0; }

		[[nodiscard]] constexpr auto begin() noexcept { return iterator::StringIterator{_data}; }
		[[nodiscard]] constexpr auto begin() const noexcept { return iterator::StringIterator{_data}; }
		[[nodiscard]] constexpr auto end() noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }
		[[nodiscard]] constexpr auto end() const noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }

		[[nodiscard]] constexpr auto rbegin() noexcept { return std::reverse_iterator{end()}; }
		[[nodiscard]] constexpr auto rbegin() const noexcept { return std::reverse_iterator{end()}; }
		[[nodiscard]] constexpr auto rend() noexcept { return std::reverse_iterator{begin()}; }
		[[nodiscard]] constexpr auto rend() const noexcept { return std::reverse_iterator{begin()}; }

		[[nodiscard]] constexpr StringView substr(size_t offset, size_t count = npos) const noexcept
		{
			size_t begin{};
			// If the starting point is less than the number of characters in the string
			if (offset < length())
			{
				// Figure out the offset for the first character requested
				for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{offset})
					begin += Char{_data.substr(begin)}.length();
			}
			else
			{
				// Otherwise it's just the end of this string
				offset = length();
				begin = byteLength();
			}

			// Bring count into range
			if (count > length() - offset)
				count = length() - offset;

			auto end{begin};
			// Now we have [begin, ) but need to figure out the end offset - so go forward count characters
			for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{count})
				end += Char{_data.substr(end)}.length();

			// Finally, we have the substring range and the number of UTF-8 characters in it, make the substring
			return {_data.substr(begin, end), count};
		}

		[[nodiscard]] bool beginsWith(const StringView &str) const noexcept
		{
			// Start by making sure the string to compare is not longer than this one
			if (str.length() > length() || str.byteLength() > byteLength())
				return false;
			// Then extract the raw data begin/end
			const auto *const begin{str.data()};
			const auto *const end{begin + str.byteLength()};
			// And bytewise compare for equality
			return std::equal(begin, end, _data.cbegin());
		}

		[[nodiscard]] bool endsWith(const StringView &str) const noexcept
		{
			// Start by making sure the string to compare is not longer than this one
			if (str.length() > length() || str.byteLength() > byteLength())
				return false;
			// Then extract the raw data begin/end
			const auto *const begin{str.data()};
			const auto *const end{begin + str.byteLength()};
			// And bytewise compare for equality, rewinding from the end of this string
			return std::equal(begin, end, _data.cend() - str.byteLength());
		}

		[[nodiscard]] bool startsWith(const StringView &str) const noexcept
			{ return beginsWith(str); }

		bool operator ==(const StringView &str) const noexcept
			{ return _length == str._length && _data == str._data; }
		bool operator !=(const StringView &str) const noexcept
			{ return _length != str._length || _data != str._data; }

		bool operator <(const StringView &str) const noexcept
		{
			if (_length != str._length)
				return _length < str._length;
			return _data.compare(str._data) < 0;
		}

		constexpr static size_t npos{SIZE_MAX};
	};

	inline namespace literals
	{
		constexpr inline StringView operator ""_sv(const char *const value, const size_t length) noexcept
			{ return StringView{std::string_view{value, length}}; }
	} // namespace literals
} // namespace mangrove::core::utf8

template<> struct fmt::formatter<mangrove::core::utf8::StringView> : formatter<std::string_view>
{
	using StringView = mangrove::core::utf8::StringView;

	template<typename FormatContext> auto format(const StringView &str, FormatContext &ctx) const
	{
		const std::string_view data{str.data(), str.byteLength()};
		return formatter<std::string_view>::format(data, ctx);
	}
};

#endif /*CORE_UTF8_STRING_VIEW_HXX*/
