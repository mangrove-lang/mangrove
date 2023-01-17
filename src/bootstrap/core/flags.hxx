// SPDX-License-Identifier: BSD-3-Clause
#ifndef CORE_FLAGS_HXX
#define CORE_FLAGS_HXX

#include <type_traits>

namespace mangrove::core
{
	template<typename T, typename Enum> struct BitFlags
	{
	private:
		static_assert(std::is_integral_v<T>, "BitFlags must be backed by an integral type");
		static_assert(std::is_enum_v<Enum>, "BitFlags must be enumerated by an enum");
		T value{};

		using EnumInt = std::underlying_type_t<Enum>;
		constexpr static T flagAsBit(const Enum flag) noexcept { return T(1U << EnumInt(flag)); }

		// Internal value constructor to make .without() work
		constexpr BitFlags(const T &flags) noexcept : value{flags} { }

	public:
		constexpr BitFlags() noexcept = default;
		constexpr BitFlags(const BitFlags &flags) noexcept : value{flags.value} { }
		// move ctor omitted as it doesn't really make sense for this type
		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr BitFlags(const Values ...flags) noexcept : value{(flagAsBit(flags) | ...)} { }

		constexpr BitFlags &operator =(const BitFlags &flags) noexcept
		{
			if (&flags != this)
				value = flags.value;
			return *this;
		}

		// Move assignment omitted as it doesn't really make sense for this type.

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr BitFlags &operator =(const Values ...flags) noexcept
		{
			value = (flagAsBit(flags) | ...);
			return *this;
		}

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr void set(const Values ...flags) noexcept { value |= (flagAsBit(flags) | ...); }

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr void clear(const Values ...flags) noexcept { value &= T(~(flagAsBit(flags) | ...)); }

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			[[nodiscard]] constexpr bool includes(const Values ...flags) const noexcept
		{
			const T bits{(flagAsBit(flags) | ...)};
			return value & bits;
		}

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			[[nodiscard]] constexpr BitFlags without(const Values ...flags) const noexcept
		{
			const auto bits{(flagAsBit(flags) | ...)};
			const auto newValue{value & ~bits};
			return {T(newValue)};
		}

		[[nodiscard]] constexpr auto toRaw() const noexcept { return value; }

		constexpr bool operator ==(const BitFlags &flags) const noexcept { return value == flags.value; }
		constexpr bool operator ==(const Enum flag) const noexcept { return value == flagAsBit(flag); }
		constexpr bool operator !=(const BitFlags &flags) const noexcept { return value != flags.value; }
		constexpr bool operator !=(const Enum flag) const noexcept { return value != flagAsBit(flag); }
	};

	template<typename Enum> struct Flags
	{
	private:
		static_assert(std::is_enum_v<Enum>, "Flags must be enumerated by an enum");
		using EnumInt = std::underlying_type_t<Enum>;

		EnumInt value{};

	public:
		constexpr Flags() noexcept = default;
		constexpr Flags(const Flags &flags) noexcept : value{flags.value} { }
		// move ctor omitted as it doesn't really make sense for this type
		constexpr Flags(const EnumInt &flags) noexcept : value{flags} { }
		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr Flags(const Values ...flags) noexcept : value{(EnumInt(flags) | ...)} { }

		constexpr Flags &operator =(const Flags &flags) noexcept
		{
			if (&flags != this)
				value = flags.value;
			return *this;
		}

		// Move assignment omitted as it doesn't really make sense for this type.

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr Flags &operator =(const Values ...flags) noexcept
		{
			value = (EnumInt(flags) | ...);
			return *this;
		}

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr void set(const Values ...flags) noexcept { value |= (EnumInt(flags) | ...); }

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			constexpr void clear(const Values ...flags) noexcept { value &= EnumInt(~(EnumInt(flags) | ...)); }

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			[[nodiscard]] constexpr bool includes(const Values ...flags) const noexcept
		{
			const auto bits{(EnumInt(flags) | ...)};
			return value & bits;
		}

		template<typename... Values, typename = std::enable_if_t<(std::is_same_v<Values, Enum> && ...)>>
			[[nodiscard]] constexpr Flags without(const Values ...flags) const noexcept
		{
			const auto bits{(EnumInt(flags) | ...)};
			const auto newValue{value & ~bits};
			return {EnumInt(newValue)};
		}

		[[nodiscard]] constexpr auto toRaw() const noexcept { return value; }

		constexpr bool operator ==(const Flags &flags) const noexcept { return value == flags.value; }
		constexpr bool operator ==(const Enum flag) const noexcept { return value == flagAsBit(flag); }
		constexpr bool operator !=(const Flags &flags) const noexcept { return value != flags.value; }
		constexpr bool operator !=(const Enum flag) const noexcept { return value != flagAsBit(flag); }
	};
} // namespace mangrove::core

#endif /*CORE_FLAGS_HXX*/
