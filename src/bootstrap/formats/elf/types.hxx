// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_TYPES_HXX
#define FORMATS_ELF_TYPES_HXX

#include <variant>
#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"
#include "elf32Types.hxx"
#include "elf64Types.hxx"

namespace mangrove::elf::types
{
	inline namespace internal
	{
		using mangrove::elf::io::overloaded_t;
	} // namespace internal

	struct elfHeader_t
	{
	private:
		std::variant<elf32::elfHeader_t, elf64::elfHeader_t> _header;

	public:
		template<typename T> elfHeader_t(T header) noexcept : _header{header} { }

		std::array<uint8_t, 4> magic() const;
		class_t elfClass() const;
		endian_t endian() const;
		abi_t abi() const;
		elfType_t type() const;
		machine_t machine() const;
		version_t version() const;
		uint64_t entryPoint() const;
		uint64_t phdrOffset() const;
		uint64_t shdrOffset() const;
		// auto flags() const noexcept { return _storage.read<uint32_t>(48, _endian); }
		// auto headerSize() const noexcept { return _storage.read<uint16_t>(52, _endian); }
		// auto programHeaderSize() const noexcept { return _storage.read<uint16_t>(54, _endian); }
		// auto programHeaderCount() const noexcept { return _storage.read<uint16_t>(56, _endian); }
		// auto sectionHeaderSize() const noexcept { return _storage.read<uint16_t>(58, _endian); }
		// auto sectionHeaderCount() const noexcept { return _storage.read<uint16_t>(60, _endian); }
		// auto sectionNamesIndex() const noexcept { return _storage.read<uint16_t>(62, _endian); }
	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_TYPES_HXX*/
