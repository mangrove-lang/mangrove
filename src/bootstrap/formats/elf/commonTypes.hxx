// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_COMMON_TYPES_HXX
#define FORMATS_ELF_COMMON_TYPES_HXX

#include "io.hxx"
#include "enums.hxx"

namespace mangrove::elf::types
{
	inline namespace internal
	{
		using mangrove::elf::io::memory_t;
		using namespace mangrove::elf::enums;
	} // namespace internal

	// This represents the magic number \x7f ELF
	constexpr static inline std::array<uint8_t, 4> elfMagic{{0x7fU, 0x45U, 0x4cU, 0x46U}};

	struct elfIdent_t
	{
	protected:
		memory_t _storage;
		endian_t _endian;

	public:
		elfIdent_t(const memory_t &storage) : _storage{storage}, _endian{_storage.read<endian_t>(5)} {}

		auto magic() const noexcept { return _storage.read<std::array<uint8_t, 4>>(0); }
		auto elfClass() const noexcept { return _storage.read<class_t>(4); }
		auto endian() const noexcept { return _endian; }
		auto version() const noexcept { return _storage.read<identVersion_t>(6); }
		auto abi() const noexcept { return _storage.read<abi_t>(7); }
		auto padding() const noexcept { return _storage.read<std::array<uint8_t, 8>>(8); }

		constexpr static size_t size() noexcept { return 16U; }
	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_COMMON_TYPES_HXX*/
