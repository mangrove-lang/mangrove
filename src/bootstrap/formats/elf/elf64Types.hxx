// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF64_TYPES_HXX
#define FORMATS_ELF64_TYPES_HXX

#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"

namespace mangrove::elf::types::elf64
{
	struct ELFHeader : ELFIdent
	{
	public:
		ELFHeader(const Memory &storage) : ELFIdent{storage} { }

		[[nodiscard]] auto type() const noexcept { return _storage.read<Type>(16, _endian); }
		[[nodiscard]] auto machine() const noexcept { return _storage.read<Machine>(18, _endian); }
		[[nodiscard]] auto version() const noexcept { return _storage.read<Version>(20, _endian); }
		[[nodiscard]] auto entryPoint() const noexcept { return _storage.read<uint64_t>(24, _endian); }
		[[nodiscard]] auto phdrOffset() const noexcept { return _storage.read<uint64_t>(32, _endian); }
		[[nodiscard]] auto shdrOffset() const noexcept { return _storage.read<uint64_t>(40, _endian); }
		[[nodiscard]] auto flags() const noexcept { return _storage.read<uint32_t>(48, _endian); }
		[[nodiscard]] auto headerSize() const noexcept { return _storage.read<uint16_t>(52, _endian); }
		[[nodiscard]] auto programHeaderSize() const noexcept { return _storage.read<uint16_t>(54, _endian); }
		[[nodiscard]] auto programHeaderCount() const noexcept { return _storage.read<uint16_t>(56, _endian); }
		[[nodiscard]] auto sectionHeaderSize() const noexcept { return _storage.read<uint16_t>(58, _endian); }
		[[nodiscard]] auto sectionHeaderCount() const noexcept { return _storage.read<uint16_t>(60, _endian); }
		[[nodiscard]] auto sectionNamesIndex() const noexcept { return _storage.read<uint16_t>(62, _endian); }

		[[nodiscard]] bool valid() const noexcept
		{
			return
				magic() == elfMagic &&
				ELFIdent::version() == IdentVersion::current &&
				version() == Version::current &&
				headerSize() == size();
		}
		[[nodiscard]] constexpr static size_t size() noexcept { return ELFIdent::size() + 48; }
	};
} // namespace mangrove::elf::types::elf64

#endif /*FORMATS_ELF64_TYPES_HXX*/
