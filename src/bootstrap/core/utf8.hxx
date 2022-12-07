// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_HXX
#define CORE_UTF8_HXX

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <utility>

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
			const auto byteA{safeIndex(data, 0)};
			if (byteA & 0x80U)
			{
				const auto byteB{safeIndex(data, 1)};
				if ((byteA & 0x60U) == 0x40U && (byteB & 0xc0U) == 0x80U)
					return encode((uint32_t(byteA & 0x1fU) << 6U) | uint32_t(byteB & 0x3fU), 2);
				if ((byteB & 0xc0U) == 0x80U)
				{
					const auto byteC{safeIndex(data, 2)};
					if ((byteA & 0x70U) == 0x60U && (byteC & 0xc0U) == 0x80U)
						return encode((uint32_t(byteA & 0x0fU) << 12U) | (uint32_t(byteB & 0x3fU) << 6U) | (byteC & 0x3fU), 3U);
					if ((byteA & 0x78U) == 0x70U && (byteC & 0xc0U) == 0x80U)
					{
						const auto byteD{safeIndex(data, 3)};
						if ((byteD & 0xc0) == 0x80U)
							return  encode(
								(uint32_t(byteA & 0x07U) << 18U) | (uint32_t(byteB & 0x3fU) << 12U) | (uint32_t(byteC & 0x3fU) << 6U) | (byteD & 0x3fU), 4U
							);
					}
				}
				return invalidCodePoint;
			}
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

	public:
		constexpr static uint32_t invalidCodePoint{UINT32_MAX};

		constexpr Char() noexcept = default;
		constexpr Char(const char c) noexcept : _codePoint{encode(c)} { }
		constexpr Char(const uint32_t codePoint) noexcept : _codePoint{encode(codePoint)} { }
		constexpr Char(const std::string_view &value) noexcept : _codePoint{encode(value)} { }

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
		constexpr inline Char operator ""_u8c(const char c) noexcept { return {c}; }
		constexpr inline Char operator ""_c(const char *const value, const size_t length) noexcept
			{ return Char{std::string_view{value, length}}; }
	} // namespace literals
} // namespace mangrove::core::utf8

#endif /*CORE_UTF8_HXX*/
