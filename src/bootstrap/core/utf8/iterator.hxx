// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_ITERATOR_HXX
#define CORE_UTF8_ITERATOR_HXX

#include "char.hxx"

namespace mangrove::core::utf8::iterator
{
	struct StartAtEnd final  { };

	// Please note, this is ONLY valid on strings that are entirely valid UTF-8.
	struct StringIterator final
	{
	private:
		const char *_data;
		// length and offset are in bytes for simplicity..
		size_t _length;
		size_t _offset{};

	public:
		constexpr StringIterator(const char *data, size_t length) : _data{data}, _length{length} { }
		constexpr StringIterator(const std::string_view &str) : _data{str.data()}, _length{str.length()} { }
		constexpr StringIterator(const char *data, size_t length, StartAtEnd) :
			_data{data}, _length{length}, _offset{_length} { }
		constexpr StringIterator(const std::string_view &str, StartAtEnd) :
			_data{str.data()}, _length{str.length()}, _offset{_length} { }

		[[nodiscard]] constexpr Char operator *() const noexcept
			{ return {std::string_view{_data + _offset, _length}}; }

		constexpr StringIterator &operator ++() noexcept
		{
			if (_offset == _length)
				return *this;
			const auto chr{**this};
			_offset += chr.length();
			return *this;
		}

		constexpr StringIterator &operator --() noexcept
		{
			if (!_offset)
				return *this;
			auto offset{_offset - 1U};
			while (offset > 0 && (_data[offset] & 0xc0U) == 0x80U)
				--offset;
			_offset = offset;
			return *this;
		}

		[[nodiscard]] constexpr inline bool operator ==(const StringIterator &other) noexcept
			{  return _data == other._data && _length == other._length && _offset == other._offset; }
		[[nodiscard]] constexpr inline bool operator !=(const StringIterator &other) noexcept
			{  return _data != other._data || _length != other._length || _offset != other._offset; }

		using difference_type = ptrdiff_t;
		using value_type = Char;
		using pointer = void;
		using reference = Char;
		using iterator_category = std::bidirectional_iterator_tag;
	};
} // namespace mangrove::core::utf8::iterator

#endif /*CORE_UTF8_ITERATOR_HXX*/
