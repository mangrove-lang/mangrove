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
	struct ELFHeader final
	{
	private:
		std::variant<elf32::ELFHeader, elf64::ELFHeader> _header;

	public:
		template<typename T> ELFHeader(T header) noexcept : _header{header} { }

		[[nodiscard]] std::array<uint8_t, 4> magic() const noexcept;
		[[nodiscard]] Class elfClass() const noexcept;
		[[nodiscard]] Endian endian() const noexcept;
		[[nodiscard]] ABI abi() const noexcept;
		[[nodiscard]] Type type() const noexcept;
		[[nodiscard]] Machine machine() const noexcept;
		[[nodiscard]] Version version() const noexcept;
		[[nodiscard]] uint64_t entryPoint() const noexcept;
		[[nodiscard]] uint64_t phdrOffset() const noexcept;
		[[nodiscard]] uint64_t shdrOffset() const noexcept;
		[[nodiscard]] uint32_t flags() const noexcept;
		[[nodiscard]] uint16_t headerSize() const noexcept;
		[[nodiscard]] uint16_t programHeaderSize() const noexcept;
		[[nodiscard]] uint16_t programHeaderCount() const noexcept;
		[[nodiscard]] uint16_t sectionHeaderSize() const noexcept;
		[[nodiscard]] uint16_t sectionHeaderCount() const noexcept;
		[[nodiscard]] uint16_t sectionNamesIndex() const noexcept;
	};

	struct ProgramHeader final
	{
	private:
		std::variant<elf32::ProgramHeader, elf64::ProgramHeader> _header;

	public:
		template<typename T> ProgramHeader(T header) noexcept : _header{header} { }

		[[nodiscard]] ProgramHeaderType type() const noexcept;
		[[nodiscard]] uint32_t flags() const noexcept;
		[[nodiscard]] uint64_t offset() const noexcept;
		[[nodiscard]] uint64_t virtualAddress() const noexcept;
		[[nodiscard]] uint64_t physicalAddress() const noexcept;
		[[nodiscard]] uint64_t fileLength() const noexcept;
		[[nodiscard]] uint64_t memoryLength() const noexcept;
		[[nodiscard]] uint64_t alignment() const noexcept;

	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_TYPES_HXX*/
