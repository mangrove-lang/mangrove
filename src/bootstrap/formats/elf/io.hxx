// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_IO_HXX
#define FORMATS_ELF_IO_HXX

#include <cstdint>
#include <array>
#include <cstring>
#include <type_traits>
#include <substrate/span>
#include "enums.hxx"

namespace mangrove::elf::io
{
	using substrate::span;

	inline namespace internal
	{
		using mangrove::elf::enums::endian_t;

		struct container_t
		{
			span<uint8_t> _data;

			void fromBytes(void *const value) const noexcept
				{ std::memcpy(value, _data.data(), _data.size()); }

			void fromBytesLE(uint16_t &value) const noexcept
			{
				std::array<uint8_t, 2> data{};
				fromBytes(data.data());
				value = (uint16_t(data[1]) << 8U) | data[0];
			}

			void fromBytesLE(uint32_t &value) const noexcept
			{
				std::array<uint8_t, 4> data{};
				fromBytes(data.data());
				value =
					(uint32_t(data[3]) << 24U) | (uint32_t(data[2]) << 16U) |
					(uint32_t(data[1]) << 8U) | data[0];
			}

			void fromBytesLE(uint64_t &value) const noexcept
			{
				std::array<uint8_t, 8> data{};
				fromBytes(data.data());
				value =
					(uint64_t(data[7]) << 56U) | (uint64_t(data[6]) << 48U) |
					(uint64_t(data[5]) << 40U) | (uint64_t(data[4]) << 32U) |
					(uint64_t(data[3]) << 24U) | (uint64_t(data[2]) << 16U) |
					(uint64_t(data[1]) << 8U) | data[0];
			}

			template<typename T> std::enable_if_t<
				std::is_integral_v<T> && !std::is_same_v<T, bool> &&
				std::is_signed_v<T> && sizeof(T) >= 2
			> fromBytesLE(T &value) const noexcept
			{
				std::make_unsigned_t<T> data{};
				fromBytesLE(data);
				value = static_cast<T>(data);
			}

			template<typename T> std::enable_if_t<std::is_enum_v<T>> fromBytesLE(T &value) const noexcept
			{
				std::underlying_type_t<T> data{};
				fromBytesLE(data);
				value = static_cast<T>(data);
			}

			void fromBytesBE(uint16_t &value) const noexcept
			{
				std::array<uint8_t, 2> data{};
				fromBytes(data.data());
				value = uint16_t((data[0] << 8U) | data[1]);
			}

			void fromBytesBE(uint32_t &value) const noexcept
			{
				std::array<uint8_t, 4> data{};
				fromBytes(data.data());
				value =
					(uint32_t(data[0]) << 24U) | (uint32_t(data[1]) << 16U) |
					(uint32_t(data[2]) << 8U) | data[3];
			}

			void fromBytesBE(uint64_t &value) const noexcept
			{
				std::array<uint8_t, 8> data{};
				fromBytes(data.data());
				value =
					(uint64_t(data[0]) << 56U) | (uint64_t(data[1]) << 48U) |
					(uint64_t(data[2]) << 40U) | (uint64_t(data[3]) << 32U) |
					(uint64_t(data[4]) << 24U) | (uint64_t(data[5]) << 16U) |
					(uint64_t(data[6]) << 8U) | data[7];
			}

			template<typename T> std::enable_if_t<
				std::is_integral_v<T> && !std::is_same_v<T, bool> &&
				std::is_signed_v<T> && sizeof(T) >= 2
			> fromBytesBE(T &value) const noexcept
			{
				std::make_unsigned_t<T> data{};
				fromBytesBE(data);
				value = static_cast<T>(data);
			}

			template<typename T> std::enable_if_t<std::is_enum_v<T>> fromBytesBE(T &value) const noexcept
			{
				std::underlying_type_t<T> data{};
				fromBytesBE(data);
				value = static_cast<T>(data);
			}
		};

		template<typename T> struct reader_t
		{
			container_t _data;
			reader_t(const span<uint8_t> &data) noexcept : _data{data.subspan(0, sizeof(T))} { }

			auto read() const noexcept
			{
				T value{};
				_data.fromBytes(&value);
				return value;
			}

			auto read(const endian_t endian) const noexcept
			{
				if (endian == endian_t::little)
					return readLE();
				else
					return readBE();
			}

			auto readLE() const noexcept
			{
				T value{};
				_data.fromBytesLE(value);
				return value;
			}

			auto readBE() const noexcept
			{
				T value{};
				_data.fromBytesBE(value);
				return value;
			}
		};

		template<typename T, size_t N> struct reader_t<std::array<T, N>>
		{
			container_t _data;
			reader_t(const span<uint8_t> &data) noexcept : _data{data.subspan(0, sizeof(T) * N)} { }

			auto read() const noexcept
			{
				std::array<T, N> value{};
				_data.fromBytes(value.data());
				return value;
			}
		};
	} // namespace internal

	struct memory_t final
	{
	private:
		span<uint8_t> _data;

	public:
		memory_t(span<uint8_t> storage) : _data{storage} { }

		[[nodiscard]] size_t length() const noexcept { return _data.size(); }

		template<typename T> [[nodiscard]] auto read(const size_t offset) const noexcept
			{ return reader_t<T>{_data.subspan(offset)}.read(); }

		template<typename T> [[nodiscard]] auto read(const size_t offset, const endian_t endian) const noexcept
			{ return reader_t<T>{_data.subspan(offset)}.read(endian); }
	};

	template<typename... Ts> struct overloaded_t : Ts... { using Ts::operator()...; };
	template<typename... Ts> overloaded_t(Ts...) -> overloaded_t<Ts...>;
} // namespace mangrove:::elf::io

#endif /*FORMATS_ELF_IO_HXX*/
