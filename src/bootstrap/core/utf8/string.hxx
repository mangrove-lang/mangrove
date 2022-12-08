// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_STRING_HXX
#define CORE_UTF8_STRING_HXX

#include "char.hxx"
#include "iterator.hxx"
#include "helpers.hxx"
#include "stringView.hxx"

namespace mangrove::core::utf8
{
	struct String
	{
	private:
		std::string _data{};
		size_t _length{};

		void copyChar(const Char &chr, const size_t offset) noexcept
		{
			const size_t length{chr.length()};
			const uint32_t codePoint{chr.toCodePoint()};
			if (length == 1)
				_data[offset] = codePoint;
			else if (length == 2)
			{
				_data[offset] = (codePoint >> 6U) | 0xc0U;
				_data[offset + 1] = (codePoint & 0x3fU) | 0x80U;
			}
			else if (length == 3)
			{
				_data[offset] = (codePoint >> 12U) | 0xe0U;
				_data[offset + 1] = ((codePoint >> 6U) & 0x3fU) | 0x80U;
				_data[offset + 2] = (codePoint & 0x3fU) | 0x80U;
			}
			else
			{
				_data[offset] = (codePoint >> 18U) | 0xf0U;
				_data[offset + 1] = ((codePoint >> 12U) & 0x3fU) | 0x80U;
				_data[offset + 2] = ((codePoint >> 6U) & 0x3fU) | 0x80U;
				_data[offset + 3] = (codePoint & 0x3fU) | 0x80U;
			}
		}

	public:
		constexpr String() noexcept = default;
		String(const std::string_view &string) : _data{string}, _length{helpers::countUnits(_data)} { }

		auto data() noexcept { return _data.data(); }
		const auto *data() const noexcept { return _data.data(); }
		auto length() const noexcept { return _length; }
		auto size() const noexcept { return _length; }
		auto byteLength() const noexcept { return _data.size(); }

		auto begin() noexcept { return iterator::StringIterator{_data}; }
		auto begin() const noexcept { return iterator::StringIterator{_data}; }
		auto end() noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }
		auto end() const noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }

		auto rbegin() noexcept { return std::reverse_iterator{end()}; }
		auto rbegin() const noexcept { return std::reverse_iterator{end()}; }
		auto rend() noexcept { return std::reverse_iterator{begin()}; }
		auto rend() const noexcept { return std::reverse_iterator{begin()}; }

		String &append(const Char &chr) noexcept
		{
			const auto offset{_data.length()};
			_data.resize(offset + chr.length());
			copyChar(chr, offset);
			++_length;
			return *this;
		}

		String &append(const String &str) noexcept
		{
			_data += str._data;
			_length += str._length;
			return *this;
		}

		StringView substr(size_t offset, size_t count = npos) const noexcept
		{
			std::string_view str{_data};
			size_t begin{};
			// If the starting point is less than the number of characters in the string
			if (offset < length())
			{
				// Figure out the offset for the first character requested
				for (size_t i = 0; begin < byteLength() && i < offset; ++i)
					begin += Char{str.substr(begin)}.length();
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
				end += Char{str.substr(end)}.length();

			// Finally, we have the substring range and the number of UTF-8 characters in it, make the substring
			return {str.substr(begin, end), count};
		}

		String &operator +=(const Char &chr) noexcept
			{ return append(chr); }
		String &operator +=(const String &str) noexcept
			{ return append(str); }

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

		bool operator ==(const String &str) const noexcept
			{ return _length == str._length && _data == str._data; }
		bool operator !=(const String &str) const noexcept
			{ return _length != str._length || _data != str._data; }

		constexpr static size_t npos{SIZE_MAX};
	};
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_STRING_HXX*/
