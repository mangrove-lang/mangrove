// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_UTF8_HELPERS_HXX
#define CORE_UTF8_HELPERS_HXX

#include <cstdint>
#include <cstddef>
#include <string_view>

namespace mangrove::core::utf8::helpers
{
	[[nodiscard]] constexpr static inline uint8_t safeIndex(std::string_view str, const size_t index) noexcept
	{
		if (index > str.size())
			return UINT8_MAX;
		return uint8_t(str[index]);
	}

	constexpr inline bool checkValid(uint8_t c) noexcept
		{ return (c & 0xc0U) == 0x80U; }

	template<typename... values_t> [[nodiscard]] constexpr inline std::enable_if_t<(std::is_same_v<values_t, uint8_t> && ...), bool>
		isMultiValid(values_t ...values) noexcept
			{ return (checkValid(values) && ...); }

	[[nodiscard]] constexpr static inline size_t countUnits(const std::string_view &str) noexcept
	{
		size_t count{};
		for (size_t i = 0; i < str.size(); ++i)
		{
			const auto byteA{safeIndex(str, i)};
			// Multiple code unit encoded character?
			if (byteA & 0x80U)
			{
				const auto byteB{safeIndex(str, ++i)};
				if ((byteA & 0x60U) == 0x40U)
				{
					// 2 code units.. check that the second unit is valid and return 0 if not.
					if (!isMultiValid(byteB))
						return 0;
				}
				else if ((byteA & 0x70U) == 0x60U)
				{
					// 3 code units.. check that the second and third units are valid and return 0 if not
					// Also check that the code point is valid (not D800-DFFF)
					if (!isMultiValid(byteB, safeIndex(str, ++i)) ||
						((byteA & 0x0fU) == 0x0dU && (byteB & 0x20U)))
						return 0;
				}
				else if ((byteA & 0x78U) == 0x70U)
				{
					// 4 code units.. check that the second, third and fourth unit is valid
					if (!isMultiValid(byteB, safeIndex(str, i + 1), safeIndex(str, i + 2)))
						return 0;
					i += 2;
				}
				else
					return 0;
			}
			++count;
		}
		return count;
	}
} // namespace mangrove::core::utf8::helpers

#endif /*CORE_UTF8_HELPERS_HXX*/
