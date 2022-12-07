// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_TYPES_HXX
#define FORMATS_ELF_TYPES_HXX

#include <variant>
#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"
#include "elf32Types.hxx"
#include "elf64Types.hxx"

/**
 * @file types.hxx
 * @brief High-level types representing the header structures and data in ELF files
 */

namespace mangrove::elf::types
{
	struct ELFHeader
	{
	private:
		std::variant<elf32::ELFHeader, elf64::ELFHeader> _header;

	public:
		template<typename T> ELFHeader(T header) noexcept : _header{header} { }

		std::array<uint8_t, 4> magic() const noexcept;
		Class elfClass() const noexcept;
		Endian endian() const noexcept;
		ABI abi() const noexcept;
		Type type() const noexcept;
		Machine machine() const noexcept;
		Version version() const noexcept;
		uint64_t entryPoint() const noexcept;
		uint64_t phdrOffset() const noexcept;
		uint64_t shdrOffset() const noexcept;
		uint32_t flags() const noexcept;
		uint16_t headerSize() const noexcept;
		uint16_t programHeaderSize() const noexcept;
		uint16_t programHeaderCount() const noexcept;
		uint16_t sectionHeaderSize() const noexcept;
		uint16_t sectionHeaderCount() const noexcept;
		uint16_t sectionNamesIndex() const noexcept;
	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_TYPES_HXX*/
