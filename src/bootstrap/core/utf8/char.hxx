// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_CHAR_HXX
#define CORE_UTF8_CHAR_HXX

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <utility>
#include <substrate/fd>

namespace mangrove::core::utf8
{
	struct Char
	{
	private:
		uint32_t _codePoint{invalidCodePoint};

		constexpr static uint32_t codePointMask{0x00ffffffU};
		constexpr static uint32_t lengthMask{0xc0000000U};
		constexpr static uint32_t validMask{0x10000000U};
		constexpr static size_t lengthShift{30U};

		constexpr static uint32_t encode(const uint32_t codePoint, const size_t length)
			{ return (codePoint & codePointMask) | ((length - 1U) << lengthShift); }

		constexpr static inline uint8_t safeIndex(std::string_view str, const size_t index) noexcept
		{
			if (index > str.size())
				return UINT8_MAX;
			return uint8_t(str[index]);
		}

		constexpr static uint32_t encode(const std::string_view &data) noexcept
		{
			// To encode a UTF-8 character as a code point, start by reading a byte
			const auto byteA{safeIndex(data, 0)};
			// If the high bit is set, it should be a multi-byte sequence
			if (byteA & 0x80U)
			{
				// Read the next byte
				const auto byteB{safeIndex(data, 1)};
				// If the bits are in the pattern 0bx10xxxxx and 0b10xxxxxx, it's a 2-byte character
				if ((byteA & 0x60U) == 0x40U && (byteB & 0xc0U) == 0x80U)
					return encode((uint32_t(byteA & 0x1fU) << 6U) | uint32_t(byteB & 0x3fU), 2);
				// If the second byte read at least has 0b10xxxxxx as the pattern, it should be valid
				if ((byteB & 0xc0U) == 0x80U)
				{
					// Read the next byte
					const auto byteC{safeIndex(data, 2)};
					// If the bits of bytes 1 and 3 are in the pattern 0bx110xxxx and 0b10xxxxxx, it's a 3-byte character
					if ((byteA & 0x70U) == 0x60U && (byteC & 0xc0U) == 0x80U)
						return encode(
							(uint32_t(byteA & 0x0fU) << 12U) | (uint32_t(byteB & 0x3fU) << 6U) | (byteC & 0x3fU), 3U
						);
					// If the bits are instead in the pattern 0bx1110xxx and 0b10xxxxxx, we should have a 4-byte character
					if ((byteA & 0x78U) == 0x70U && (byteC & 0xc0U) == 0x80U)
					{
						// Read the final byte
						const auto byteD{safeIndex(data, 3)};
						// Validate it has the pattern 0b10xxxxxx
						if ((byteD & 0xc0) == 0x80U)
							return  encode(
								(uint32_t(byteA & 0x07U) << 18U) | (uint32_t(byteB & 0x3fU) << 12U) | (uint32_t(byteC & 0x3fU) << 6U) | (byteD & 0x3fU), 4U
							);
					}
				}
				// If any of the previous checks failed, we read a guf character
				return invalidCodePoint;
			}
			// We got a single byte character
			return encode(byteA, 1);
		}

		constexpr static uint32_t encode(const uint32_t codePoint) noexcept
		{
			if (codePoint < 0x80U)
				return encode(codePoint, 1U);
			if (codePoint < 0x0800U)
				return encode(codePoint, 2U);
			if (codePoint < 0x010000U)
				return encode(codePoint, 3U);
			else if (codePoint < 0x110000U)
				return encode(codePoint, 4U);
			return invalidCodePoint;
		}

		static uint32_t fromFile(const substrate::fd_t &file) noexcept
		{
			const auto readByte
			{
				[](const substrate::fd_t &file) noexcept -> uint8_t
				{
					uint8_t value{};
					const auto result{file.read(value)};
					if (result)
						return value;
					return UINT8_MAX;
				}
			};
			// This works the same as encode() above for std::string_view, the comments only cover the differnces.
			const auto byteA{readByte(file)};
			if (byteA & 0x80U)
			{
				const auto byteB{readByte(file)};
				// If the high bit of this byte is not set, it cannot be a continuation byte so step back one
				if (!(byteB & 0x80) && !file.seekRel(-1))
					return invalidCodePoint; // fast-return if the seek fails
				if ((byteA & 0x60U) == 0x40U && (byteB & 0xc0U) == 0x80U)
					return encode((uint32_t(byteA & 0x1fU) << 6U) | uint32_t(byteB & 0x3fU), 2);
				if ((byteB & 0xc0U) == 0x80U)
				{
					const auto byteC{readByte(file)};
					// Similary to byteC, check for continuation and step back if not
					if (!(byteC & 0x80) && !file.seekRel(-1))
						return invalidCodePoint; // fast-return if the seek fails
					if ((byteA & 0x70U) == 0x60U && (byteC & 0xc0U) == 0x80U)
						return encode((uint32_t(byteA & 0x0fU) << 12U) | (uint32_t(byteB & 0x3fU) << 6U) | (byteC & 0x3fU), 3U);
					if ((byteA & 0x78U) == 0x70U && (byteC & 0xc0U) == 0x80U)
					{
						const auto byteD{readByte(file)};
						if ((byteD & 0xc0) == 0x80U)
							return  encode(
								(uint32_t(byteA & 0x07U) << 18U) | (uint32_t(byteB & 0x3fU) << 12U) | (uint32_t(byteC & 0x3fU) << 6U) | (byteD & 0x3fU), 4U
							);
						// If byte D was not a continuation byte, step back one
						if (!file.seekRel(-1))
							return invalidCodePoint; // fast-return if the seek fails
					}
				}
				// Either we read a guf character, or one that was truncated.. either way..
				return invalidCodePoint;
			}
			return encode(byteA, 1);
		}

	public:
		constexpr static uint32_t invalidCodePoint{UINT32_MAX};

		constexpr Char() noexcept = default;
		constexpr Char(const Char &chr) noexcept : _codePoint{chr._codePoint} { }
		Char(Char &&chr) noexcept : Char{} { swap(chr); }
		constexpr Char(const char c) noexcept : _codePoint{encode(c)} { }
		constexpr Char(const uint32_t codePoint) noexcept : _codePoint{encode(codePoint)} { }
		constexpr Char(const std::string_view &value) noexcept : _codePoint{encode(value)} { }

		Char(const substrate::fd_t &file) noexcept : _codePoint{fromFile(file)} { }

		constexpr Char &operator =(const Char &chr) noexcept
		{
			_codePoint = chr._codePoint;
			return *this;
		}

		Char &operator =(Char &&chr) noexcept
		{
			swap(chr);
			return *this;
		}

		void swap(Char &chr) noexcept { std::swap(_codePoint, chr._codePoint); }
		constexpr bool valid() const noexcept { return !(_codePoint & validMask);}
		constexpr void indvalidate() noexcept { _codePoint |= validMask; }

		constexpr bool revalidate() noexcept
		{
			if (_codePoint == UINT32_MAX)
				return false;
			_codePoint &= ~validMask;
			return true;
		}

		constexpr uint32_t value() const noexcept
			{ return valid() ? (_codePoint & codePointMask) : UINT32_MAX; }
		constexpr size_t length() const noexcept
			{ return valid() ? ((_codePoint & lengthMask) >> lengthShift) + 1U : 3U; }
		constexpr uint32_t toCodePoint() const noexcept
			{ return valid() ? (_codePoint & codePointMask) : 0xfdffU; }
		constexpr void fromCodePoint(uint32_t codePoint) noexcept
			{ _codePoint = encode(codePoint); }

		constexpr bool operator ==(const Char &chr) const noexcept
			{ return _codePoint == chr._codePoint; }
		constexpr bool operator ==(const std::string_view &value) const noexcept
			{ return *this == Char{value}; }

		constexpr bool operator !=(const Char &chr) const noexcept
			{ return _codePoint != chr._codePoint; }
		constexpr bool operator !=(const std::string_view &value) const noexcept
			{ return *this != Char{value}; }

		constexpr bool operator <(const Char &chr) const noexcept
			{ return _codePoint < chr._codePoint; }
		constexpr bool operator <(const std::string_view &value) const noexcept
			{ return *this < Char{value}; }

		constexpr bool operator <=(const Char &chr) const noexcept
			{ return _codePoint <= chr._codePoint; }
		constexpr bool operator <=(const std::string_view &value) const noexcept
			{ return *this <= Char{value}; }

		constexpr bool operator >(const Char &chr) const noexcept
			{ return _codePoint > chr._codePoint; }
		constexpr bool operator >(const std::string_view &value) const noexcept
			{ return *this > Char{value}; }

		constexpr bool operator >=(const Char &chr) const noexcept
			{ return _codePoint >= chr._codePoint; }
		constexpr bool operator >=(const std::string_view &value) const noexcept
			{ return *this >= Char{value}; }
	};

	inline namespace literals
	{
		constexpr static inline Char operator ""_u8c(const char c) noexcept { return {c}; }
		constexpr static inline Char operator ""_c(const char *const value, const size_t length) noexcept
			{ return Char{std::string_view{value, length}}; }
	} // namespace literals
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_CHAR_HXX*/
