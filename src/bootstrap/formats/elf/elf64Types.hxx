// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF64_TYPES_HXX
#define FORMATS_ELF64_TYPES_HXX

#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"

namespace mangrove::elf::types::elf64
{
	struct elfHeader_t : elfIdent_t
	{
	public:
		elfHeader_t(const memory_t &storage) : elfIdent_t{storage} { }

		auto type() const noexcept { return _storage.read<elfType_t>(16, _endian); }
		auto machine() const noexcept { return _storage.read<machine_t>(18, _endian); }
		auto version() const noexcept { return _storage.read<version_t>(20, _endian); }
		auto entryPoint() const noexcept { return _storage.read<uint64_t>(24, _endian); }
		auto phdrOffset() const noexcept { return _storage.read<uint64_t>(32, _endian); }
		auto shdrOffset() const noexcept { return _storage.read<uint64_t>(40, _endian); }
		auto flags() const noexcept { return _storage.read<uint32_t>(48, _endian); }
		auto headerSize() const noexcept { return _storage.read<uint16_t>(52, _endian); }
		auto programHeaderSize() const noexcept { return _storage.read<uint16_t>(54, _endian); }
		auto programHeaderCount() const noexcept { return _storage.read<uint16_t>(56, _endian); }
		auto sectionHeaderSize() const noexcept { return _storage.read<uint16_t>(58, _endian); }
		auto sectionHeaderCount() const noexcept { return _storage.read<uint16_t>(60, _endian); }
		auto sectionNamesIndex() const noexcept { return _storage.read<uint16_t>(62, _endian); }

		bool valid() const noexcept
		{
			return
				magic() == elfMagic &&
				elfIdent_t::version() == identVersion_t::current &&
				version() == version_t::current &&
				headerSize() == size();
		}
		constexpr static size_t size() noexcept { return elfIdent_t::size() + 48; }
	};
} // namespace mangrove::elf::types::elf64

#endif /*FORMATS_ELF64_TYPES_HXX*/
