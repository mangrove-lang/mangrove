// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_COMMON_TYPES_HXX
#define FORMATS_ELF_COMMON_TYPES_HXX

#include "io.hxx"
#include "enums.hxx"

namespace mangrove::elf::types
{
	inline namespace internal
	{
		using mangrove::elf::io::Memory;
		using namespace mangrove::elf::enums;
	} // namespace internal

	// This represents the magic number \x7f ELF
	constexpr static inline std::array<uint8_t, 4> elfMagic{{0x7fU, 0x45U, 0x4cU, 0x46U}};

	struct ELFIdent
	{
	protected:
		// NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
		Memory _storage;
		// NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
		Endian _endian;

	public:
		ELFIdent(const Memory &storage) : _storage{storage}, _endian{_storage.read<Endian>(5)} {}

		[[nodiscard]] auto magic() const noexcept { return _storage.read<std::array<uint8_t, 4>>(0); }
		[[nodiscard]] auto elfClass() const noexcept { return _storage.read<Class>(4); }
		[[nodiscard]] auto endian() const noexcept { return _endian; }
		[[nodiscard]] auto version() const noexcept { return _storage.read<IdentVersion>(6); }
		[[nodiscard]] auto abi() const noexcept { return _storage.read<ABI>(7); }
		[[nodiscard]] auto padding() const noexcept { return _storage.read<std::array<uint8_t, 8>>(8); }

		[[nodiscard]] constexpr static size_t size() noexcept { return 16U; }
	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_COMMON_TYPES_HXX*/
