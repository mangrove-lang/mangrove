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

		StringView substr(size_t offset, size_t count = npos) const noexcept
		{
			size_t begin{};
			// If the starting point is less than the number of characters in the string
			if (offset < length())
			{
				// Figure out the offset for the first character requested
				for (size_t i = 0; begin < byteLength() && i < offset; ++i)
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
			for (size_t i = 0; i < count; ++i)
				end += Char{_data.substr(end)}.length();

			// Finally, we have the substring range and the number of UTF-8 characters in it, make the substring
			return {_data.substr(begin, end), count};
		}

		bool beginsWith(const StringView &str) const noexcept
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

		bool endsWith(const StringView &str) const noexcept
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

		bool startsWith(const StringView &str) const noexcept
			{ return beginsWith(str); }

		bool operator ==(const StringView &str) const noexcept
			{ return _length == str._length && _data == str._data; }
		bool operator !=(const StringView &str) const noexcept
			{ return _length != str._length || _data != str._data; }

		constexpr static size_t npos{SIZE_MAX};
	};
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_STRING_VIEW_HXX*/
