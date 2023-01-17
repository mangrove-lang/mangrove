// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_STRING_HXX
#define CORE_UTF8_STRING_HXX

#include <stdexcept>
#include <fmt/format.h>
#include "char.hxx"
#include "iterator.hxx"
#include "helpers.hxx"
#include "stringView.hxx"

namespace mangrove::core::utf8
{
	struct String final
	{
	private:
		std::string _data{};
		size_t _length{};

		void copyChar(const Char &chr, const size_t offset) noexcept
		{
			const size_t length{chr.length()};
			const uint32_t codePoint{chr.toCodePoint()};
			if (length == 1)
				_data[offset] = static_cast<char>(codePoint);
			else if (length == 2)
			{
				_data[offset] = static_cast<char>((codePoint >> 6U) | 0xc0U);
				_data[offset + 1] = static_cast<char>((codePoint & 0x3fU) | 0x80U);
			}
			else if (length == 3)
			{
				_data[offset] = static_cast<char>((codePoint >> 12U) | 0xe0U);
				_data[offset + 1] = static_cast<char>(((codePoint >> 6U) & 0x3fU) | 0x80U);
				_data[offset + 2] = static_cast<char>((codePoint & 0x3fU) | 0x80U);
			}
			else
			{
				_data[offset] = static_cast<char>((codePoint >> 18U) | 0xf0U);
				_data[offset + 1] = static_cast<char>(((codePoint >> 12U) & 0x3fU) | 0x80U);
				_data[offset + 2] = static_cast<char>(((codePoint >> 6U) & 0x3fU) | 0x80U);
				_data[offset + 3] = static_cast<char>((codePoint & 0x3fU) | 0x80U);
			}
		}

		template<typename... Chars>
			void copyChars(const size_t offset, const Char &chr, const Chars &... chrs) noexcept
		{
			copyChar(chr, offset);
			if constexpr (sizeof...(Chars) != 0)
				copyChars(offset + chr.length(), chrs...);
		}

	public:
		constexpr String() noexcept = default;
		String(const String &str) = default;
		String(String &&str) noexcept = default;
		String(const std::string_view &string) noexcept : _data{string}, _length{helpers::countUnits(_data)} { }
		String(const StringView &string) noexcept :
			_data{string.data(), string.byteLength()}, _length{string.length()} { }

		template<typename... Chars, typename = std::enable_if_t<(std::is_same_v<Chars, Char> && ...)>>
			String(const Chars &... chrs) noexcept : _data((chrs.length() + ...), '\0'), _length{sizeof...(chrs)}
				{ copyChars(0, chrs...); }

		~String() noexcept = default;
		String &operator =(const String &str) = default;
		String &operator =(String &&str) noexcept = default;

		[[nodiscard]] operator StringView() const noexcept
			{ return {_data, _length}; }

		[[nodiscard]] auto data() noexcept { return _data.data(); }
		[[nodiscard]] const auto *data() const noexcept { return _data.data(); }
		[[nodiscard]] auto length() const noexcept { return _length; }
		[[nodiscard]] auto size() const noexcept { return _length; }
		[[nodiscard]] auto byteLength() const noexcept { return _data.size(); }
		[[nodiscard]] auto isEmpty() const noexcept { return _length == 0; }

		[[nodiscard]] auto begin() noexcept { return iterator::StringIterator{_data}; }
		[[nodiscard]] auto begin() const noexcept { return iterator::StringIterator{_data}; }
		[[nodiscard]] auto end() noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }
		[[nodiscard]] auto end() const noexcept { return iterator::StringIterator{_data, iterator::StartAtEnd{}}; }

		[[nodiscard]] auto rbegin() noexcept { return std::reverse_iterator{end()}; }
		[[nodiscard]] auto rbegin() const noexcept { return std::reverse_iterator{end()}; }
		[[nodiscard]] auto rend() noexcept { return std::reverse_iterator{begin()}; }
		[[nodiscard]] auto rend() const noexcept { return std::reverse_iterator{begin()}; }

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

		String &operator +=(const Char &chr) noexcept
			{ return append(chr); }
		String &operator +=(const String &str) noexcept
			{ return append(str); }

		[[nodiscard]] Char operator [](const size_t index) const
		{
			if (index >= _length)
				throw std::out_of_range{"String index is out of range"};
			const std::string_view str{_data};
			size_t offset{};
			for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{index})
				offset += Char{str.substr(offset)}.length();
			return {str.substr(offset)};
		}

		[[nodiscard]] StringView substr(size_t offset, size_t count = npos) const noexcept
		{
			const std::string_view str{_data};
			size_t begin{};
			// If the starting point is less than the number of characters in the string
			if (offset < _length)
			{
				// Figure out the offset for the first character requested
				for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{offset})
					begin += Char{str.substr(begin)}.length();
			}
			else
			{
				// Otherwise it's just the end of this string
				offset = _length;
				begin = str.length();
			}

			// Bring count into range
			if (count > _length - offset)
				count = _length - offset;

			auto end{begin};
			// Now we have [begin, ) but need to figure out the end offset - so go forward count characters
			for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{count})
				end += Char{str.substr(end)}.length();

			// Finally, we have the substring range and the number of UTF-8 characters in it, make the substring
			return {str.substr(begin, end), count};
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
			return std::equal(begin, end, _data.data() + _data.size() - str.byteLength());
		}

		[[nodiscard]] bool startsWith(const StringView &str) const noexcept
			{ return beginsWith(str); }

		bool operator ==(const String &str) const noexcept
			{ return _length == str._length && _data == str._data; }
		bool operator ==(const StringView &str) const noexcept
			{ return StringView{_data, _length} == str; }
		bool operator !=(const String &str) const noexcept
			{ return _length != str._length || _data != str._data; }
		bool operator !=(const StringView &str) const noexcept
			{ return StringView{_data, _length} != str; }

		bool operator <(const String &str) const noexcept
		{
			if (_length != str._length)
				return _length < str._length;
			return _data.compare(str._data) < 0;
		}

		bool operator <(const StringView &str) const noexcept
			{ return StringView{_data, _length} < str; }

		constexpr static size_t npos{SIZE_MAX};
	};

	inline namespace literals
	{
		inline String operator ""_s(const char *const value, const size_t length) noexcept
			{ return String{std::string_view{value, length}}; }
	} // namespace literals
} // namespace mangrove::core::utf8

template<> struct fmt::formatter<mangrove::core::utf8::String> : formatter<std::string_view>
{
	using String = mangrove::core::utf8::String;

	template<typename FormatContext> auto format(const String &str, FormatContext &ctx) const
	{
		const std::string_view data{str.data(), str.byteLength()};
		return formatter<std::string_view>::format(data, ctx);
	}
};

#endif /*CORE_UTF8_STRING_HXX*/
